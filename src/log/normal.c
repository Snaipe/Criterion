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
#define _GNU_SOURCE
#define CRITERION_LOGGING_COLORS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "criterion/stats.h"
#include "criterion/logging.h"
#include "criterion/options.h"
#include "criterion/ordered-set.h"
#include "timer.h"
#include "config.h"
#include "i18n.h"

void normal_log_pre_all(UNUSED struct criterion_test_set *set) {
    criterion_pinfo(CRITERION_PREFIX_DASHES, _("Criterion v%s\n"), VERSION);
}

void normal_log_pre_init(struct criterion_test *test) {
    criterion_pinfo(CRITERION_PREFIX_RUN, _("%1$s::%2$s\n"),
            test->category,
            test->name);

    if (test->data->description)
        criterion_pinfo(CRITERION_PREFIX_RUN, _("  %s\n"),
                test->data->description);
}

void normal_log_post_test(struct criterion_test_stats *stats) {
    const char *format = can_measure_time() ? "%1$s::%2$s: (%3$3.2fs)\n"
                                            : "%1$s::%2$s\n";

    const enum criterion_logging_level level
            = stats->failed ? CRITERION_IMPORTANT : CRITERION_INFO;
    const struct criterion_prefix_data *prefix
            = stats->failed ? CRITERION_PREFIX_FAIL : CRITERION_PREFIX_PASS;

    criterion_plog(level, prefix, _(format),
            stats->test->category,
            stats->test->name,
            stats->elapsed_time);
}

__attribute__((always_inline))
static inline bool is_disabled(struct criterion_test *t,
                               struct criterion_suite *s) {
    return t->data->disabled || (s->data && s->data->disabled);
}

void normal_log_post_suite(struct criterion_suite_stats *stats) {
    for (struct criterion_test_stats *ts = stats->tests; ts; ts = ts->next) {
        if (is_disabled(ts->test, stats->suite)) {
            const char *format = ts->test->data->disabled
                    ? _("%1$s::%2$s: Test is disabled\n")
                    : _("%1$s::%2$s: Suite is disabled\n");

            criterion_pinfo(CRITERION_PREFIX_SKIP, format,
                    ts->test->category,
                    ts->test->name);

            if (ts->test->data->description)
                criterion_pinfo(CRITERION_PREFIX_DASHES, "  %s\n",
                        ts->test->data->description);
        }
    }
}

void normal_log_post_all(struct criterion_global_stats *stats) {
    criterion_pimportant(CRITERION_PREFIX_EQUALS,
            _("%1$sSynthesis: Tested: %2$s%3$lu%4$s "
              "| Passing: %5$s%6$lu%7$s "
              "| Failing: %8$s%9$lu%10$s "
              "| Crashing: %11$s%12$lu%13$s "
              "%14$s\n"),
            FG_BOLD,
            FG_BLUE,  (unsigned long) stats->nb_tests,      FG_BOLD,
            FG_GREEN, (unsigned long) stats->tests_passed,  FG_BOLD,
            FG_RED,   (unsigned long) stats->tests_failed,  FG_BOLD,
            FG_RED,   (unsigned long) stats->tests_crashed, FG_BOLD,
            RESET);
}

void normal_log_assert(struct criterion_assert_stats *stats) {
    if (!stats->passed) {
        char *dup       = strdup(*stats->message ? stats->message
                                                 : stats->condition);
        char *saveptr   = NULL;
        char *line      = strtok_r(dup, "\n", &saveptr);

        criterion_pimportant(CRITERION_PREFIX_DASHES,
                _("%1$s%2$s%3$s:%4$s%5$d%6$s: Assertion failed: %7$s\n"),
                FG_BOLD, stats->file, RESET,
                FG_RED,  stats->line, RESET,
                line);

        while ((line = strtok_r(NULL, "\n", &saveptr)))
            criterion_pimportant(CRITERION_PREFIX_DASHES, _("  %s\n"), line);
        free(dup);
    }
}

void normal_log_test_crash(struct criterion_test_stats *stats) {
    criterion_pimportant(CRITERION_PREFIX_DASHES,
            _("%1$s%2$s%3$s:%4$s%5$u%6$s: "
              "Unexpected signal caught below this line!\n"),
            FG_BOLD, stats->file,     RESET,
            FG_RED,  stats->progress, RESET);
    criterion_pimportant(CRITERION_PREFIX_FAIL, _("%1$s::%2$s: CRASH!\n"),
            stats->test->category,
            stats->test->name);
}

void normal_log_pre_suite(struct criterion_suite_set *set) {
    criterion_pinfo(CRITERION_PREFIX_EQUALS,
            _s("Running %1$s%2$lu%3$s test from %4$s%5$s%6$s:\n",
               "Running %1$s%2$lu%3$s tests from %4$s%5$s%6$s:\n",
               set->tests->size),
            FG_BLUE, (unsigned long) set->tests->size, RESET,
            FG_GOLD, set->suite.name,  RESET);
}

struct criterion_output_provider normal_logging = {
    .log_pre_all    = normal_log_pre_all,
    .log_pre_init   = normal_log_pre_init,
    .log_pre_suite  = normal_log_pre_suite,
    .log_assert     = normal_log_assert,
    .log_test_crash = normal_log_test_crash,
    .log_post_test  = normal_log_post_test,
    .log_post_suite = normal_log_post_suite,
    .log_post_all   = normal_log_post_all,
};
