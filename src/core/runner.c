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
#include <stdlib.h>
#include <stdio.h>
#include <csptr/smalloc.h>
#include "criterion/criterion.h"
#include "criterion/options.h"
#include "criterion/ordered-set.h"
#include "criterion/logging.h"
#include "compat/time.h"
#include "compat/posix.h"
#include "string/i18n.h"
#include "io/event.h"
#include "stats.h"
#include "runner.h"
#include "report.h"
#include "worker.h"
#include "abort.h"
#include "config.h"
#include "common.h"

#ifdef HAVE_PCRE
#include "string/extmatch.h"
#endif

#ifdef _MSC_VER
struct criterion_test  *SECTION_START_(cr_tst);
struct criterion_suite *SECTION_START_(cr_sts);
struct criterion_test  *SECTION_END_(cr_tst);
struct criterion_suite *SECTION_END_(cr_sts);
#endif

IMPL_SECTION_LIMITS(struct criterion_test*, cr_tst);
IMPL_SECTION_LIMITS(struct criterion_suite*, cr_sts);

// This is here to make the test suite & test sections non-empty
TestSuite();
Test(,) {};

static INLINE void nothing(void) {}
static INLINE void nothing_ptr(UNUSED void* ptr) {}

int cmp_suite(void *a, void *b) {
    struct criterion_suite *s1 = a, *s2 = b;
    return strcmp(s1->name, s2->name);
}

int cmp_test(void *a, void *b) {
    struct criterion_test *s1 = a, *s2 = b;
    return strcmp(s1->name, s2->name);
}

static void dtor_suite_set(void *ptr, UNUSED void *meta) {
    struct criterion_suite_set *s = ptr;
    sfree(s->tests);
}

static void dtor_test_set(void *ptr, UNUSED void *meta) {
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
        if (!*s)
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

        if (!(*test)->category)
            continue;

        criterion_register_test(set, *test);
    }

    return set;
}

typedef void (*f_test_run)(struct criterion_global_stats *,
        struct criterion_suite_stats *,
        struct criterion_test *,
        struct criterion_suite *);

static void map_tests(struct criterion_test_set *set,
                      struct criterion_global_stats *stats,
                      f_test_run fun) {

    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        if (!s->tests)
            continue;

        report(PRE_SUITE, s);
        log(pre_suite, s);

        struct criterion_suite_stats *suite_stats = suite_stats_init(&s->suite);

        struct event ev = { .kind = PRE_SUITE };
        stat_push_event(stats, suite_stats, NULL, &ev);

        FOREACH_SET(struct criterion_test *t, s->tests) {
            fun(stats, suite_stats, t, &s->suite);
            if (criterion_options.fail_fast && stats->tests_failed > 0)
                break;
            if (!is_runner()) {
                sfree(suite_stats);
                return;
            }
        }

        report(POST_SUITE, suite_stats);
        log(post_suite, suite_stats);

        sfree(suite_stats);
    }

}

static void run_test_child(struct criterion_test *test,
                           struct criterion_suite *suite) {

    if (suite->data && suite->data->timeout != 0 && test->data->timeout == 0)
        setup_timeout((uint64_t) (suite->data->timeout * 1e9));
    else if (test->data->timeout != 0)
        setup_timeout((uint64_t) (test->data->timeout * 1e9));

    send_event(PRE_INIT, NULL, 0);
    if (suite->data)
        (suite->data->init ? suite->data->init : nothing)();
    (test->data->init ? test->data->init : nothing)();
    send_event(PRE_TEST, NULL, 0);

    struct timespec_compat ts;
    if (!setjmp(g_pre_test)) {
        timer_start(&ts);
        if (!test->data->param_)
            (test->test ? test->test : nothing)();
        else
            (test->test ? (void(*)(void*)) test->test
                        : nothing_ptr)(g_worker_context.param->ptr);
    }

    double elapsed_time;
    if (!timer_end(&elapsed_time, &ts))
        elapsed_time = -1;

    send_event(POST_TEST, &elapsed_time, sizeof (double));
    (test->data->fini ? test->data->fini : nothing)();
    if (suite->data)
        (suite->data->fini ? suite->data->fini : nothing)();
    send_event(POST_FINI, NULL, 0);
}

