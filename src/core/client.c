/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#include "protocol/protocol.h"
#include "criterion/logging.h"
#include "criterion/options.h"
#include "io/event.h"
#include "report.h"
#include "stats.h"
#include "client.h"

void nothing();

enum protocol_version {
    PROTOCOL_V1 = 1,
};

KHASH_MAP_INIT_INT(ht_client, struct client_ctx)

static enum client_state msg_to_state[] = {
    [criterion_protocol_submessage_birth_tag]       = CS_BIRTH,
    [criterion_protocol_submessage_pre_init_tag]    = CS_PRE_INIT,
    [criterion_protocol_submessage_pre_test_tag]    = CS_PRE_TEST,
    [criterion_protocol_submessage_post_test_tag]   = CS_POST_TEST,
    [criterion_protocol_submessage_post_fini_tag]   = CS_POST_FINI,
    [criterion_protocol_submessage_death_tag]       = CS_DEATH,
};

static const char *state_to_string[] = {
    [CS_BIRTH]     = "BIRTH",
    [CS_PRE_INIT]  = "PRE_INIT",
    [CS_PRE_TEST]  = "PRE_TEST",
    [CS_POST_TEST] = "POST_TEST",
    [CS_POST_FINI] = "POST_FINI",
    [CS_DEATH]     = "DEATH",
};

typedef void message_handler(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);

