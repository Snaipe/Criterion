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
#include <stdlib.h>
#include <stdio.h>
#include <csptr/smalloc.h>

#include "criterion/internal/parameterized.h"
#include "criterion/redirect.h"
#include "compat/alloc.h"
#include "compat/time.h"
#include "compat/posix.h"
#include "compat/processor.h"
#include "compat/kill.h"
#include "log/logging.h"
#include "protocol/protocol.h"
#include "protocol/connect.h"
#include "protocol/messages.h"
#include "string/i18n.h"
#include "client.h"
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

typedef const char *const msg_t;

#ifdef ENABLE_NLS
static msg_t msg_print_pid = N_("%1$s::%2$s: Started test has PID %3$lu.\n");
#else
static msg_t msg_print_pid = "%s::%s: Started test has PID %lu.\n";
#endif

/* *INDENT-OFF* - This is a structure definition in disguise */
ccrBeginDefineContextType(run_next_context);

    struct criterion_suite_set *suite_set;
    struct criterion_test *test;
    struct criterion_suite_stats *suite_stats;
    struct criterion_test_stats *test_stats;
    struct criterion_test_set *set;
    struct criterion_global_stats *stats;
    struct criterion_test_params params;

    struct criterion_ordered_set_node *ns;
    struct criterion_ordered_set_node *nt;
    size_t i;
    const char *url;

ccrEndDefineContextType;
/* *INDENT-ON* */

CR_API const struct criterion_test *criterion_current_test;
CR_API const struct criterion_suite *criterion_current_suite;

static int serialize_test(bxf_context ctx, struct criterion_test *test,
        struct criterion_suite *suite)
{
    int rc = 0;

    if (!rc)
        rc = bxf_context_addobject(ctx, "criterion.test.name",
                        test->name, strlen(test->name) + 1);
    if (!rc)
        rc = bxf_context_addobject(ctx, "criterion.test.suite",
                        test->category, strlen(test->category) + 1);
    if (!rc && test->test)
        rc = bxf_context_addfnaddr(ctx, "criterion.test.test",
                        test->test);
    if (!rc)
        rc = bxf_context_addobject(ctx, "criterion.test.extra",
                        test->data, sizeof (*test->data));
    if (!rc && test->data->init)
        rc = bxf_context_addfnaddr(ctx, "criterion.test.extra.init",
                        test->data->init);
    if (!rc && test->data->fini)
        rc = bxf_context_addfnaddr(ctx, "criterion.test.extra.fini",
                        test->data->fini);
    if (!rc)
        rc = bxf_context_addobject(ctx, "criterion.suite.name",
                        suite->name, strlen(suite->name) + 1);

    if (suite->data) {
        if (!rc)
            rc = bxf_context_addobject(ctx, "criterion.suite.extra",
                            suite->data, sizeof (*suite->data));
        if (!rc && suite->data->init)
            rc = bxf_context_addfnaddr(ctx, "criterion.suite.extra.init",
                            suite->data->init);
        if (!rc && suite->data->fini)
            rc = bxf_context_addfnaddr(ctx, "criterion.suite.extra.fini",
                            suite->data->fini);
    }
    return rc;
}

static int deserialize_test(struct criterion_test *test,
        struct criterion_suite *suite)
{
    bxf_context ctx = bxf_context_current();

    struct criterion_test_extra_data *test_data = NULL;
    struct criterion_test_extra_data *suite_data = NULL;

    int rc;

    rc = bxf_context_getobject(ctx, "criterion.test.name",
                    (void **) &test->name);
    if (rc <= 0) goto err;

    rc = bxf_context_getobject(ctx, "criterion.test.suite",
                    (void **) &test->category);
    if (rc <= 0) goto err;

    rc = bxf_context_getfnaddr(ctx, "criterion.test.test",
                    &test->test);
    if (rc < 0) goto err;

    rc = bxf_context_getobject(ctx, "criterion.test.extra",
                    (void **) &test_data);
    if (rc <= 0) goto err;

    rc = bxf_context_getfnaddr(ctx, "criterion.test.extra.init",
                    &test_data->init);
    if (rc < 0) goto err;

    rc = bxf_context_getfnaddr(ctx, "criterion.test.extra.fini",
                    &test_data->fini);
    if (rc < 0) goto err;

    rc = bxf_context_getobject(ctx, "criterion.suite.name",
                    (void **) &suite->name);
    if (rc <= 0) goto err;

    rc = bxf_context_getobject(ctx, "criterion.suite.extra",
                    (void **) &suite_data);
    if (rc < 0) goto err;

    if (suite_data) {
        rc = bxf_context_getfnaddr(ctx, "criterion.suite.extra.init",
                        &suite_data->init);
        if (rc < 0) goto err;

        rc = bxf_context_getfnaddr(ctx, "criterion.suite.extra.fini",
                        &suite_data->fini);
        if (rc < 0) goto err;
    }

