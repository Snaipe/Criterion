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
#include "report.h"

#define IMPL_CALL_REPORT_HOOKS(Kind)                        \
    static f_report_hook * const g_##Kind##_section_start = \
        &__start_criterion_hooks_##Kind;                    \
    static f_report_hook * const g_##Kind##_section_end =   \
        &__stop_criterion_hooks_##Kind;                     \
    void call_report_hooks_##Kind(void *data) {             \
        for (f_report_hook *hook = g_##Kind##_section_start;\
             hook < g_##Kind##_section_end;                 \
             ++hook) {                                      \
            (*hook)(data);                                  \
        }                                                   \
    }

IMPL_CALL_REPORT_HOOKS(PRE_EVERYTHING);
IMPL_CALL_REPORT_HOOKS(PRE_INIT);
IMPL_CALL_REPORT_HOOKS(PRE_TEST);
IMPL_CALL_REPORT_HOOKS(ASSERT);
IMPL_CALL_REPORT_HOOKS(TEST_CRASH);
IMPL_CALL_REPORT_HOOKS(POST_TEST);
IMPL_CALL_REPORT_HOOKS(POST_FINI);
IMPL_CALL_REPORT_HOOKS(POST_EVERYTHING);

ReportHook(PRE_INIT)(struct criterion_test *test) {
    fprintf(stderr, "%s::%s: RUNNING\n", test->category, test->name);
}

ReportHook(POST_TEST)(struct criterion_test_stats *stats) {
    int success = stats->failed == 0;

    fprintf(stderr, "%s::%s: %s\n", stats->test->category, stats->test->name, success ? "SUCCESS" : "FAILURE");
}

ReportHook(PRE_TEST)() {}
ReportHook(POST_FINI)() {}

ReportHook(PRE_EVERYTHING)() {}
ReportHook(POST_EVERYTHING)(struct criterion_global_stats *stats) {
    fprintf(stderr, "Synthesis: %lu tests were run. %lu passed, %lu failed (with %lu crashes)\n", stats->nb_tests, stats->tests_passed, stats->tests_failed, stats->tests_crashed);
}

ReportHook(ASSERT)(struct criterion_assert_stats *stats) {
    if (!stats->passed) {
        fprintf(stderr, "\t%s:%d: Assertion failed: %s\n",
                stats->file,
                stats->line,
                *stats->message ? stats->message : stats->condition);
    }
}

ReportHook(TEST_CRASH)(struct criterion_test_stats *stats) {
    fprintf(stderr, "\tUnexpected signal after %s:%u!\n%s::%s: FAILURE (CRASH!)\n",
            stats->file,
            stats->progress,
            stats->test->category,
            stats->test->name);
}
