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
#define CRITERION_LOGGING_COLORS
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <csptr/smalloc.h>
#include <valgrind/valgrind.h>
#include <nanomsg/nn.h>
#include "criterion/internal/test.h"
#include "criterion/options.h"
#include "criterion/internal/ordered-set.h"
#include "criterion/internal/preprocess.h"
#include "criterion/redirect.h"
#include "protocol/protocol.h"
#include "protocol/connect.h"
#include "protocol/messages.h"
#include "compat/time.h"
#include "compat/posix.h"
#include "compat/processor.h"
#include "compat/kill.h"
#include "string/i18n.h"
#include "io/event.h"
#include "io/output.h"
#include "log/logging.h"
#include "runner_coroutine.h"
#include "stats.h"
#include "runner.h"
#include "report.h"
#include "worker.h"
#include "abort.h"
#include "config.h"
#include "common.h"
#include "client.h"

#include "string/extglobmatch.h"

typedef const char *const msg_t;

#ifdef ENABLE_NLS
static msg_t msg_valgrind_early_exit = N_("%1$sWarning! Criterion has detected "
        "that it is running under valgrind, but the no_early_exit option is "
        "explicitely disabled. Reports will not be accurate!%2$s\n");

static msg_t msg_valgrind_jobs = N_("%1$sWarning! Criterion has detected "
        "that it is running under valgrind, but the number of jobs have been "
        "explicitely set. Reports might appear confusing!%2$s\n");
#else
static msg_t msg_valgrind_early_exit = "%sWarning! Criterion has detected "
        "that it is running under valgrind, but the no_early_exit option is "
        "explicitely disabled. Reports will not be accurate!%s\n";

static msg_t msg_valgrind_jobs = "%sWarning! Criterion has detected "
        "that it is running under valgrind, but the number of jobs have been "
        "explicitely set. Reports might appear confusing!%s\n";
#endif


#ifdef _MSC_VER
struct criterion_test  *CR_SECTION_START_(cr_tst);
struct criterion_suite *CR_SECTION_START_(cr_sts);
struct criterion_test  *CR_SECTION_END_(cr_tst);
struct criterion_suite *CR_SECTION_END_(cr_sts);
#endif

CR_IMPL_SECTION_LIMITS(struct criterion_test*, cr_tst);
CR_IMPL_SECTION_LIMITS(struct criterion_suite*, cr_sts);

// This is here to make the test suite & test sections non-empty
CR_SECTION_("cr_sts") struct criterion_suite *dummy_suite = NULL;
CR_SECTION_("cr_tst") struct criterion_test  *dummy_test = NULL;

static INLINE void nothing(void) {}

int cmp_suite(void *a, void *b) {
    struct criterion_suite *s1 = a, *s2 = b;
    return strcmp(s1->name, s2->name);
}

int cmp_test(void *a, void *b) {
    struct criterion_test *s1 = a, *s2 = b;
    return strcmp(s1->name, s2->name);
}

static void dtor_suite_set(void *ptr, CR_UNUSED void *meta) {
    struct criterion_suite_set *s = ptr;
    sfree(s->tests);
}

static void dtor_test_set(void *ptr, CR_UNUSED void *meta) {
    struct criterion_test_set *t = ptr;
    sfree(t->suites);
}

void criterion_register_test(struct criterion_test_set *set,
                                    struct criterion_test *test) {

    struct criterion_suite_set css = {
        .suite = { .name = test->category },
    };
    struct criterion_suite_set *s = insert_ordered_set(set->suites, &css, sizeof (css));
    if (!s->tests)
        s->tests = new_ordered_set(cmp_test, NULL);

    insert_ordered_set(s->tests, test, sizeof(*test));
    ++set->tests;
}

struct criterion_test_set *criterion_init(void) {
    struct criterion_ordered_set *suites = new_ordered_set(cmp_suite, dtor_suite_set);

    FOREACH_SUITE_SEC(s) {
        if (!*s || !*(*s)->name)
            continue;

        struct criterion_suite_set css = {
            .suite = **s,
        };
        insert_ordered_set(suites, &css, sizeof (css));
    }

    struct criterion_test_set *set = smalloc(
            .size = sizeof (struct criterion_test_set),
            .dtor = dtor_test_set
        );

