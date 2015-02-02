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
#include "runner.h"
#include "report.h"
#include "assert.h"

static struct criterion_test * const g_section_start = &__start_criterion_tests;
static struct criterion_test * const g_section_end   = &__stop_criterion_tests;

static void map_tests(void (*fun)(struct criterion_test *)) {
    for (struct criterion_test *test = g_section_start; test < g_section_end; ++test) {
        fun(test);
    }
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
    map_tests(run_test);
    report(POST_EVERYTHING, NULL);
}

int main(void) {
    run_all();
}