static INLINE bool is_disabled(struct criterion_test *t,
                               struct criterion_suite *s) {

    return t->data->disabled || (s->data && s->data->disabled);
}

#define push_event(Kind, ...)                                       \
    do {                                                            \
        stat_push_event(ctx->stats,                                 \
                ctx->suite_stats,                                   \
                ctx->test_stats,                                    \
                &(struct event) { .kind = Kind, __VA_ARGS__ });     \
        report(Kind, ctx->test_stats);                              \
    } while (0)

s_pipe_handle *g_worker_pipe;

struct execution_context {
    bool test_started;
    bool normal_finish;
    bool cleaned_up;
    struct criterion_global_stats *stats;
    struct criterion_test *test;
    struct criterion_test_stats *test_stats;
    struct criterion_suite *suite;
    struct criterion_suite_stats *suite_stats;
};

static void handle_worker_terminated(struct event *ev,
        struct execution_context *ctx) {

    struct worker_status *ws = ev->data;
    struct process_status status = ws->status;

    if (status.kind == SIGNAL) {
        if (status.status == SIGPROF) {
            ctx->test_stats->timed_out = true;
            double elapsed_time = ctx->test->data->timeout;
            if (elapsed_time == 0 && ctx->suite->data)
                elapsed_time = ctx->suite->data->timeout;
            push_event(POST_TEST, .data = &elapsed_time);
            push_event(POST_FINI);
            log(test_timeout, ctx->test_stats);
            return;
        }

        if (ctx->normal_finish || !ctx->test_started) {
            log(other_crash, ctx->test_stats);
            if (!ctx->test_started) {
                stat_push_event(ctx->stats,
                        ctx->suite_stats,
                        ctx->test_stats,
                        &(struct event) { .kind = TEST_CRASH });
            }
            return;
        }
        ctx->test_stats->signal = status.status;
        if (ctx->test->data->signal == 0) {
            push_event(TEST_CRASH);
            log(test_crash, ctx->test_stats);
        } else {
            double elapsed_time = 0;
            push_event(POST_TEST, .data = &elapsed_time);
            log(post_test, ctx->test_stats);
            push_event(POST_FINI);
            log(post_fini, ctx->test_stats);
        }
    } else {
        if ((ctx->normal_finish && !ctx->cleaned_up) || !ctx->test_started) {
            log(abnormal_exit, ctx->test_stats);
            if (!ctx->test_started) {
                stat_push_event(ctx->stats,
                        ctx->suite_stats,
                        ctx->test_stats,
                        &(struct event) { .kind = TEST_CRASH });
            }
            return;
        }
        ctx->test_stats->exit_code = status.status;
        if (!ctx->normal_finish) {
            if (ctx->test->data->exit_code == 0) {
                push_event(TEST_CRASH);
                log(abnormal_exit, ctx->test_stats);
            } else {
                double elapsed_time = 0;
                push_event(POST_TEST, .data = &elapsed_time);
                log(post_test, ctx->test_stats);
                push_event(POST_FINI);
                log(post_fini, ctx->test_stats);
            }
        }
    }
}