    *set = (struct criterion_test_set) {
        suites,
        0,
    };

    FOREACH_TEST_SEC(test) {
        if (!*test)
            continue;

        if (!*(*test)->category || !*(*test)->name)
            continue;

        criterion_register_test(set, *test);
    }

    return set;
}

const struct criterion_test  *criterion_current_test;
const struct criterion_suite *criterion_current_suite;

void run_test_child(struct criterion_test *test,
                    struct criterion_suite *suite) {
    if (!is_single_mode())
        reset_proc_compat();

    cr_redirect_stdin();
    g_client_socket = connect_client();
    if (g_client_socket < 0) {
        criterion_perror("Could not initialize the message client: %s.\n",
                strerror(errno));
        abort();
    }

    // Notify the runner that the test was born
    criterion_protocol_msg msg = criterion_message(birth, .name = (char *) test->name);
    criterion_message_set_id(msg);
    cr_send_to_runner(&msg);

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_ENABLE_ERROR_REPORTING;
#endif

    criterion_current_test = test;
    criterion_current_suite = suite;

    if (suite->data && suite->data->timeout != 0 && test->data->timeout == 0)
        setup_timeout((uint64_t) (suite->data->timeout * 1e9));
    else if (test->data->timeout != 0)
        setup_timeout((uint64_t) (test->data->timeout * 1e9));

    if (test->test)
        test->test();

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_DISABLE_ERROR_REPORTING;
#endif

    close_socket(g_client_socket);

    fflush(NULL); // flush all opened streams
    if (criterion_options.no_early_exit)
        return;
    _Exit(0);
}

#define push_event(...)                                             \
    do {                                                            \
        stat_push_event(ctx->stats,                                 \
                ctx->suite_stats,                                   \
                ctx->test_stats,                                    \
                &(struct event) {                                   \
                    .kind = CR_VA_HEAD(__VA_ARGS__),                \
                    CR_VA_TAIL(__VA_ARGS__)                         \
                });                                                 \
        report(CR_VA_HEAD(__VA_ARGS__), ctx->test_stats);           \
    } while (0)

s_pipe_handle *g_worker_pipe;

void disable_unmatching(struct criterion_test_set *set) {
    if (!compile_pattern(criterion_options.pattern)) {
        exit(3);
    }
    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        if ((s->suite.data && s->suite.data->disabled) || !s->tests)
            continue;

        FOREACH_SET(struct criterion_test *test, s->tests) {
            int ret = match(test->data->identifier_);
            if (ret == 0) {
                test->data->disabled = true;
            }
        }
    }
    free_pattern();
}

struct criterion_test_set *criterion_initialize(void) {
    init_i18n();

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_DISABLE_ERROR_REPORTING;
#endif
    if (RUNNING_ON_VALGRIND) {
        criterion_options.no_early_exit = 1;
        criterion_options.jobs = 1;
    }

    if (resume_child()) // (windows only) resume from the fork
        exit(0);

    criterion_register_output_provider("tap", tap_report);
    criterion_register_output_provider("xml", xml_report);
    criterion_register_output_provider("json", json_report);

    setup_parent_job();

    return criterion_init();
}

void criterion_finalize(struct criterion_test_set *set) {
    sfree(set);

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_ENABLE_ERROR_REPORTING;
#endif

    criterion_free_output();
}

static struct client_ctx *spawn_next_client(struct server_ctx *sctx, ccrContext *ctx) {
    struct worker *w = ctx ? run_next_test(NULL, NULL, ctx) : NULL;

    if (!is_runner() || w == NULL)
        return NULL;

    struct client_ctx new_ctx = (struct client_ctx) {
        .test = w->ctx.test,
        .tstats = w->ctx.test_stats,
        .suite = w->ctx.suite,
        .sstats = w->ctx.suite_stats,
        .gstats = w->ctx.stats,
    };

    return add_client_from_worker(sctx, &new_ctx, w);
}

