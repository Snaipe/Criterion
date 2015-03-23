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
#include <stdio.h>
#include "criterion/stats.h"
#include "criterion/logging.h"
#include "criterion/options.h"
#include "criterion/ordered-set.h"
#include "timer.h"

void normal_log_pre_init(struct criterion_test *test) {
    criterion_info("%s::%s: RUNNING\n", test->category, test->name);
}

void normal_log_post_test(struct criterion_test_stats *stats) {
    const char *format = can_measure_time() ? "%s::%s: %s (%3.2fs)\n" : "%s::%s: %s\n";
    criterion_log(stats->failed ? CRITERION_IMPORTANT : CRITERION_INFO,
            format,
            stats->test->category,
            stats->test->name,
            stats->failed ? "FAILURE" : "SUCCESS",
            stats->elapsed_time);
}

void normal_log_post_all(struct criterion_global_stats *stats) {
    criterion_important("Synthesis: " SIZE_T_FORMAT " test%s run. " SIZE_T_FORMAT " passed, " SIZE_T_FORMAT " failed (with " SIZE_T_FORMAT " crash%s)\n",
            stats->nb_tests,
            stats->nb_tests == 1 ? " was" : "s were",
            stats->tests_passed,
            stats->tests_failed,
            stats->tests_crashed,
            stats->tests_crashed == 1 ? "" : "es");
}

void normal_log_assert(struct criterion_assert_stats *stats) {
    if (!stats->passed) {
        criterion_important("%s:%d: Assertion failed: %s\n",
                stats->file,
                stats->line,
                *stats->message ? stats->message : stats->condition);
    }
}

void normal_log_test_crash(struct criterion_test_stats *stats) {
    criterion_important("Unexpected signal after %s:%u!\n%s::%s: FAILURE (CRASH!)\n",
            stats->file,
            stats->progress,
            stats->test->category,
            stats->test->name);
}

struct criterion_output_provider normal_logging = {
    .log_pre_init = normal_log_pre_init,
    .log_post_test = normal_log_post_test,
    .log_assert = normal_log_assert,
    .log_post_all = normal_log_post_all,
    .log_test_crash = normal_log_test_crash,
};
