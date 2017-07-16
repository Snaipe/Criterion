/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <inttypes.h>
#include <csptr/smalloc.h>
#include "compat/strtok.h"
#include "protocol/protocol.h"
#include "protocol/messages.h"
#include "criterion/options.h"
#include "log/logging.h"
#include "io/event.h"
#include "client.h"
#include "err.h"
#include "report.h"
#include "stats.h"

KHASH_MAP_INIT_INT(ht_client, struct client_ctx)
KHASH_MAP_INIT_STR(ht_extern, struct client_ctx)

static enum client_state phase_to_state[] = {
    [criterion_protocol_phase_kind_SETUP]    = CS_SETUP,
    [criterion_protocol_phase_kind_MAIN]     = CS_MAIN,
    [criterion_protocol_phase_kind_TEARDOWN] = CS_TEARDOWN,
    [criterion_protocol_phase_kind_END]      = CS_END,
    [criterion_protocol_phase_kind_ABORT]    = CS_ABORT,
    [criterion_protocol_phase_kind_TIMEOUT]  = CS_TIMEOUT,
    [criterion_protocol_phase_kind_SKIP]     = CS_SKIP,
};

static const char *state_to_string[] = {
    [CS_SETUP]      = "setup",
    [CS_MAIN]       = "main",
    [CS_TEARDOWN]   = "teardown",
    [CS_END]        = "end",
    [CS_ABORT]      = "abort",
    [CS_TIMEOUT]    = "timeout",
    [CS_SKIP]       = "skip",
};

