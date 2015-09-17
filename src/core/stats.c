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
#include <string.h>
#include <csptr/smalloc.h>
#include "criterion/common.h"
#include "stats.h"
#include "common.h"

#include <assert.h>

typedef struct criterion_global_stats s_glob_stats;
typedef struct criterion_suite_stats  s_suite_stats;
typedef struct criterion_test_stats   s_test_stats;
typedef struct criterion_assert_stats s_assert_stats;

static void push_pre_suite(s_glob_stats *stats,
		           s_suite_stats *sstats,
		           s_test_stats *tstats,
		           void *data);
static void push_pre_init(s_glob_stats *stats,
		           s_suite_stats *sstats,
		           s_test_stats *tstats,
		           void *data);
static void push_assert(s_glob_stats *stats,
		           s_suite_stats *sstats,
		           s_test_stats *tstats,
		           void *data);
static void push_post_test(s_glob_stats *stats,
		           s_suite_stats *sstats,
		           s_test_stats *tstats,
		           void *data);
static void push_test_crash(s_glob_stats *stats,
		           s_suite_stats *sstats,
		           s_test_stats *tstats,
		           void *data);

static void nothing(UNUSED s_glob_stats *stats,
		    UNUSED s_suite_stats *sstats,
		    UNUSED s_test_stats *tstats,
		    UNUSED void *data) {
};

static void destroy_stats(void *ptr, UNUSED void *meta) {
    s_glob_stats *stats = ptr;
    for (s_suite_stats *s = stats->suites, *next; s; s = next) {
        next = s->next;
        sfree(s);
    }
}

s_glob_stats *stats_init(void) {
    s_glob_stats *stats = smalloc(
            .size = sizeof (s_glob_stats),
            .dtor = destroy_stats
        );
    *stats = (s_glob_stats) { .suites = NULL };
    return stats;
}

static void destroy_suite_stats(void *ptr, UNUSED void *meta) {
    s_suite_stats *stats = ptr;
    for (s_test_stats *t = stats->tests, *next; t; t = next) {
        next = t->next;
        sfree(t);
    }
}

s_suite_stats *suite_stats_init(struct criterion_suite *s) {
    s_suite_stats *stats = smalloc(
            .size = sizeof (s_suite_stats),
            .kind = SHARED,
            .dtor = destroy_suite_stats
        );
    *stats = (s_suite_stats) { .suite = s };
    return stats;
}

static void destroy_test_stats(void *ptr, UNUSED void *meta) {
    s_test_stats *stats = ptr;
    for (s_assert_stats *a = stats->asserts, *next; a; a = next) {
        next = a->next;
        sfree(a);
    }
}

s_test_stats *test_stats_init(struct criterion_test *t) {
    s_test_stats *stats = smalloc(
            .size = sizeof (s_test_stats),
            .kind = SHARED,
            .dtor = destroy_test_stats
        );
    *stats = (s_test_stats) {
            .test = t,
            .progress = t->data->line_,
            .file = t->data->file_
    };
    return stats;
}

typedef void (*f_handle)(s_glob_stats *, s_suite_stats *, s_test_stats *, void *);

void stat_push_event(s_glob_stats *stats,
                     s_suite_stats *suite,
                     s_test_stats *test,
                     struct event *data) {
    static const f_handle handles[] = {
        nothing,            // PRE_ALL
        push_pre_suite,     // PRE_SUITE
        push_pre_init,      // PRE_INIT
        nothing,            // PRE_TEST
        push_assert,        // ASSERT
        nothing,            // THEORY_FAIL
        push_test_crash,    // TEST_CRASH
        push_post_test,     // POST_TEST
        nothing,            // POST_FINI
        nothing,            // PRE_SUITE
        nothing,            // POST_ALL
    };

    assert(data->kind > 0);
    assert(data->kind <= (signed long long) (sizeof (handles) / sizeof (void (*)(void))));

    handles[data->kind](stats, suite, test, data->data);
}

static void push_pre_suite(s_glob_stats *stats,
                        s_suite_stats *suite,
                        UNUSED s_test_stats *test,
                        UNUSED void *ptr) {
    suite->next = stats->suites;
    stats->suites = sref(suite);
    ++stats->nb_suites;
}

static INLINE bool is_disabled(struct criterion_test *t,
                               struct criterion_suite *s) {

    return t->data->disabled || (s->data && s->data->disabled);
}

static void push_pre_init(s_glob_stats *stats,
                          s_suite_stats *suite,
                          s_test_stats *test,
                          UNUSED void *ptr) {
    test->next = suite->tests;
    suite->tests = sref(test);
    ++stats->nb_tests;
    ++suite->nb_tests;

    if (is_disabled(test->test, suite->suite)) {
        ++stats->tests_skipped;
        ++suite->tests_skipped;
    }
}

static void push_assert(s_glob_stats *stats,
                        s_suite_stats *suite,
                        s_test_stats *test,
                        void *ptr) {

    s_assert_stats *data = ptr;

    s_assert_stats *dup = smalloc(sizeof (s_assert_stats));
    memcpy(dup, data, sizeof (s_assert_stats));

    dup->next = test->asserts;
    test->asserts = dup;

    if (data->passed) {
        ++stats->asserts_passed;
        ++suite->asserts_passed;
        ++test->passed_asserts;
    } else {
        ++stats->asserts_failed;
        ++suite->asserts_failed;
        ++test->failed_asserts;
    }

    test->progress = dup->line;
    test->file = dup->file;
}

static void push_post_test(s_glob_stats *stats,
                           s_suite_stats *suite,
                           s_test_stats *test,
                           void *ptr) {
    double *data = ptr;

    test->elapsed_time = (float) *data;
    if (test->failed_asserts > 0
            || test->timed_out
            || test->signal != test->test->data->signal
            || test->exit_code != test->test->data->exit_code) {
        test->failed = 1;
    }

    if (test->failed) {
        ++stats->tests_failed;
        ++suite->tests_failed;
    } else {
        ++stats->tests_passed;
        ++suite->tests_passed;
    }

}

static void push_test_crash(s_glob_stats *stats,
                            s_suite_stats *suite,
                            s_test_stats *test,
                            UNUSED void *ptr) {
    test->failed = 1;
    ++suite->tests_failed;
    ++suite->tests_crashed;
    ++stats->tests_failed;
    ++stats->tests_crashed;
}
