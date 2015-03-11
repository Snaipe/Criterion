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
#include "criterion/criterion.h"
#include "criterion/stats.h"
#include "criterion/logging.h"
#include "criterion/options.h"
#include "report.h"

#define IMPL_CALL_REPORT_HOOKS(Kind)                        \
    IMPL_SECTION_LIMITS(f_report_hook, Kind);               \
    void call_report_hooks_##Kind(void *data) {             \
        for (f_report_hook *hook = SECTION_START(Kind);     \
             hook < SECTION_END(Kind);                      \
             ++hook) {                                      \
            (*hook)(data);                                  \
        }                                                   \
    }

static size_t tap_test_index = 1;

IMPL_CALL_REPORT_HOOKS(PRE_ALL);
IMPL_CALL_REPORT_HOOKS(PRE_INIT);
IMPL_CALL_REPORT_HOOKS(PRE_TEST);
IMPL_CALL_REPORT_HOOKS(ASSERT);
IMPL_CALL_REPORT_HOOKS(TEST_CRASH);
IMPL_CALL_REPORT_HOOKS(POST_TEST);
IMPL_CALL_REPORT_HOOKS(POST_FINI);
IMPL_CALL_REPORT_HOOKS(POST_ALL);

ReportHook(PRE_INIT)(struct criterion_test *test) {
    if (criterion_options.enable_tap_format) return;

    criterion_info("%s::%s: RUNNING\n", test->category, test->name);
}

ReportHook(POST_TEST)(struct criterion_test_stats *stats) {
    if (criterion_options.enable_tap_format) {
        criterion_important("%s %lu - %s::%s\n",
                stats->failed ? "not ok" : "ok",
                tap_test_index++,
                stats->test->category,
                stats->test->name);
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
    } else {
        criterion_log(stats->failed ? CRITERION_IMPORTANT : CRITERION_INFO,
                "%s::%s: %s\n",
                stats->test->category,
                stats->test->name,
                stats->failed ? "FAILURE" : "SUCCESS");
    }
}

ReportHook(PRE_TEST)() {}
ReportHook(POST_FINI)() {}

ReportHook(PRE_ALL)(struct criterion_test_set *set) {
    if (criterion_options.enable_tap_format) {
        size_t enabled_count = 0, i = 0;
        for (struct criterion_test **test = set->tests; i < set->nb_tests; ++i)
            if (!(test[i])->data->disabled)
                ++enabled_count;
        criterion_important("1..%lu\n", enabled_count);
    }
}
ReportHook(POST_ALL)(struct criterion_global_stats *stats) {
    if (criterion_options.enable_tap_format) return;

    criterion_important("Synthesis: %lu tests were run. %lu passed, %lu failed (with %lu crashes)\n",
            stats->nb_tests,
            stats->tests_passed,
            stats->tests_failed,
            stats->tests_crashed);
}

ReportHook(ASSERT)(struct criterion_assert_stats *stats) {
    if (criterion_options.enable_tap_format) return;

    if (!stats->passed) {
        criterion_important("%s:%d: Assertion failed: %s\n",
                stats->file,
                stats->line,
                *stats->message ? stats->message : stats->condition);
    }
}

ReportHook(TEST_CRASH)(struct criterion_test_stats *stats) {
    if (criterion_options.enable_tap_format) {
        criterion_important("not ok %lu - %s::%s unexpected signal after %s:%u\n",
                tap_test_index++,
                stats->test->category,
                stats->test->name,
                stats->file,
                stats->progress);
    } else {
        criterion_important("Unexpected signal after %s:%u!\n%s::%s: FAILURE (CRASH!)\n",
                stats->file,
                stats->progress,
                stats->test->category,
                stats->test->name);
    }
}
