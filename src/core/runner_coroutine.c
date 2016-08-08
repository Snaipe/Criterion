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
#include <valgrind/valgrind.h>

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
#include "client.h"
#include "err.h"
#include "report.h"
#include "runner.h"
#include "runner_coroutine.h"
#include "stats.h"

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

CR_API const struct criterion_test  *criterion_current_test;
CR_API const struct criterion_suite *criterion_current_suite;

static int run_test_child(void)
{

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_DISABLE_ERROR_REPORTING;
#endif

    struct criterion_test *test;
    struct criterion_suite *suite;
    const char *url;

    bxf_context ctx = bxf_context_current();

    int rc;

    rc = bxf_context_getobject(ctx, "criterion.test", (void **)&test);
    if (rc > 0)
        rc = bxf_context_getobject(ctx, "criterion.suite", (void **)&suite);
    if (rc > 0)
        rc = bxf_context_getobject(ctx, "criterion.url", (void **)&url);

    if (rc < 0)
        cr_panic("Could not get the test information: %s", strerror(-rc));
    else if (!rc)
        cr_panic("Could not initialize test context: property not found");

    cr_redirect_stdin();
    g_client_socket = cri_proto_connect(url);

    if (g_client_socket < 0)
        cr_panic("could not initialize the message client: %s", strerror(errno));

    // Notify the runner that the test was born
    criterion_protocol_msg msg = criterion_message(birth, .name = (char *) test->name);
    criterion_message_set_id(msg);
    cr_send_to_runner(&msg);

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_ENABLE_ERROR_REPORTING;
#endif

    criterion_current_test = test;
    criterion_current_suite = suite;

    if (test->test)
        test->test();

#ifndef ENABLE_VALGRIND_ERRORS
    VALGRIND_DISABLE_ERROR_REPORTING;
#endif

    cri_proto_close(g_client_socket);
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

    if (!rc)
        rc = bxf_context_addobject(inst_ctx, "criterion.test",
                ctx->test, sizeof (*ctx->test));
    if (!rc)
        rc = bxf_context_addobject(inst_ctx, "criterion.suite",
                &ctx->suite_set->suite, sizeof (ctx->suite_set->suite));

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
        rc = bxf_context_addarena(inst_ctx, cri_alloc_getarena());

    if (rc < 0)
        cr_panic("Could not initialize test context: %s", strerror(-rc));

    struct bxf_spawn_params sp = {
        .fn = run_test_child,
        .callback = death_callback,
        .inherit.context = inst_ctx,
    };

#if 0
    sp.debug.debugger = BXF_DBG_NATIVE;
    sp.debug.tcp = 1234;
#endif

    if (ctx->suite_set->suite.data && ctx->suite_set->suite.data->timeout != 0)
        sp.quotas.runtime = ctx->suite_set->suite.data->timeout;
    if (ctx->test->data->timeout != 0)
        sp.iquotas.runtime = ctx->test->data->timeout;

    bxf_instance *instance;
    rc = bxf_spawn_struct(&instance, &sp);
    if (rc < 0)
        cr_panic("Could not spawn test instance: %s", strerror(-rc));

    *client = (struct client_ctx) {
        .test = ctx->test,
        .suite = &ctx->suite_set->suite,
        .gstats = ctx->stats,
        .sstats = sref(ctx->suite_stats),
        .tstats = test_stats_init(ctx->test),
    };

    return instance;
}

static INLINE bool is_disabled(struct criterion_test *t,
        struct criterion_suite *s)
{

    return t->data->disabled || (s->data && s->data->disabled);
}


static int skip_disabled(struct run_next_context *ctx)
{
    if (is_disabled(ctx->test, ctx->suite_stats->suite)) {
        ctx->test_stats = test_stats_init(ctx->test);
        stat_push_event(ctx->stats,
                ctx->suite_stats,
                ctx->test_stats,
                &(struct event) { .kind = PRE_INIT });
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
        ctx->suite_set = (void*) (ctx->ns + 1);

        if (!ctx->suite_set->tests)
            continue;

        report(PRE_SUITE, ctx->suite_set);
        log(pre_suite, ctx->suite_set);

        ctx->suite_stats = suite_stats_init(&ctx->suite_set->suite);

        stat_push_event(ctx->stats, ctx->suite_stats, NULL, &(struct event) { .kind = PRE_SUITE });

        for (ctx->nt = ctx->suite_set->tests->first; ctx->nt; ctx->nt = ctx->nt->next) {
            ctx->test = (void*) (ctx->nt + 1);

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
        log(post_suite, ctx->suite_stats);

        sfree(ctx->suite_stats);
    }

    ccrFinish(NULL);
}
