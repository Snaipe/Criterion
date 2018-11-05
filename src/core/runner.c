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
#include <nanomsg/nn.h>
#include "criterion/internal/test.h"
#include "criterion/options.h"
#include "criterion/internal/ordered-set.h"
#include "criterion/internal/preprocess.h"
#include "criterion/redirect.h"
#include "protocol/protocol.h"
#include "protocol/connect.h"
#include "protocol/messages.h"
#include "compat/alloc.h"
#include "compat/time.h"
#include "compat/posix.h"
#include "compat/processor.h"
#include "compat/kill.h"
#include "string/extglobmatch.h"
#include "string/i18n.h"
#include "io/event.h"
#include "io/output.h"
#include "log/logging.h"
#include "abort.h"
#include "client.h"
#include "common.h"
#include "config.h"
#include "err.h"
#include "report.h"
#include "runner.h"
#include "runner_coroutine.h"
#include "stats.h"

#ifdef __GNUC__
# include <valgrind/valgrind.h>
#else
# define ENABLE_VALGRIND_ERRORS
# define RUNNING_ON_VALGRIND    0
#endif

int cri_is_runner;

typedef const char *const msg_t;

#ifdef ENABLE_NLS
static msg_t msg_valgrind_jobs = N_("%1$sWarning! Criterion has detected "
                "that it is running under valgrind, but the number of jobs have been "
                "explicitely set. Reports might appear confusing!%2$s\n");
#else
static msg_t msg_valgrind_jobs = "%sWarning! Criterion has detected "
        "that it is running under valgrind, but the number of jobs have been "
        "explicitely set. Reports might appear confusing!%s\n";
#endif

static int cmp_suite(void *a, void *b)
{
    struct criterion_suite *s1 = a, *s2 = b;

    return strcmp(s1->name, s2->name);
}

static int cmp_test(void *a, void *b)
{
    struct criterion_test *s1 = a, *s2 = b;

    return strcmp(s1->name, s2->name);
}

static void dtor_suite_set(void *ptr, CR_UNUSED void *meta)
{
    struct criterion_suite_set *s = ptr;

    sfree(s->tests);
}

static void dtor_test_set(void *ptr, CR_UNUSED void *meta)
{
    struct criterion_test_set *t = ptr;

    sfree(t->suites);
}

CR_API void criterion_register_test(struct criterion_test_set *set,
        struct criterion_test *test)
{
    struct criterion_suite_set css = {
        .suite = { .name = test->category },
    };
    struct criterion_suite_set *s = insert_ordered_set(set->suites, &css, sizeof (css));

    if (!s->tests)
        s->tests = new_ordered_set(cmp_test, NULL);

    insert_ordered_set(s->tests, test, sizeof (*test));
    ++set->tests;
}

struct criterion_test_set *criterion_init(void)
{
    struct criterion_ordered_set *suites = new_ordered_set(cmp_suite, dtor_suite_set);

    struct cri_section *sections = NULL;

    if (!cri_sections_getaddr("cr_sts", &sections)) {
        for (struct cri_section *s = sections; s->addr; ++s) {
            void *start = s->addr;
            void *end = (char *) start + s->length;

            FOREACH_SUITE_SEC(s, start, end) {
                if (!*s || !*(*s)->name)
                    continue;

                struct criterion_suite_set css = {
                    .suite = **s,
                };
                insert_ordered_set(suites, &css, sizeof (css));
            }
        }
    }
    free(sections);

    struct criterion_test_set *set = smalloc(
        .size = sizeof (struct criterion_test_set),
        .dtor = dtor_test_set);

    *set = (struct criterion_test_set) {
        suites,
        0,
    };

    sections = NULL;
    if (!cri_sections_getaddr("cr_tst", &sections)) {
        for (struct cri_section *s = sections; s->addr; ++s) {
            void *start = s->addr;
            void *end = (char *) start + s->length;

            FOREACH_TEST_SEC(test, start, end) {
                if (!*test)
                    continue;

                if (!*(*test)->category || !*(*test)->name)
                    continue;

                criterion_register_test(set, *test);
            }
        }
    }
    free(sections);

    return set;
}

#define push_event(...)                                   \
    do {                                                  \
        stat_push_event(ctx->stats,                       \
                ctx->suite_stats,                         \
                ctx->test_stats,                          \
                &(struct event) {                         \
            .kind = CR_VA_HEAD(__VA_ARGS__),              \
            CR_VA_TAIL(__VA_ARGS__)                       \
        });                                               \
        report(CR_VA_HEAD(__VA_ARGS__), ctx->test_stats); \
    } while (0)

void disable_unmatching(struct criterion_test_set *set)
{
    if (!compile_pattern(criterion_options.pattern))
        exit(3);
    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        if ((s->suite.data && s->suite.data->disabled) || !s->tests)
            continue;

        FOREACH_SET(struct criterion_test *test, s->tests) {
            int ret = match(test->data->identifier_);

            if (ret == 0)
                test->data->disabled = true;
        }
    }
    free_pattern();
}

