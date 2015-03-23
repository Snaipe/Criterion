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

#define NORMALIZE(Str) (criterion_options.use_ascii ? "" : Str)

#define FG_BOLD  NORMALIZE("\e[0;1m")
#define FG_RED   NORMALIZE("\e[0;31m")
#define FG_GREEN NORMALIZE("\e[0;32m")
#define FG_GOLD  NORMALIZE("\e[0;33m")
#define FG_BLUE  NORMALIZE("\e[0;34m")
#define RESET    NORMALIZE("\e[0m")

void normal_log_pre_init(struct criterion_test *test) {
    criterion_info("[%sRUN%s ] %s::%s\n", FG_BLUE, RESET, test->category, test->name);
}

void normal_log_post_test(struct criterion_test_stats *stats) {
    const char *format = can_measure_time() ? "%s::%s: (%3.2fs)\n" : "%s::%s\n";
    const enum criterion_logging_level level = stats->failed ? CRITERION_IMPORTANT
                                                             : CRITERION_INFO;
    const char *color = stats->failed ? FG_RED : FG_GREEN;

    criterion_log(level, "[%s%s%s] ", color, stats->failed ? "FAIL" : "PASS", RESET);
    criterion_log(level, format,
            stats->test->category,
            stats->test->name,
            stats->elapsed_time);
}

void normal_log_post_all(struct criterion_global_stats *stats) {
    criterion_important("[%s====%s] ", FG_BLUE, RESET);
    criterion_important("%sSynthesis: " SIZE_T_FORMAT " test%s run. " SIZE_T_FORMAT " passed, " SIZE_T_FORMAT " failed (with " SIZE_T_FORMAT " crash%s)%s\n",
            FG_BOLD,
            stats->nb_tests,
            stats->nb_tests == 1 ? " was" : "s were",
            stats->tests_passed,
            stats->tests_failed,
            stats->tests_crashed,
            stats->tests_crashed == 1 ? "" : "es",
            RESET);
}

void normal_log_assert(struct criterion_assert_stats *stats) {
    if (!stats->passed) {
        criterion_important("[%s----%s] ", FG_BLUE, RESET);
        criterion_important("%s%s%s:%s%d%s: Assertion failed: %s\n",
                FG_BOLD,
                stats->file,
                RESET,
                FG_RED,
                stats->line,
                RESET,
                *stats->message ? stats->message : stats->condition);
    }
}

void normal_log_test_crash(struct criterion_test_stats *stats) {
    criterion_important("[%s----%s] ", FG_BLUE, RESET);
    criterion_important("%s%s%s:%s%u%s: Unexpected signal caught below this line!\n",
            FG_BOLD,
            stats->file,
            RESET,
            FG_RED,
            stats->progress,
            RESET);
    criterion_important("[%sFAIL%s] %s::%s: CRASH!\n",
            FG_RED,
            RESET,
            stats->test->category,
            stats->test->name);
}

void normal_log_pre_suite(struct criterion_suite_set *set) {
    criterion_info("[%s====%s] ", FG_BLUE, RESET);
    criterion_info("Running %s" SIZE_T_FORMAT "%s test%s from %s%s%s:\n",
            FG_BLUE,
            set->tests->size,
            RESET,
            set->tests->size == 1 ? "" : "s",
            FG_GOLD,
            set->suite.name,
            RESET);
}

struct criterion_output_provider normal_logging = {
    .log_pre_init = normal_log_pre_init,
    .log_pre_suite = normal_log_pre_suite,
    .log_post_test = normal_log_post_test,
    .log_assert = normal_log_assert,
    .log_post_all = normal_log_post_all,
    .log_test_crash = normal_log_test_crash,
};
