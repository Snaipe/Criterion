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
#ifndef CRITERION_STATS_H_
# define CRITERION_STATS_H_

# include "types.h"

struct criterion_assert_stats {
    const char *message;
    bool passed;
    unsigned line;
    const char *file;

    struct criterion_assert_stats *next;
};

struct criterion_test_stats {
    struct criterion_test *test;
    struct criterion_assert_stats *asserts;
    bool failed;
    int passed_asserts;
    int failed_asserts;
    int signal;
    int exit_code;
    float elapsed_time;
    bool timed_out;
    bool crashed;
    unsigned progress;
    const char *file;

    struct criterion_test_stats *next;
};

struct criterion_theory_stats {
    const char *formatted_args;
    struct criterion_test_stats *stats;
};

struct criterion_suite_stats {
    struct criterion_suite *suite;
    struct criterion_test_stats *tests;
    size_t nb_tests;
    size_t nb_asserts;
    size_t tests_skipped;
    size_t tests_failed;
    size_t tests_crashed;
    size_t tests_passed;
    size_t asserts_failed;
    size_t asserts_passed;

    struct criterion_suite_stats *next;
};

struct criterion_global_stats {
    struct criterion_suite_stats *suites;
    size_t nb_suites;
    size_t nb_tests;
    size_t nb_asserts;
    size_t tests_skipped;
    size_t tests_failed;
    size_t tests_crashed;
    size_t tests_passed;
    size_t asserts_failed;
    size_t asserts_passed;
};

#endif /* !CRITERION_STATS_H_ */