    test->data = test_data;
    suite->data = suite_data;

    return 1;
err:
    return rc;
}

static int run_test_child(void)
{
#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_DISABLE_ERROR_REPORTING;
#endif

    struct criterion_test test;
    memset(&test, 0, sizeof (test));
    struct criterion_suite suite;
    memset(&suite, 0, sizeof (suite));

    const char *url;

    bxf_context ctx = bxf_context_current();

    int rc = deserialize_test(&test, &suite);
    if (rc > 0)
        rc = bxf_context_getobject(ctx, "criterion.url", (void **) &url);
    if (rc < 0)
        cr_panic("Could not get the test context: %s", strerror(-rc));
    else if (!rc)
        cr_panic("Could not initialize test context: property not found");

    cr_redirect_stdin();
    g_client_socket = cri_proto_connect(url);

    if (g_client_socket < 0)
        cr_panic("could not initialize the message client: %s", strerror(errno));

    /* Notify the runner that the test was born */
    criterion_protocol_msg msg = criterion_message(birth, .name = (char *) test.name);
    criterion_message_set_id(msg);
    cr_send_to_runner(&msg);

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_ENABLE_ERROR_REPORTING;
#endif

    criterion_current_test = &test;
    criterion_current_suite = &suite;

    if (test.test)
        test.test();

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_DISABLE_ERROR_REPORTING;
#endif

    cri_proto_close(g_client_socket);

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_ENABLE_ERROR_REPORTING;
#endif
    return 0;
}

static void death_callback(bxf_instance *instance)
{
    int result = instance->status.signal
            ? criterion_protocol_death_result_type_CRASH
            : criterion_protocol_death_result_type_NORMAL;
    int code = instance->status.signal
            ? instance->status.signal
            : instance->status.exit;

    if (instance->status.timed_out) {
        criterion_protocol_msg msg = criterion_message(phase,
                        .phase = criterion_protocol_phase_kind_TIMEOUT,
                        );

        msg.id.pid = instance->pid;
        cr_send_to_runner(&msg);

        result = criterion_protocol_death_result_type_NORMAL;
        code = 0;
    }

    criterion_protocol_msg msg = criterion_message(death,
                    .result = result,
                    .has_status = true,
                    .status = code,
                    );

    msg.id.pid = instance->pid;
    cr_send_to_runner(&msg);
}

static bxf_instance *run_test(struct run_next_context *ctx,
        struct client_ctx *client)
{
    bxf_context inst_ctx;
    int rc = bxf_context_init(&inst_ctx);

    rc = serialize_test(inst_ctx, ctx->test, &ctx->suite_set->suite);
    if (!rc) {
        size_t len = strlen(ctx->url) + 1;
        rc = bxf_context_addobject(inst_ctx, "criterion.url", ctx->url, len);
    }

    if (!rc && ctx->params.params) {
        void *param = (char *) ctx->params.params + ctx->i * ctx->params.size;
        rc = bxf_context_addobject(inst_ctx, "criterion.param",
                        param, ctx->params.size);
    }

    if (!rc)
        rc = bxf_context_addstatic(inst_ctx, &criterion_options,
                        sizeof (criterion_options));

    if (!rc)
        rc = bxf_context_addarena(inst_ctx, cri_alloc_getarena());

    if (rc < 0)
        cr_panic("Could not initialize test context: %s", strerror(-rc));

    struct bxf_spawn_params_s sp = {
        .fn              = run_test_child,
        .callback        = death_callback,
        .inherit.context = inst_ctx,
    };

    if (criterion_options.debug == CR_DBG_IDLE) {
        sp.suspended = 1;
    } else if (criterion_options.debug) {
        enum bxf_debugger debugger = BXF_DBG_NONE;
        if (criterion_options.debug == CR_DBG_NATIVE) {
            switch (ctx->test->data->compiler_) {
                case CR_COMP_GCC:   debugger = BXF_DBG_GDB; break;
                case CR_COMP_CLANG: debugger = BXF_DBG_LLDB; break;
                case CR_COMP_MSVC:  debugger = BXF_DBG_WINDBG; break;
                default: break;
            }
        } else {
            switch (criterion_options.debug) {
                case CR_DBG_GDB:    debugger = BXF_DBG_GDB; break;
                case CR_DBG_LLDB:   debugger = BXF_DBG_LLDB; break;
                case CR_DBG_WINDBG: debugger = BXF_DBG_WINDBG; break;
                default: break;
            }
        }
        if (!debugger)
            cr_panic("Could not choose the debugger server for an "
                    "unknown compiler");
        sp.debug.debugger = debugger;
        sp.debug.tcp = criterion_options.debug_port;
    }

