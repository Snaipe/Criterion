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
#include <csptr/smart_ptr.h>
#include "criterion/criterion.h"
#include "criterion/options.h"
#include "criterion/ordered-set.h"
#include "stats.h"
#include "runner.h"
#include "report.h"
#include "event.h"
#include "process.h"
#include "timer.h"
#include "posix-compat.h"
#include "abort.h"

IMPL_SECTION_LIMITS(struct criterion_test, cr_tst);
IMPL_SECTION_LIMITS(struct criterion_suite, cr_sts);

// This is here to make the test suite & test sections non-empty
TestSuite();
Test(,) {};

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

struct criterion_test_set *criterion_init(void) {
    struct criterion_ordered_set *suites = new_ordered_set(cmp_suite, dtor_suite_set);

    FOREACH_SUITE_SEC(s) {
        if (!s->name)
            break;

        struct criterion_suite_set css = {
            .suite = *s,
        };
        insert_ordered_set(suites, &css, sizeof (css));
    }

    size_t nb_tests = 0;
    FOREACH_TEST_SEC(test) {
        if (!test->category)
            break;

        if (!*test->category)
            continue;

        struct criterion_suite_set css = {
            .suite = { .name = test->category },
        };
        struct criterion_suite_set *s = insert_ordered_set(suites, &css, sizeof (css));
        if (!s->tests)
            s->tests = new_ordered_set(cmp_test, NULL);

        insert_ordered_set(s->tests, test, sizeof(*test));
        ++nb_tests;
    }

    return unique_ptr(struct criterion_test_set, {
        suites,
        nb_tests,
    }, dtor_test_set);
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

        smart struct criterion_suite_stats *suite_stats = suite_stats_init(&s->suite);

        struct event ev = { .kind = PRE_SUITE };
        stat_push_event(stats, suite_stats, NULL, &ev);

        FOREACH_SET(struct criterion_test *t, s->tests) {
            fun(stats, suite_stats, t, &s->suite);
            if (criterion_options.fail_fast && stats->tests_failed > 0)
                break;
            if (!is_runner())
                return;
        }

        report(POST_SUITE, suite_stats);
    }
}

__attribute__ ((always_inline))
static inline void nothing() {}

static void run_test_child(struct criterion_test *test,
                           struct criterion_suite *suite) {

    send_event(PRE_INIT, NULL, 0);
    if (suite->data)
        (suite->data->init ?: nothing)();
    (test->data->init ?: nothing)();
    send_event(PRE_TEST, NULL, 0);

    struct timespec_compat ts;
    if (setup_abort_test()) {
        timer_start(&ts);
        (test->test ?: nothing)();
    }

    double elapsed_time;
    if (!timer_end(&elapsed_time, &ts))
        elapsed_time = -1;

    send_event(POST_TEST, &elapsed_time, sizeof (double));
    (test->data->fini ?: nothing)();
    if (suite->data)
        (suite->data->fini ?: nothing)();
    send_event(POST_FINI, NULL, 0);
}

__attribute__((always_inline))
static inline bool is_disabled(struct criterion_test *t,
                               struct criterion_suite *s) {

    return t->data->disabled || (s->data && s->data->disabled);
}

#define push_event(Kind, ...)                                       \
    do {                                                            \
        stat_push_event(stats,                                      \
                suite_stats,                                        \
                test_stats,                                         \
                &(struct event) { .kind = Kind, __VA_ARGS__ });     \
        report(Kind, test_stats);                                   \
    } while (0)

static void run_test(struct criterion_global_stats *stats,
        struct criterion_suite_stats *suite_stats,
        struct criterion_test *test,
        struct criterion_suite *suite) {

    smart struct criterion_test_stats *test_stats = test_stats_init(test);

    if (is_disabled(test, suite)) {
        stat_push_event(stats,
                suite_stats,
                test_stats,
                &(struct event) { .kind = PRE_INIT });
        return;
    }

    smart struct process *proc = spawn_test_worker(test, suite, run_test_child);
    if (proc == NULL && !is_runner())
        return;

    bool test_started  = false;
    bool normal_finish = false;
    struct event *ev;
    while ((ev = worker_read_event(proc)) != NULL) {
        stat_push_event(stats, suite_stats, test_stats, ev);
        switch (ev->kind) {
            case PRE_INIT:  report(PRE_INIT, test); break;
            case PRE_TEST:  report(PRE_TEST, test);
                            test_started = true;
                            break;
            case ASSERT:    report(ASSERT, ev->data); break;
            case POST_TEST: report(POST_TEST, test_stats);
                            normal_finish = true;
                            break;
            case POST_FINI: report(POST_FINI, test_stats); break;
        }
        sfree(ev);
    }

    struct process_status status = wait_proc(proc);
    if (status.kind == SIGNAL) {
        if (normal_finish || !test_started) {
            log(other_crash, test_stats);
            if (!test_started) {
                stat_push_event(stats,
                        suite_stats,
                        test_stats,
                        &(struct event) { .kind = TEST_CRASH });
            }
            return;
        }
        test_stats->signal = status.status;
        if (test->data->signal == 0) {
            push_event(TEST_CRASH);
        } else {
            double elapsed_time = 0;
            push_event(POST_TEST, .data = &elapsed_time);
            push_event(POST_FINI);
        }
    }
}

static int criterion_run_all_tests_impl(void) {
    if (resume_child()) // (windows only) resume from the fork
        return -1;

    smart struct criterion_test_set *set = criterion_init();

    report(PRE_ALL, set);

    smart struct criterion_global_stats *stats = stats_init();
    map_tests(set, stats, run_test);

    if (!is_runner())
        return -1;

    report(POST_ALL, stats);
    return stats->tests_failed == 0;
}

int criterion_run_all_tests(void) {
    set_runner_process();
    int res = criterion_run_all_tests_impl();
    unset_runner_process();

    if (res == -1) // if this is the test worker terminating
        exit(0);

    return criterion_options.always_succeed || res;
}