static void run_tests_async(struct criterion_test_set *set,
                            struct criterion_global_stats *stats,
                            int socket) {

    ccrContext ctx = 0;

    size_t nb_workers = DEF(criterion_options.jobs, get_processor_count());
    size_t active_workers = 0;
    int has_msg = 0;

    struct server_ctx sctx;
    init_server_context(&sctx, stats);

    sctx.socket = socket;

    // initialization of coroutine
    run_next_test(set, stats, &ctx);

    for (size_t i = 0; i < nb_workers; ++i) {
        struct client_ctx *cctx = spawn_next_client(&sctx, &ctx);
        if (!is_runner())
            goto cleanup;

        if (!cctx)
            break;
        ++active_workers;
    }

    if (!active_workers && !criterion_options.wait_for_clients)
        goto cleanup;

    criterion_protocol_msg msg = criterion_protocol_msg_init_zero;
    while ((has_msg = read_message(socket, &msg)) == 1) {
        struct client_ctx *cctx = process_client_message(&sctx, &msg);

        // drop invalid messages
        if (!cctx)
            continue;

        if (!cctx->alive) {
            if (cctx->tstats->failed && criterion_options.fail_fast) {
                cr_terminate(cctx->gstats);
            }

            if (cctx->kind == WORKER) {
                remove_client_by_pid(&sctx, get_process_id_of(cctx->worker->proc));

                cctx = spawn_next_client(&sctx, &ctx);
                if (!is_runner())
                    goto cleanup;

                if (cctx == NULL)
                    --active_workers;
            }
        }

        if (!active_workers && !criterion_options.wait_for_clients)
            break;

        free_message(&msg);
    }

cleanup:
    if (has_msg)
        free_message(&msg);
    destroy_server_context(&sctx);
    ccrAbort(ctx);
}

static int criterion_run_all_tests_impl(struct criterion_test_set *set) {
    report(PRE_ALL, set);
    log(pre_all, set);

    if (RUNNING_ON_VALGRIND) {
        if (!criterion_options.no_early_exit)
            criterion_pimportant(CRITERION_PREFIX_DASHES,
                    _(msg_valgrind_early_exit), CR_FG_BOLD, CR_RESET);
        if (criterion_options.jobs != 1)
            criterion_pimportant(CRITERION_PREFIX_DASHES,
                    _(msg_valgrind_jobs), CR_FG_BOLD, CR_RESET);
    }

    fflush(NULL); // flush everything before forking

    int sock = bind_server();
    if (sock < 0) {
        criterion_perror("Could not initialize the message server: %s.\n",
                strerror(errno));
        abort();
    }

    init_proc_compat();

    g_client_socket = connect_client();
    if (g_client_socket < 0) {
        criterion_perror("Could not initialize the message client: %s.\n",
                strerror(errno));
        abort();
    }

    struct criterion_global_stats *stats = stats_init();
    run_tests_async(set, stats, sock);

    int result = is_runner() ? stats->tests_failed == 0 : -1;

    if (!is_runner())
        goto cleanup;

    report(POST_ALL, stats);
    process_all_output(stats);
    log(post_all, stats);

cleanup:
    free_proc_compat();
    if (is_runner()) {
        close_socket (g_client_socket);
        close_socket (sock);
    }
    sfree(stats);
    return result;
}

int criterion_run_all_tests(struct criterion_test_set *set) {
    if (criterion_options.pattern) {
        disable_unmatching(set);
    }

    set_runner_process();
    int res = criterion_run_all_tests_impl(set);
    unset_runner_process();

    if (res == -1) {
        criterion_finalize(set);
        exit(0);
    }

    return criterion_options.always_succeed || res;
}

void run_single_test_by_name(const char *testname) {
    struct criterion_test_set *set = criterion_init();

    struct criterion_test *test = NULL;
    struct criterion_suite *suite = NULL;

    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        size_t tests = s->tests ? s->tests->size : 0;
        if (!tests)
            continue;

        FOREACH_SET(struct criterion_test *t, s->tests) {
            char name[1024];
            snprintf(name, sizeof (name), "%s::%s", s->suite.name, t->name);
            if (!strncmp(name, testname, 1024)) {
                test = t;
                suite = &s->suite;
                break;
            }
        }
    }

    if (test) {
        is_extern_worker = true;
        criterion_current_test = test;
        criterion_current_suite = suite;

        run_test_child(test, suite);
    }

    sfree(set);
}