CR_API struct criterion_test_set *criterion_initialize(void)
{
    /* make sure we don't re-enter from a test worker. See #247. */
    if (getenv("BXFI_MAP")) {
        cr_panic("Re-entering criterion from a test worker. This is a "
                 "catastrophic bug, please report it on the issue tracker.\n"
                 "Bailing out to avoid fork-bombing the system.");
    }

    init_i18n();

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_DISABLE_ERROR_REPORTING;
#endif
    if (RUNNING_ON_VALGRIND) {
        criterion_options.no_early_exit = 1;
        criterion_options.jobs = 1;
    }

    criterion_register_output_provider("tap", tap_report);
    criterion_register_output_provider("xml", xml_report);
    criterion_register_output_provider("json", json_report);

    setup_parent_job();

    return criterion_init();
}

CR_API void criterion_finalize(struct criterion_test_set *set)
{
    sfree(set);

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_ENABLE_ERROR_REPORTING;
#endif

    criterion_free_output();
}

static struct client_ctx *spawn_next_client(struct server_ctx *sctx, ccrContext *ctx)
{
    struct client_ctx new_ctx;

    bxf_instance *instance = cri_run_next_test(NULL, NULL, NULL, &new_ctx, ctx);

    if (!instance)
        return NULL;

    return add_client_from_worker(sctx, &new_ctx, instance);
}

static void run_tests_async(struct criterion_test_set *set,
        struct criterion_global_stats *stats,
        const char *url,
        int socket)
{
    ccrContext ctx = 0;

    size_t nb_workers = DEF(criterion_options.jobs, get_processor_count());
    size_t active_workers = 0;
    int has_msg = 0;

    struct server_ctx sctx;

    init_server_context(&sctx, stats);

    sctx.socket = socket;

    /* initialization of coroutine */
    cri_run_next_test(set, stats, url, NULL, &ctx);

    for (size_t i = 0; i < nb_workers; ++i) {
        struct client_ctx *cctx = spawn_next_client(&sctx, &ctx);

        if (!cctx)
            break;
        ++active_workers;
    }

    if (!active_workers && !criterion_options.wait_for_clients)
        goto cleanup;

    criterion_protocol_msg msg = criterion_protocol_msg_init_zero;
    while ((has_msg = read_message(socket, &msg)) == 1) {
        struct client_ctx *cctx = process_client_message(&sctx, &msg);

        /* drop invalid messages */
        if (!cctx)
            continue;

        if (!cctx->alive) {
            if ((cctx->tstats->test_status == CR_STATUS_FAILED) && criterion_options.fail_fast)
                cr_terminate(cctx->gstats);

            if (cctx->kind == WORKER) {
                remove_client_by_pid(&sctx, cctx->instance->pid);

                cctx = spawn_next_client(&sctx, &ctx);

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

static int criterion_run_all_tests_impl(struct criterion_test_set *set)
{
    cri_report_init();

    if (RUNNING_ON_VALGRIND) {
        if (criterion_options.jobs != 1)
            criterion_pimportant(CRITERION_PREFIX_DASHES,
                    _(msg_valgrind_jobs), CR_FG_BOLD, CR_RESET);
    }

    char url[sizeof ("ipc://" NN_SOCKET_PATH "criterion_.sock") + 21];
    snprintf(url, sizeof (url), "ipc://" NN_SOCKET_PATH "criterion_%llu.sock", get_process_id());

    int sock = cri_proto_bind(url);
    if (sock < 0)
        cr_panic("Could not initialize the message server: %s.", strerror(errno));

    g_client_socket = cri_proto_connect(url);
    if (g_client_socket < 0)
        cr_panic("Could not initialize the message client: %s.", strerror(errno));

    cri_alloc_init();

    report(PRE_ALL, set);
    log(pre_all, set);

    struct criterion_global_stats *stats = stats_init();
    run_tests_async(set, stats, url, sock);

    report(POST_ALL, stats);
    if (criterion_options.logging_threshold == CRITERION_LOG_LEVEL_QUIET) {
        cri_restore_outputs();
    }
    process_all_output(stats);
    log(post_all, stats);

    cri_alloc_term();
    cri_report_term();

    cri_proto_close(g_client_socket);
    cri_proto_close(sock);
    int ok = stats->tests_failed == 0 && stats->errors == 0;
    if (!criterion_options.ignore_warnings)
        ok = ok && stats->warnings == 0;
    sfree(stats);
    return ok;
}

CR_API int criterion_run_all_tests(struct criterion_test_set *set)
{
    /* make sure we don't re-enter from a test worker. See #247. */
    if (getenv("BXFI_MAP")) {
        cr_panic("Re-entering criterion from a test worker. This is a "
                 "catastrophic bug, please report it on the issue tracker.\n"
                 "Bailing out to avoid fork-bombing the system.");
    }
#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_DISABLE_ERROR_REPORTING;
#endif

    cri_is_runner = 1;

    if (criterion_options.pattern)
        disable_unmatching(set);

    if (criterion_options.debug) {
        criterion_options.jobs = 1;
        criterion_options.crash = true;
        criterion_options.logging_threshold = 1;
    }

    if (criterion_options.logging_threshold == CRITERION_LOG_LEVEL_QUIET) {
        cri_silence_outputs();
    }

    int res = criterion_run_all_tests_impl(set);

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_ENABLE_ERROR_REPORTING;
#endif
    return criterion_options.always_succeed || res;
}
