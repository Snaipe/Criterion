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
#include <unistd.h>
#include <csptr/smart_ptr.h>
#include "criterion/options.h"
#include "criterion/ordered-set.h"
#include "stats.h"
#include "runner.h"
#include "report.h"
#include "event.h"
#include "process.h"
#include "timer.h"

IMPL_SECTION_LIMITS(struct criterion_test, criterion_tests);
IMPL_SECTION_LIMITS(struct criterion_suite, crit_suites);

TestSuite(default);

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

static struct criterion_test_set *criterion_init(void) {
    struct criterion_ordered_set *suites = new_ordered_set(cmp_suite, dtor_suite_set);

    FOREACH_SUITE_SEC(s) {
        struct criterion_suite_set css = {
            .suite = *s,
        };
        insert_ordered_set(suites, &css, sizeof (css));
    }

    FOREACH_TEST_SEC(test) {
        struct criterion_suite_set css = {
            .suite = { .name = test->category },
        };
        struct criterion_suite_set *s = insert_ordered_set(suites, &css, sizeof (css));
        if (!s->tests)
            s->tests = new_ordered_set(cmp_test, NULL);

        insert_ordered_set(s->tests, test, sizeof(*test));
    }

    const size_t nb_tests = SECTION_END(criterion_tests)
                          - SECTION_START(criterion_tests);

    return unique_ptr(struct criterion_test_set, {
        suites,
        nb_tests,
    }, dtor_test_set);
}

typedef void (*f_test_run)(struct criterion_global_stats *, struct criterion_test *, struct criterion_suite *);

static void map_tests(struct criterion_test_set *set, struct criterion_global_stats *stats, f_test_run fun) {
    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        if ((s->suite.data && s->suite.data->disabled) || !s->tests)
            continue;

        FOREACH_SET(struct criterion_test *t, s->tests) {
            fun(stats, t, &s->suite);
            if (!is_runner())
                return;
        }
    }
}

__attribute__ ((always_inline))
static inline void nothing() {}

static void run_test_child(struct criterion_test *test, struct criterion_suite *suite) {
    send_event(PRE_INIT, NULL, 0);
    if (suite->data)
        (suite->data->init ?: nothing)();
    (test->data->init ?: nothing)();
    send_event(PRE_TEST, NULL, 0);

    struct timespec_compat ts;
    timer_start(&ts);
    (test->test       ?: nothing)();
    double elapsed_time;
    if (!timer_end(&elapsed_time, &ts))
        elapsed_time = -1;

    send_event(POST_TEST, &elapsed_time, sizeof (double));
    (test->data->fini ?: nothing)();
    if (suite->data)
        (suite->data->fini ?: nothing)();
    send_event(POST_FINI, NULL, 0);
}

static void run_test(struct criterion_global_stats *stats, struct criterion_test *test, struct criterion_suite *suite) {
    if (test->data->disabled)
        return;

    smart struct criterion_test_stats *test_stats = test_stats_init(test);

    smart struct process *proc = spawn_test_worker(test, suite, run_test_child);
    if (proc == NULL && !is_runner())
        return;

    struct event *ev;
    while ((ev = worker_read_event(proc)) != NULL) {
        stat_push_event(stats, test_stats, ev);
        switch (ev->kind) {
            case PRE_INIT:  report(PRE_INIT, test); break;
            case PRE_TEST:  report(PRE_TEST, test); break;
            case ASSERT:    report(ASSERT, ev->data); break;
            case POST_TEST: report(POST_TEST, test_stats); break;
            case POST_FINI: report(POST_FINI, test_stats); break;
        }
        sfree(ev);
    }

    struct process_status status = wait_proc(proc);
    if (status.kind == SIGNAL) {
        test_stats->signal = status.status;
        if (test->data->signal == 0) {
            struct event ev = { .kind = TEST_CRASH };
            stat_push_event(stats, test_stats, &ev);
            report(TEST_CRASH, test_stats);
        } else {
            double elapsed_time = 0;
            struct event ev = { .kind = POST_TEST, .data = &elapsed_time };
            stat_push_event(stats, test_stats, &ev);
            report(POST_TEST, test_stats);

            ev = (struct event) { .kind = POST_FINI, .data = NULL };
            stat_push_event(stats, test_stats, &ev);
            report(POST_FINI, test_stats);
        }
    }
}

static int criterion_run_all_tests_impl(void) {
    smart struct criterion_test_set *set = criterion_init();

    report(PRE_ALL, set);
    set_runner_pid();

    smart struct criterion_global_stats *stats = stats_init();
    map_tests(set, stats, run_test);

    if (!is_runner())
        return -1;

    report(POST_ALL, stats);
    return stats->tests_failed == 0;
}

int criterion_run_all_tests(void) {
    int res = criterion_run_all_tests_impl();
    if (res == -1) // if this is the test worker terminating
        _exit(0);

    return criterion_options.always_succeed || res;
}