typedef bool message_handler (struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
typedef bool phase_handler (struct server_ctx *, struct client_ctx *, const criterion_protocol_phase *);

bool handle_birth(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
bool handle_phase(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
bool handle_death(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
bool handle_assert(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
bool handle_message(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
bool handle_statistic(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);

static message_handler *message_handlers[] = {
    [criterion_protocol_submessage_birth_tag]       = handle_birth,
    [criterion_protocol_submessage_phase_tag]       = handle_phase,
    [criterion_protocol_submessage_death_tag]       = handle_death,
    [criterion_protocol_submessage_assert_tag]      = handle_assert,
    [criterion_protocol_submessage_message_tag]     = handle_message,
    [criterion_protocol_submessage_statistic_tag]   = handle_statistic,
};

static void get_message_id(char *out, size_t n, const criterion_protocol_msg *msg)
{
    switch (msg->which_id) {
        case criterion_protocol_msg_pid_tag:
            snprintf(out, n, "[PID %" PRId64 "]", msg->id.pid); return;
        case criterion_protocol_msg_uid_tag:
            snprintf(out, n, "[external \"%s\"]", msg->id.uid); return;
        default: break;
    }
}

void init_server_context(struct server_ctx *sctx, struct criterion_global_stats *gstats)
{
    sctx->subprocesses = kh_init(ht_client);
    sctx->clients = kh_init(ht_extern);

    sctx->gstats = gstats;
    sctx->extern_suite = (struct criterion_suite) {
        .name = "external",
        .data = &sctx->extern_suite_data,
    };
    sctx->extern_suite_data = (struct criterion_test_extra_data) {
        .disabled = 0,
    };
    sctx->extern_sstats = suite_stats_init(&sctx->extern_suite);
}

void destroy_client_context(struct client_ctx *ctx)
{
    if (ctx->kind == WORKER) {
        int rc = bxf_wait(ctx->instance, BXF_FOREVER);
        if (rc < 0)
            cr_panic("waiting for the worker failed: %s\n", strerror(-rc));
        rc = bxf_term(ctx->instance);
        if (rc < 0)
            cr_panic("finalizing the worker failed: %s\n", strerror(-rc));
    }
    sfree(ctx->tstats);
    sfree(ctx->sstats);
}

void destroy_server_context(struct server_ctx *sctx)
{
    khint_t k;
    struct client_ctx v;

    kh_foreach(sctx->subprocesses, k, v, {
        destroy_client_context(&v);
    });

    (void) k;
    (void) v;

    kh_destroy(ht_client, sctx->subprocesses);

    kh_destroy(ht_extern, sctx->clients);

    sfree(sctx->extern_sstats);
}

struct client_ctx *add_client_from_worker(struct server_ctx *sctx,
        struct client_ctx *ctx, bxf_instance *instance)
{
    unsigned long long pid = instance->pid;
    int absent;
    khint_t k = kh_put(ht_client, sctx->subprocesses, pid, &absent);

    ctx->instance = instance;
    ctx->kind = WORKER;
    kh_value(sctx->subprocesses, k) = *ctx;
    return &kh_value(sctx->subprocesses, k);
}

void remove_client_by_pid(struct server_ctx *sctx, int pid)
{
    khint_t k = kh_get(ht_client, sctx->subprocesses, pid);

    if (k != kh_end(sctx->subprocesses)) {
        destroy_client_context(&kh_value(sctx->subprocesses, k));
        kh_del(ht_client, sctx->subprocesses, k);
    }
}

struct client_ctx *add_external_client(struct server_ctx *sctx, char *id)
{
    int absent;
    khint_t k = kh_put(ht_extern, sctx->clients, id, &absent);

    kh_value(sctx->clients, k) = (struct client_ctx) {
        .kind = EXTERN,
        .extern_test = {
            .name     = strdup(id),
            .category = "external",
        },
        .gstats = sctx->gstats,
        .sstats = sref(sctx->extern_sstats),
    };

    struct client_ctx *ctx = &kh_value(sctx->clients, k);
    ctx->test = &ctx->extern_test;
    ctx->suite = &sctx->extern_suite;
    ctx->extern_test.data = &ctx->extern_test_data;
    ctx->tstats = test_stats_init(&ctx->extern_test);

    return ctx;
}

static void process_client_message_impl(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg)
{
    message_handler *handler = message_handlers[msg->data.which_value];
    bool ack = false;

    if (handler)
        ack = handler(sctx, ctx, msg);

    if (!ack)
        send_ack(sctx->socket, true, NULL);
}

#define handler_error(Ctx, IdFmt, Id, Fmt, ...)            \
    do {                                                   \
        criterion_perror(IdFmt Fmt "\n", Id, __VA_ARGS__); \
        send_ack((Ctx)->socket, false, Fmt, __VA_ARGS__);  \
    } while (0)

struct client_ctx *process_client_message(struct server_ctx *ctx, const criterion_protocol_msg *msg)
{
    if (msg->version != PROTOCOL_V1) {
        handler_error(ctx, "%s", "", "Received message using invalid protocol version number '%" PRIi32 "'.", msg->version);
        return NULL;
    }

    struct client_ctx *client = NULL;
    switch (msg->which_id) {
        case criterion_protocol_msg_pid_tag: {
            khiter_t k = kh_get(ht_client, ctx->subprocesses, msg->id.pid);
            if (k != kh_end(ctx->subprocesses))
                client = &kh_value(ctx->subprocesses, k);
            else
                handler_error(ctx, "%s", "", "Received message identified by a PID '%" PRIi64 "' "
                        "that is not a child process.", msg->id.pid);
        } break;
        case criterion_protocol_msg_uid_tag: {
            khiter_t k = kh_get(ht_extern, ctx->clients, msg->id.uid);
            bool client_found = k != kh_end(ctx->clients);
            if (!client_found && msg->data.which_value == criterion_protocol_submessage_birth_tag)
                client = add_external_client(ctx, msg->id.uid);
            else if (client_found)
                client = &kh_value(ctx->clients, k);
            else
                handler_error(ctx, "%s", "", "Received message identified by the ID '%s'"
                        "that did not send a birth message previously.", msg->id.uid);
        } break;
        default: {
            handler_error(ctx, "%s", "", "Received message with malformed id tag '%d'.\n",
                    criterion_protocol_msg_pid_tag);
        } break;
    }

    if (client)
        process_client_message_impl(ctx, client, msg);
    return client;
}

#define push_event(...)                               \
    do {                                              \
        push_event_noreport(__VA_ARGS__);             \
        report(CR_VA_HEAD(__VA_ARGS__), ctx->tstats); \
    } while (0)

#define push_event_noreport(...)             \
    do {                                     \
        stat_push_event(ctx->gstats,         \
                ctx->sstats,                 \
                ctx->tstats,                 \
                &(struct event) {            \
            .kind = CR_VA_HEAD(__VA_ARGS__), \
            CR_VA_TAIL(__VA_ARGS__)          \
        });                                  \
    } while (0)

bool handle_birth(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg)
{
    (void) sctx;
    (void) msg;

    ctx->alive = true;
    return false;
}

bool handle_pre_init(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_phase *msg)
{
    (void) sctx;
    (void) msg;

    if (ctx->state == 0) { /* only pre_init if there are no nested states */
        push_event_noreport(PRE_INIT);
        report(PRE_INIT, ctx->test);
        log(pre_init, ctx->suite, ctx->test);
    }
    return false;
}

bool handle_pre_test(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_phase *msg)
{
    (void) sctx;
    (void) msg;

    if (ctx->state < CS_MAX_CLIENT_STATES) {
        ctx->start_time = msg->timestamp;
        push_event_noreport(PRE_TEST);
        report(PRE_TEST, ctx->test);
        log(pre_test, ctx->suite, ctx->test);
    }
    return false;
}

bool handle_post_test(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_phase *msg)
{
    (void) sctx;
    (void) msg;

    if (ctx->state < CS_MAX_CLIENT_STATES) {
        double elapsed_time = (msg->timestamp - ctx->start_time) / 1e9;
        push_event_noreport(POST_TEST, .data = &elapsed_time);
        report(POST_TEST, ctx->tstats);
        log(post_test, ctx->tstats);
    }
    return false;
}

bool handle_post_fini(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_phase *msg)
{
    (void) sctx;
    (void) ctx;
    (void) msg;
    if (ctx->state < CS_MAX_CLIENT_STATES) {
        push_event(POST_FINI);
        log(post_fini, ctx->tstats);
    }
    return false;
}

bool handle_abort(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_phase *msg)
{
    (void) sctx;
    (void) ctx;
    (void) msg;

    enum client_state curstate = ctx->state & (CS_MAX_CLIENT_STATES - 1);

    if (ctx->state < CS_MAX_CLIENT_STATES) {
        ctx->tstats->test_status = CR_STATUS_FAILED;
        log(test_abort, ctx->tstats, msg->message ? msg->message : "");

        if (curstate < CS_TEARDOWN) {
            double elapsed_time = 0;
            push_event(POST_TEST, .data = &elapsed_time);
            log(post_test, ctx->tstats);
        }
        if (curstate < CS_END) {
            push_event(POST_FINI);
            log(post_fini, ctx->tstats);
        }
    } else {
        struct criterion_theory_stats ths = {
            .formatted_args = strdup(msg->message),
            .stats          = ctx->tstats,
        };
        report(THEORY_FAIL, &ths);
        log(theory_fail, &ths);
    }
    return false;
}

bool handle_timeout(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_phase *msg)
{
    (void) sctx;
    (void) msg;

    if (ctx->state < CS_MAX_CLIENT_STATES) {
        ctx->tstats->timed_out = true;
        double elapsed_time = ctx->instance->time.elapsed / 1000000000.;
        push_event(POST_TEST, .data = &elapsed_time);
        push_event(POST_FINI);
        log(test_timeout, ctx->tstats);
    }
    return false;
}

bool handle_skip(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_phase *msg)
{
    (void) sctx;
    (void) msg;

    if (ctx->state < CS_MAX_CLIENT_STATES) {
        ctx->tstats->test_status = CR_STATUS_SKIPPED;
        ctx->tstats->message = msg->message ? strdup(msg->message) : NULL;
        double elapsed_time = 0;
        push_event(POST_TEST, .data = &elapsed_time);
        push_event(POST_FINI);
        log(post_test, ctx->tstats);
    }
    return false;
}
#define MAX_TEST_DEPTH    16

bool handle_phase(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg)
{
    const criterion_protocol_phase *phase_msg = &msg->data.value.phase;

    enum client_state new_state = phase_to_state[phase_msg->phase];
    enum client_state curstate = ctx->state & (CS_MAX_CLIENT_STATES - 1);

    if (new_state == CS_SETUP) {
        if (ctx->state != 0 && ctx->state != CS_MAIN) {
            char id[32];
            get_message_id(id, sizeof (id), msg);

            handler_error(sctx, "%s: ", id, "Cannot spawn a subtest outside of the '%s' test phase.", state_to_string[CS_MAIN]);
            return true;
        }
        if (ctx->state & (0x3u << (MAX_TEST_DEPTH - 1) * 2)) {
            char id[32];
            get_message_id(id, sizeof (id), msg);

            handler_error(sctx, "%s: ", id, "Cannot nest more than %d tests at a time.", MAX_TEST_DEPTH);
            return true;
        }
    } else if (curstate == CS_END) {
        char id[32];
        get_message_id(id, sizeof (id), msg);

        handler_error(sctx, "%s: ", id, "The test has already ended, invalid state '%s'.", state_to_string[new_state]);
        return true;
    } else if (curstate < CS_END && new_state <= CS_END && new_state != curstate + 1) {
        char id[32];
        get_message_id(id, sizeof (id), msg);

        handler_error(sctx, "%s: ", id, "Expected message to change to state '%s', got '%s' instead.",
                state_to_string[ctx->state + 1],
                state_to_string[new_state]);
        return true;
    }

    static phase_handler *handlers[] = {
        [CS_SETUP]      = handle_pre_init,
        [CS_MAIN]       = handle_pre_test,
        [CS_TEARDOWN]   = handle_post_test,
        [CS_END]        = handle_post_fini,
        [CS_ABORT]      = handle_abort,
        [CS_TIMEOUT]    = handle_timeout,
        [CS_SKIP]       = handle_skip,
    };

    bool ack = handlers[new_state](sctx, ctx, phase_msg);

    if (new_state >= CS_END) {
        if ((ctx->state >> 2) != 0)
            ctx->state >>= 2; /* pop the current state */
        else
            ctx->state = CS_END;
    } else if (new_state == CS_SETUP) {
        ctx->state <<= 2; /* shift the state to make space for a new state */
    } else {
        ++ctx->state;
    }

    return ack;
}

bool handle_death(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg)
{
    (void) sctx;

    ctx->alive = false;

    const criterion_protocol_death *death = &msg->data.value.death;
    enum client_state curstate = ctx->state & (CS_MAX_CLIENT_STATES - 1);
    switch (death->result) {
        case criterion_protocol_death_result_type_CRASH: {
            if (curstate != CS_MAIN) {
                log(other_crash, ctx->tstats);
                ++ctx->gstats->warnings;

                if (ctx->state < CS_MAIN) {
                    stat_push_event(ctx->gstats,
                            ctx->sstats,
                            ctx->tstats,
                            &(struct event) { .kind = TEST_CRASH });
                }
            } else {
                ctx->tstats->signal = death->status;
                if (ctx->test->data->signal == 0) {
                    push_event(TEST_CRASH);
                    log(test_crash, ctx->tstats);
                } else {
                    double elapsed_time = 0;
                    push_event(POST_TEST, .data = &elapsed_time);
                    log(post_test, ctx->tstats);
                    push_event(POST_FINI);
                    log(post_fini, ctx->tstats);
                }
            }
        } break;
        case criterion_protocol_death_result_type_NORMAL: {
            if (curstate == CS_TEARDOWN || ctx->state == CS_SETUP) {
                log(abnormal_exit, ctx->tstats);
                if (ctx->state == CS_SETUP) {
                    stat_push_event(ctx->gstats,
                            ctx->sstats,
                            ctx->tstats,
                            &(struct event) { .kind = TEST_CRASH });
                }
                break;
            }
            ctx->tstats->exit_code = death->status;
            if (ctx->state == CS_MAIN) {
                double elapsed_time = 0;
                push_event(POST_TEST, .data = &elapsed_time);
                log(post_test, ctx->tstats);
                push_event(POST_FINI);
                log(post_fini, ctx->tstats);
            }
        } break;
        default: break;
    }
    return false;
}

static void make_param(struct cr_log_assert_param *param,
        criterion_protocol_param_entry *entry)
{
    *param = (struct cr_log_assert_param) {
        .name = entry->name,
    };
    if (entry->which_data == criterion_protocol_param_entry_raw_tag) {
        param->size = entry->data.raw->size;
        param->data = entry->data.raw->bytes;
        param->kind = CR_LOG_PARAM_RAW;
    } else if (entry->which_data == criterion_protocol_param_entry_str_tag) {
        param->size = strlen(entry->data.str);
        param->data = entry->data.str;
        param->kind = CR_LOG_PARAM_STR;
    }
}

bool handle_assert(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg)
{
    (void) sctx;
    (void) ctx;
    (void) msg;
    const criterion_protocol_assert *asrt = &msg->data.value.assert;
    struct criterion_assert_stats asrt_stats = {
        .message = asrt->message,
        .passed  = asrt->passed,
        .line    = asrt->has_line ? asrt->line : 0,
        .file    = asrt->file ? asrt->file : "unknown",
    };

    push_event_noreport(ASSERT, .data = &asrt_stats);
    report(ASSERT, &asrt_stats);
    log(assert, &asrt_stats);
    if (asrt->results_count > 0) {
        for (size_t i = 0; i < asrt->results_count; ++i) {
            criterion_protocol_result *res = &asrt->results[i];
            if (res->value.params->list_count == 0 && !res->message) {
                continue;
            }

            log(assert_sub, &asrt_stats, res->repr, res->message);

            if (res->which_value == criterion_protocol_result_params_tag) {
                size_t j = 0;
                if (res->value.params->list_count >= 2) {
                    criterion_protocol_param_entry *actual = &res->value.params->list[0];
                    criterion_protocol_param_entry *expected = &res->value.params->list[1];
                    if (!strcmp(actual->name, "actual") && !strcmp(expected->name, "expected")) {
                        struct cr_log_assert_param pa, pe;
                        make_param(&pa, actual);
                        make_param(&pe, expected);
                        log(assert_param_eq, &asrt_stats, &pa, &pe);
                        j += 2;
                    }
                }

                for (; j < res->value.params->list_count; ++j) {
                    struct cr_log_assert_param p;

                    make_param(&p, &res->value.params->list[j]);
                    log(assert_param, &asrt_stats, &p);
                }
            } else if (res->which_value == criterion_protocol_result_formatted_tag) {
                log(assert_formatted, &asrt_stats, res->value.formatted);
            }
        }
    }
    return false;
}

bool handle_statistic(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg)
{
    (void) sctx;
    const criterion_protocol_statistic *stat = &msg->data.value.statistic;
    /* TODO: handle this better with new statistics API */
    if (!strcmp(stat->key, ".asserts_passed") && stat->which_value == criterion_protocol_statistic_num_tag) {
        ctx->tstats->passed_asserts += stat->value.num;
        ctx->sstats->asserts_passed += stat->value.num;
        ctx->sstats->nb_asserts     += stat->value.num;
        ctx->gstats->asserts_passed += stat->value.num;
        ctx->gstats->nb_asserts     += stat->value.num;
    }
    return false;
}

bool handle_message(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg)
{
    (void) sctx;
    (void) ctx;
    const criterion_protocol_log *lg = &msg->data.value.message;
    enum criterion_severity severity = (enum criterion_severity) lg->severity;

    log(message, severity, lg->message);
    if (severity == CR_LOG_WARNING)
        ++ctx->gstats->warnings;
    if (severity == CR_LOG_ERROR)
        ++ctx->gstats->errors;
    return false;
}
