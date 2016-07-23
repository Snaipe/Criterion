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
#include "log/logging.h"
#include "runner_coroutine.h"
#include "worker.h"
#include "stats.h"
#include "runner.h"
#include "report.h"

static CR_INLINE void nothing(void) {}

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

ccrEndDefineContextType;

static struct worker *run_test(struct criterion_global_stats *stats,
        struct criterion_suite_stats *suite_stats,
        struct criterion_test_stats *test_stats,
        struct test_single_param *param) {

    struct execution_context ctx = {
        .stats = sref(stats),
        .test = test_stats->test,
        .test_stats = sref(test_stats),
        .suite = suite_stats->suite,
        .suite_stats = sref(suite_stats),
        .param = param,
    };
    return spawn_test_worker(&ctx, run_test_child);
}

static CR_INLINE bool is_disabled(struct criterion_test *t,
                               struct criterion_suite *s) {

    return t->data->disabled || (s->data && s->data->disabled);
}


static struct worker *cleanup_and_return_worker(struct run_next_context *ctx,
                                                struct worker *worker) {

    sfree(ctx->test_stats);
    if (!is_runner()) {
        worker = NULL;
        sfree(ctx->suite_stats);
        if (ctx->test->data->kind_ == CR_TEST_PARAMETERIZED
                && ctx->params.cleanup)
            ctx->params.cleanup(&ctx->params);
    }
    return worker;
}

static int skip_disabled(struct run_next_context *ctx) {
    if (is_disabled(ctx->test, ctx->suite_stats->suite)) {
        ctx->test_stats = test_stats_init(ctx->test);
        ctx->test_stats->test_status = CR_STATUS_SKIPPED;
        stat_push_event(ctx->stats,
                ctx->suite_stats,
                ctx->test_stats,
                &(struct event) { .kind = PRE_INIT });
        sfree(ctx->test_stats);
        return 1;
    }
    return 0;
}

struct worker *run_next_test(struct criterion_test_set *p_set,
                             struct criterion_global_stats *p_stats,
                             ccrContParam) {
    struct worker *worker = NULL;

    ccrUseNamedContext(run_next_context, ctx);

    ccrBegin(ctx);

    do {
        ctx->set = p_set;
        ctx->stats = p_stats;
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

            if (ctx->test->data->kind_ == CR_TEST_PARAMETERIZED
                    && ctx->test->data->param_) {

                if (skip_disabled(ctx))
                    continue;

                ctx->params = ctx->test->data->param_();
                for (ctx->i = 0; ctx->i < ctx->params.length; ++ctx->i) {
                    ctx->test_stats = test_stats_init(ctx->test);


                    worker = run_test(ctx->stats,
                            ctx->suite_stats,
                            ctx->test_stats,
                            &(struct test_single_param) {
                                ctx->params.size,
                                (char *) ctx->params.params + ctx->i * ctx->params.size
                            });

                    ccrReturn(cleanup_and_return_worker(ctx, worker));
                }

                if (ctx->params.cleanup)
                    ctx->params.cleanup(&ctx->params);
            } else {
                if (skip_disabled(ctx))
                    continue;

                ctx->test_stats = test_stats_init(ctx->test);

                worker = run_test(ctx->stats,
                        ctx->suite_stats,
                        ctx->test_stats,
                        NULL);

                ccrReturn(cleanup_and_return_worker(ctx, worker));
            }
        }

        report(POST_SUITE, ctx->suite_stats);
        log(post_suite, ctx->suite_stats);

        sfree(ctx->suite_stats);
    }

    ccrFinish(NULL);
}
