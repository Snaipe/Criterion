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
#include <csptr/smart_ptr.h>
#include "criterion/common.h"
#include "stats.h"

#include <assert.h>

static void nothing() {};
static void push_pre_test();
static void push_assert();
static void push_post_test();
static void push_test_crash();

typedef struct criterion_global_stats s_glob_stats;
typedef struct criterion_test_stats   s_test_stats;
typedef struct criterion_assert_stats s_assert_stats;

static void destroy_stats(void *ptr, UNUSED void *meta) {
    s_glob_stats *stats = ptr;
    sfree(stats->tests);
}

s_glob_stats *stats_init(void) {
    return unique_ptr(s_glob_stats, ({0}), destroy_stats);
}

static void destroy_test_stats(void *ptr, UNUSED void *meta) {
    s_test_stats *stats = ptr;
    sfree(stats->asserts);
    sfree(stats->next);
}

s_test_stats *test_stats_init(struct criterion_test *t) {
    return shared_ptr(s_test_stats, ({
                .test = t,
                .progress = t->data->line_,
                .file = t->data->file_
            }), destroy_test_stats);
}

void stat_push_event(s_glob_stats *stats,
                     s_test_stats *test,
                     struct event *data) {
    static void (*const handles[])(s_glob_stats *, s_test_stats *, void *) = {
        nothing,            // PRE_EVERYTHING
        nothing,            // PRE_INIT
        push_pre_test,      // PRE_TEST
        push_assert,        // ASSERT
        push_test_crash,    // TEST_CRASH
        push_post_test,     // POST_TEST
        nothing,            // POST_FINI
        nothing,            // POST_EVERYTHING
    };

    assert(data->kind > 0);
    assert(data->kind <= (ssize_t) (sizeof (handles) / sizeof (void (*)(void))));

    handles[data->kind](stats, test, data->data);
}

static void push_pre_test(s_glob_stats *stats,
                          s_test_stats *test,
                          UNUSED void *ptr) {
    test->next = stats->tests;
    stats->tests = sref(test);
    ++stats->nb_tests;
}

static void destroy_assert(void *ptr, UNUSED void *meta) {
    s_assert_stats *data = ptr;
    sfree(data->next);
}

static void push_assert(s_glob_stats *stats,
                        s_test_stats *test,
                        s_assert_stats *data) {
    s_assert_stats *dup = unique_ptr(s_assert_stats, (*data), destroy_assert);
    dup->next = test->asserts;
    test->asserts = dup;

    if (data->passed) {
        ++stats->asserts_passed;
        ++test->passed;
    } else {
        ++stats->asserts_failed;
        ++test->failed;
    }

    test->progress = dup->line;
    test->file = dup->file;
}

static void push_post_test(s_glob_stats *stats,
                           s_test_stats *test,
                           UNUSED void *ptr) {
    if (test->failed > 0)
        ++stats->tests_failed;
    else
        ++stats->tests_passed;
}

static void push_test_crash(s_glob_stats *stats,
                            s_test_stats *test,
                            UNUSED void *ptr) {
    ++stats->tests_failed;
    ++stats->tests_crashed;
}