void handle_birth(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
void handle_pre_init(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
void handle_pre_test(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
void handle_post_test(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);
void handle_death(struct server_ctx *, struct client_ctx *, const criterion_protocol_msg *);

static message_handler *message_handlers[] = {
    [criterion_protocol_submessage_birth_tag]       = handle_birth,
    [criterion_protocol_submessage_pre_init_tag]    = handle_pre_init,
    [criterion_protocol_submessage_pre_test_tag]    = handle_pre_test,
    [criterion_protocol_submessage_post_test_tag]   = handle_post_test,
    [criterion_protocol_submessage_death_tag]       = handle_death,
};

static void get_message_id(char *out, size_t n, const criterion_protocol_msg *msg) {
    switch (msg->which_id) {
        case criterion_protocol_msg_pid_tag:
            snprintf(out, n, "[PID %" PRId64 "]", msg->id.pid); return;
        default: break;
    }
}

void init_server_context(struct server_ctx *sctx) {
    sctx->subprocesses = kh_init(ht_client);
}

void destroy_server_context(struct server_ctx *sctx) {
    kh_destroy(ht_client, sctx->subprocesses);
}

struct client_ctx *add_client_from_worker(struct server_ctx *sctx, struct client_ctx *ctx, struct worker *w) {
    unsigned long long pid = get_process_id_of(w->proc);
    int absent;
    khint_t k = kh_put(ht_client, sctx->subprocesses, pid, &absent);
    ctx->worker = w;
    ctx->kind = WORKER;
    kh_value(sctx->subprocesses, k) = *ctx;
    return &kh_value(sctx->subprocesses, k);
}

void remove_client_by_pid(struct server_ctx *sctx, int pid) {
    khint_t k = kh_get(ht_client, sctx->subprocesses, pid);
    if (k != kh_end(sctx->subprocesses))
        kh_del(ht_client, sctx->subprocesses, pid);
}

static void process_client_message_impl(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg) {
    if (msg->data.which_value >= CS_MAX_CLIENT_STATES
            || msg_to_state[msg->data.which_value] == 0) {
        char id[32];
        get_message_id(id, sizeof (id), msg);

        criterion_perror("%s: Received message with malformed data.value tag '%d'.",
                id,
                msg->data.which_value);
        return;
    }

    enum client_state new_state = msg_to_state[msg->data.which_value];
    if (new_state < CS_DEATH && new_state != ctx->state + 1) {
        char id[32];
        get_message_id(id, sizeof (id), msg);

        criterion_perror("%s: Expected message to change to state '%s', got '%s' instead.",
                id,
                state_to_string[ctx->state + 1],
                state_to_string[new_state]);
        return;
    }

    message_handler *handler = message_handlers[msg->data.which_value];
    if (handler)
        handler(sctx, ctx, msg);

    if (new_state <= CS_DEATH)
        ctx->state = new_state;
}

struct client_ctx *process_client_message(struct server_ctx *ctx, const criterion_protocol_msg *msg) {
    if (msg->version != PROTOCOL_V1) {
        criterion_perror("Received message using invalid protocol version number '%d'.", msg->version);
        return NULL;
    }

    switch (msg->which_id) {
        case criterion_protocol_msg_pid_tag: {
            khiter_t k = kh_get(ht_client, ctx->subprocesses, msg->id.pid);
            if (k != kh_end(ctx->subprocesses)) {
                process_client_message_impl(ctx, &kh_value(ctx->subprocesses, k), msg);
                return &kh_value(ctx->subprocesses, k);
            } else {
                char id[32];
                get_message_id(id, sizeof (id), msg);

                criterion_perror("%s: Received message identified by a PID "
                        "that is not a child process.",
                        id);
            }
        } break;
        default: {
            criterion_perror("Received message with malformed id tag '%d'.",
                    criterion_protocol_msg_pid_tag);
        } break;
    }
    return NULL;
}

#define push_event(...)                                             \
    do {                                                            \
        push_event_noreport(__VA_ARGS__);                           \
        report(CR_VA_HEAD(__VA_ARGS__), ctx->tstats);               \
    } while (0)

#define push_event_noreport(...)                                    \
    do {                                                            \
        stat_push_event(ctx->gstats,                                \
                ctx->sstats,                                        \
                ctx->tstats,                                        \
                &(struct event) {                                   \
                    .kind = CR_VA_HEAD(__VA_ARGS__),                \
                    CR_VA_TAIL(__VA_ARGS__)                         \
                });                                                 \
    } while (0)

void handle_birth(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg) {
    (void) sctx;
    (void) ctx;
    (void) msg;
}

void handle_pre_init(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg) {
    (void) sctx;
    (void) msg;

    push_event_noreport(PRE_INIT);
    report(PRE_INIT, ctx->test);
    log(pre_init, ctx->test);
}

void handle_pre_test(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg) {
    (void) sctx;
    (void) msg;

    report(PRE_TEST, ctx->test);
    log(pre_test, ctx->test);
}

void handle_post_test(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg) {
    (void) sctx;
    (void) msg;

    double elapsed_time = 0; // TODO: restore elapsed time handling
    push_event_noreport(POST_TEST, .data = &elapsed_time);
    report(POST_TEST, ctx->tstats);
    log(post_test, ctx->tstats);
}

void handle_death(struct server_ctx *sctx, struct client_ctx *ctx, const criterion_protocol_msg *msg) {
    (void) sctx;
    const criterion_protocol_death *death = &msg->data.value.death;
    switch (death->result) {
        case criterion_protocol_death_result_type_CRASH: {
            if (ctx->state >= CS_POST_TEST || ctx->state < CS_PRE_TEST) {
                log(other_crash, ctx->tstats);

                if (ctx->state < CS_PRE_TEST) {
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
        case criterion_protocol_death_result_type_TIMEOUT: {
            ctx->tstats->timed_out = true;
            double elapsed_time = ctx->test->data->timeout;
            if (elapsed_time == 0 && ctx->suite->data)
                elapsed_time = ctx->suite->data->timeout;
            push_event(POST_TEST, .data = &elapsed_time);
            push_event(POST_FINI);
            log(test_timeout, ctx->tstats);
        } break;
        case criterion_protocol_death_result_type_ABORT: {
            if (ctx->state < CS_POST_TEST) {
                double elapsed_time = 0;
                push_event(POST_TEST, .data = &elapsed_time);
                log(post_test, ctx->tstats);
            }
            if (ctx->state < CS_POST_FINI) {
                push_event(POST_FINI);
                log(post_fini, ctx->tstats);
            }
        } break;
        case criterion_protocol_death_result_type_NORMAL: {
            if ((ctx->state >= CS_POST_TEST && ctx->state < CS_POST_FINI) || ctx->state < CS_PRE_TEST) {
                log(abnormal_exit, ctx->tstats);
                if (ctx->state < CS_PRE_TEST) {
                    stat_push_event(ctx->gstats,
                            ctx->sstats,
                            ctx->tstats,
                            &(struct event) { .kind = TEST_CRASH });
                }
                return;
            }
            ctx->tstats->exit_code = death->status;
            if (ctx->state < CS_POST_TEST) {
                if (ctx->test->data->exit_code == 0) {
                    push_event(TEST_CRASH);
                    log(abnormal_exit, ctx->tstats);
                } else {
                    double elapsed_time = 0;
                    push_event(POST_TEST, .data = &elapsed_time);
                    log(post_test, ctx->tstats);
                    push_event(POST_FINI);
                    log(post_fini, ctx->tstats);
                }
            }
        } break;
        default: break;
    }
}
