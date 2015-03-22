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

static size_t tap_test_index = 1;

void tap_log_pre_all(struct criterion_test_set *set) {
    size_t enabled_count = 0;
    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        if ((s->suite.data && s->suite.data->disabled) || !s->tests)
            continue;

        FOREACH_SET(struct criterion_test *test, s->tests) {
            if (!test->data->disabled)
                ++enabled_count;
        }
    }
    criterion_important("1.." SIZE_T_FORMAT "\n", enabled_count);
}

void tap_log_post_test(struct criterion_test_stats *stats) {
    const char *format = can_measure_time() ? "%s " SIZE_T_FORMAT " - %s::%s (%3.2fs)\n"
                                            : "%s " SIZE_T_FORMAT " - %s::%s\n";
    criterion_important(format,
            stats->failed ? "not ok" : "ok",
            tap_test_index++,
            stats->test->category,
            stats->test->name,
            stats->elapsed_time);
    for (struct criterion_assert_stats *asrt = stats->asserts; asrt; asrt = asrt->next) {
        if (!asrt->passed) {
            char *dup = strdup(*asrt->message ? asrt->message : asrt->condition), *saveptr = NULL;
            char *line = strtok_r(dup, "\n", &saveptr);
            criterion_important("\t%s:%u: Assertion failed: %s\n",
                    asrt->file,
                    asrt->line,
                    line);
            while ((line = strtok_r(NULL, "\n", &saveptr)))
                criterion_important("\t%s\n", line);
            free(dup);
        }
    }
}

void tap_log_test_crash(struct criterion_test_stats *stats) {
    criterion_important("not ok " SIZE_T_FORMAT " - %s::%s unexpected signal after %s:%u\n",
            tap_test_index++,
            stats->test->category,
            stats->test->name,
            stats->file,
            stats->progress);
}

struct criterion_output_provider tap_logging = {
    .log_pre_all    = tap_log_pre_all,
    .log_test_crash = tap_log_test_crash,
    .log_post_test  = tap_log_post_test,
};
