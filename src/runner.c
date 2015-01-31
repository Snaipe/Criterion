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
#include "criterion.h"
#include "runner.h"

struct criterion_test * const g_section_start = &__start_criterion_tests;
struct criterion_test * const g_section_end   = &__stop_criterion_tests;

static void map_tests(void (*fun)(struct criterion_test *)) {
    for (struct criterion_test *test = g_section_start; test < g_section_end; ++test) {
        fun(test);
    }
}

__attribute__ ((always_inline))
static inline void nothing(void) {}

static void run_test(struct criterion_test *test) {
    printf("%s::%s: ", test->category, test->name);
    fflush(stdout);
    pid_t pid;
    if (!(pid = fork())) {
        (test->data->init ?: nothing)();
        (test->test       ?: nothing)();
        (test->data->fini ?: nothing)();
        exit(0);
    } else {
        int status;
        waitpid(pid, &status, 0);
        int success = WIFEXITED(status) && WEXITSTATUS(status) == 0;
        printf("%s\n", success ? "SUCCESS" : "FAILURE");
    }
}

void run_all(void) {
    map_tests(run_test);
}

int main(void) {
    (criterion_init ?: nothing)();
    run_all();
    (criterion_fini ?: nothing)();
}