static void run_test(struct criterion_global_stats *stats,
        struct criterion_suite_stats *suite_stats,
        struct criterion_test *test,
        struct criterion_suite *suite,
        struct test_single_param *param) {

    struct criterion_test_stats *test_stats = test_stats_init(test);
    struct process *proc = NULL;

    if (is_disabled(test, suite)) {
        stat_push_event(stats,
                suite_stats,
                test_stats,
                &(struct event) { .kind = PRE_INIT });
        goto cleanup;
    }

    proc = spawn_test_worker(test, suite, run_test_child, g_worker_pipe, param);
    if (proc == NULL && !is_runner())
        goto cleanup;

    struct execution_context ctx = {
        .stats = stats,
        .test = test,
        .test_stats = test_stats,
        .suite = suite,
        .suite_stats = suite_stats,
    };
    struct event *ev;
    while ((ev = worker_read_event(proc)) != NULL) {
        if (ev->kind < WORKER_TERMINATED)
            stat_push_event(stats, suite_stats, test_stats, ev);
        switch (ev->kind) {
            case PRE_INIT:
                report(PRE_INIT, test);
                log(pre_init, test);
                break;
            case PRE_TEST:
                report(PRE_TEST, test);
                log(pre_test, test);
                ctx.test_started = true;
                break;
            case THEORY_FAIL: {
                struct criterion_theory_stats ths = {
                    .formatted_args = (char*) ev->data,
                    .stats = test_stats,
                };
                report(THEORY_FAIL, &ths);
                log(theory_fail, &ths);
            } break;
            case ASSERT:
                report(ASSERT, ev->data);
                log(assert, ev->data);
                break;
            case POST_TEST:
                report(POST_TEST, test_stats);
                log(post_test, test_stats);
                ctx.normal_finish = true;
                break;
            case POST_FINI:
                report(POST_FINI, test_stats);
                log(post_fini, test_stats);
                ctx.cleaned_up = true;
                break;
            case WORKER_TERMINATED:
                handle_worker_terminated(ev, &ctx);
                sfree(ev);
                goto cleanup;
        }
        sfree(ev);
    }

cleanup:
    sfree(test_stats);
    sfree(proc);
}

static void run_test_param(struct criterion_global_stats *stats,
        struct criterion_suite_stats *suite_stats,
        struct criterion_test *test,
        struct criterion_suite *suite) {

    if (!test->data->param_)
        return;

    struct criterion_test_params params = test->data->param_();
    for (size_t i = 0; i < params.length; ++i) {
        struct test_single_param param = { params.size, (char *) params.params + i * params.size };

        run_test(stats, suite_stats, test, suite, &param);
        if (criterion_options.fail_fast && stats->tests_failed > 0)
            break;
        if (!is_runner())
            break;
    }
}

static void run_test_switch(struct criterion_global_stats *stats,
        struct criterion_suite_stats *suite_stats,
        struct criterion_test *test,
        struct criterion_suite *suite) {

    switch (test->data->kind_) {
        case CR_TEST_NORMAL:
            run_test(stats, suite_stats, test, suite, NULL);
            break;
        case CR_TEST_PARAMETERIZED:
            run_test_param(stats, suite_stats, test, suite);
            break;
        default: break;
    }
}

#ifdef HAVE_PCRE
void disable_unmatching(struct criterion_test_set *set) {
    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        if ((s->suite.data && s->suite.data->disabled) || !s->tests)
            continue;

        FOREACH_SET(struct criterion_test *test, s->tests) {
            const char *errmsg;
            int ret = extmatch(criterion_options.pattern, test->data->identifier_, &errmsg);
            if (ret == -10) {
                printf("pattern error: %s\n", errmsg);
                exit(1);
            } else if (ret < 0) {
                test->data->disabled = true;
            }
        }
    }
}
#endif

struct criterion_test_set *criterion_initialize(void) {
    init_i18n();

    if (resume_child()) // (windows only) resume from the fork
        exit(0);

    return criterion_init();
}

void criterion_finalize(struct criterion_test_set *set) {
    sfree(set);
}

static int criterion_run_all_tests_impl(struct criterion_test_set *set) {
    report(PRE_ALL, set);
    log(pre_all, set);

    fflush(NULL); // flush everything before forking

    g_worker_pipe = stdpipe();
    if (g_worker_pipe == NULL)
        abort();

    struct criterion_global_stats *stats = stats_init();
    map_tests(set, stats, run_test_switch);

    int result = is_runner() ? stats->tests_failed == 0 : -1;

    if (!is_runner())
        goto cleanup;

    report(POST_ALL, stats);
    log(post_all, stats);

cleanup:
    sfree(g_worker_pipe);
    sfree(stats);
    return result;
}

int criterion_run_all_tests(struct criterion_test_set *set) {
    #ifdef HAVE_PCRE
    if (criterion_options.pattern)
        disable_unmatching(set);
    #endif

    set_runner_process();
    int res = criterion_run_all_tests_impl(set);
    unset_runner_process();

    return criterion_options.always_succeed || res;
}