    double timeout = 0;
    if (ctx->suite_set->suite.data && ctx->suite_set->suite.data->timeout > 0)
        timeout = ctx->suite_set->suite.data->timeout;
    if (ctx->test->data->timeout > 0)
        timeout = ctx->test->data->timeout;
    if (criterion_options.timeout > 0 && timeout > criterion_options.timeout)
        timeout = criterion_options.timeout;

    sp.iquotas.runtime = timeout;

    bxf_instance *instance;
    rc = bxf_spawn_struct(&instance, &sp);
    if (rc < 0) {
        if (rc == -ENOENT && criterion_options.debug) {
            const char *dbgname = "<unknown>";
            switch (sp.debug.debugger) {
                case BXF_DBG_GDB:    dbgname = "gdbserver"; break;
                case BXF_DBG_LLDB:   dbgname = "lldb-server"; break;
                case BXF_DBG_WINDBG: dbgname = "windbg"; break;
                default: break;
            }
            cr_panic("Could not spawn test instance: Debugger does not exist.\n\n"
                    "Please make sure that `%s` is installed on your system and\n"
                    "available in your PATH, or manually specify another\n"
                    "debugging mode with --debug=mode.",
                    dbgname);
        }
        cr_panic("Could not spawn test instance: %s", strerror(-rc));
    }

    bxf_context_term(inst_ctx);

    /* TODO: integrate this to the logger after refactor */
    if (criterion_options.debug == CR_DBG_IDLE) {
        criterion_pinfo(CRITERION_PREFIX_DEBUG, _(msg_print_pid),
                ctx->test->category,
                ctx->test->name,
                (unsigned long) instance->pid);
    }

    *client = (struct client_ctx) {
        .test = ctx->test,
        .suite = &ctx->suite_set->suite,
        .gstats = ctx->stats,
        .sstats = sref(ctx->suite_stats),
        .tstats = test_stats_init(ctx->test),
    };

    return instance;
}

static CR_INLINE bool is_disabled(struct criterion_test *t,
        struct criterion_suite *s)
{
    return t->data->disabled || (s->data && s->data->disabled);
}

static int skip_disabled(struct run_next_context *ctx)
{
    if (is_disabled(ctx->test, ctx->suite_stats->suite)) {
        ctx->test_stats = test_stats_init(ctx->test);
        ctx->test_stats->test_status = CR_STATUS_SKIPPED;
        stat_push_event(ctx->stats,
                ctx->suite_stats,
                ctx->test_stats,
                &(struct event) { .kind = PRE_INIT });
        log(pre_init, ctx->suite_stats->suite, ctx->test);
        sfree(ctx->test_stats);
        return 1;
    }
    return 0;
}

bxf_instance *cri_run_next_test(struct criterion_test_set *p_set,
        struct criterion_global_stats *p_stats,
        const char *url,
        struct client_ctx *client,
        ccrContParam)
{
    ccrUseNamedContext(run_next_context, ctx);

    ccrBegin(ctx);

    do {
        ctx->set = p_set;
        ctx->stats = p_stats;
        ctx->url = url;
        memset(&ctx->params, 0, sizeof (ctx->params));
        ccrReturn(NULL);
    } while (ctx->set == NULL && ctx->stats == NULL);

    for (ctx->ns = ctx->set->suites->first; ctx->ns; ctx->ns = ctx->ns->next) {
        ctx->suite_set = (void *) (ctx->ns + 1);

        if (!ctx->suite_set->tests)
            continue;

        report(PRE_SUITE, ctx->suite_set);
        log(pre_suite, ctx->suite_set);

        ctx->suite_stats = suite_stats_init(&ctx->suite_set->suite);

        stat_push_event(ctx->stats, ctx->suite_stats, NULL, &(struct event) { .kind = PRE_SUITE });

        for (ctx->nt = ctx->suite_set->tests->first; ctx->nt; ctx->nt = ctx->nt->next) {
            ctx->test = (void *) (ctx->nt + 1);

            if (skip_disabled(ctx))
                continue;

            if (ctx->test->data->kind_ == CR_TEST_PARAMETERIZED
                    && ctx->test->data->param_) {
                ctx->params = ctx->test->data->param_();
                for (ctx->i = 0; ctx->i < ctx->params.length; ++ctx->i)
                    ccrReturn(run_test(ctx, client));

                if (ctx->params.cleanup)
                    ctx->params.cleanup(&ctx->params);
                ctx->params.params = NULL;
            } else {
                ccrReturn(run_test(ctx, client));
            }
        }

        report(POST_SUITE, ctx->suite_stats);

        sfree(ctx->suite_stats);
    }

    ccrFinish(NULL);
}
