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
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <csptr/smart_ptr.h>
#include "runner.h"
#include "report.h"
#include "assert.h"

static struct criterion_test * const g_section_start = &__start_criterion_tests;
static struct criterion_test * const g_section_end   = &__stop_criterion_tests;

struct test_set {
    struct criterion_test **tests;
    size_t nb_tests;
};

static int compare_test_by_name(const struct criterion_test *first,
                                const struct criterion_test *second) {
    // likely to happen
    if (first->name == second->name)
        return 0;
    return strcmp(first->name, second->name);
}

static int compare_test(const void *a, const void *b) {
    struct criterion_test *first = *(struct criterion_test **) a;
    struct criterion_test *second = *(struct criterion_test **) b;

    // likely to happen
    if (first->category == second->category) {
        return compare_test_by_name(first, second);
    } else {
        return strcmp(first->category, second->category)
            ?: compare_test_by_name(first, second);
    }
}

static void destroy_test_set(void *ptr, UNUSED void *meta) {
    struct test_set *set = ptr;
    free(set->tests);
}

static struct test_set *read_all_tests(void) {
    size_t nb_tests = g_section_end - g_section_start;

    struct criterion_test **tests = malloc(nb_tests * sizeof (void *));
    if (tests == NULL)
        return NULL;

    size_t i = 0;
    for (struct criterion_test *test = g_section_start; test < g_section_end; ++test)
        tests[i++] = test;

    qsort(tests, nb_tests, sizeof (void *), compare_test);

    return unique_ptr(struct test_set, ({
                .tests = tests,
                .nb_tests = nb_tests
            }), destroy_test_set);
}

static void map_tests(struct test_set *set, void (*fun)(struct criterion_test *)) {
    size_t i = 0;
    for (struct criterion_test **t = set->tests; i < set->nb_tests; ++i, ++t)
        fun(*t);
}

__attribute__ ((always_inline))
static inline void nothing() {}

struct criterion_test_stats g_current_test_stats;

static void run_test_nofork(struct criterion_test *test) {
    report(PRE_INIT, test);
    (test->data->init ?: nothing)();
    report(PRE_TEST, test);

    g_current_test_stats = (struct criterion_test_stats) { .test = test };
    (test->test       ?: nothing)();

    report(POST_TEST, &g_current_test_stats);
    (test->data->fini ?: nothing)();
    report(POST_FINI, test);
}

static void run_test(struct criterion_test *test) {
    pid_t pid;
    if (!(pid = fork())) {
        run_test_nofork(test);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
    }
}

void run_all(void) {
    report(PRE_EVERYTHING, NULL);
    smart struct test_set *set = read_all_tests();
    if (!set)
        abort();
    map_tests(set, run_test);
    report(POST_EVERYTHING, NULL);
}

int main(void) {
    run_all();
}
