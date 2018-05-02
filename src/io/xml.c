/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "criterion/stats.h"
#include "criterion/options.h"
#include "criterion/internal/ordered-set.h"
#include "log/logging.h"
#include "compat/posix.h"
#include "compat/strtok.h"
#include "compat/time.h"
#include "config.h"
#include "common.h"

#define TESTSUITES_PROPERTIES            \
    "name=\"Criterion Tests\" "          \
    "tests=\"" CR_SIZE_T_FORMAT "\" "    \
    "failures=\"" CR_SIZE_T_FORMAT "\" " \
    "errors=\"" CR_SIZE_T_FORMAT "\" "   \
    "disabled=\"" CR_SIZE_T_FORMAT "\""

#define XML_BASE_TEMPLATE_BEGIN                             \
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"          \
    "<!-- Tests compiled with Criterion v" VERSION " -->\n" \
    "<testsuites " TESTSUITES_PROPERTIES ">\n"              \

#define XML_BASE_TEMPLATE_END \
    "</testsuites>\n"

#define TESTSUITE_PROPERTIES             \
    "name=\"%s\" "                       \
    "tests=\"" CR_SIZE_T_FORMAT "\" "    \
    "failures=\"" CR_SIZE_T_FORMAT "\" " \
    "errors=\"" CR_SIZE_T_FORMAT "\" "   \
    "disabled=\"" CR_SIZE_T_FORMAT "\" " \
    "skipped=\"" CR_SIZE_T_FORMAT "\" "  \
    "time=\"%.3f\""

#define XML_TESTSUITE_TEMPLATE_BEGIN \
    "  <testsuite " TESTSUITE_PROPERTIES ">\n"

#define XML_TESTSUITE_TEMPLATE_END \
    "  </testsuite>\n"

#define TEST_PROPERTIES                    \
    "name=\"%s\" "                         \
    "assertions=\"" CR_SIZE_T_FORMAT "\" " \
    "status=\"%s\" "                       \
    "time=\"%.3f\""

#define XML_TEST_TEMPLATE_BEGIN            \
    "    <testcase " TEST_PROPERTIES ">\n" \

#define XML_TEST_TEMPLATE_END \
    "    </testcase>\n"

#define XML_TEST_SKIPPED    "      <skipped/>\n"

#define LF                  "&#10;"

#define XML_FAILURE_MSG_ENTRY \
    "%s:%u: %s" LF

#define XML_TEST_FAILED_TEMPLATE_BEGIN \
    "      <failure type=\"assert\" message=\"%d assertion(s) failed.\">"

#define XML_TEST_FAILED_TEMPLATE_END \
    "</failure>\n"

#define XML_CRASH_MSG_ENTRY \
    "      <error type=\"crash\" message=\"The test crashed.\" />"

#define XML_TIMEOUT_MSG_ENTRY \
    "      <error type=\"timeout\" message=\"The test timed out.\" />"

static CR_INLINE const char *get_status_string(struct criterion_test_stats *ts)
{
    return (ts->crashed || ts->timed_out)         ? "ERRORED"
           : ts->test_status == CR_STATUS_FAILED   ? "FAILED"
           : ts->test_status == CR_STATUS_SKIPPED  ? "SKIPPED"
           : "PASSED";
}

/*
 * floats are printed locale dependent, but the xml-specification
 * requires a dot as the decimal separator.
 * Therefore we set the locale temporarily to print dots.
 */
static int fprintf_locale(FILE *stream,
        const char *format, ...)
{
    va_list args;
    int result;
    const char *locale = strdup(setlocale(LC_NUMERIC, NULL));

    setlocale(LC_NUMERIC, "C");
    va_start(args, format);
    result = vfprintf(stream, format, args);
    va_end(args);
    setlocale(LC_NUMERIC, locale);

    free((void *)locale);
    return result;
}

static void print_test(FILE *f, struct criterion_test_stats *ts)
{
    fprintf_locale(f, XML_TEST_TEMPLATE_BEGIN,
            ts->test->name,
            (size_t) (ts->passed_asserts + ts->failed_asserts),
            get_status_string(ts),
            ts->elapsed_time
            );

    if (ts->test_status == CR_STATUS_SKIPPED) {
        fprintf(f, XML_TEST_SKIPPED);
    } else if (ts->crashed) {
        fprintf(f, XML_CRASH_MSG_ENTRY);
    } else if (ts->timed_out) {
        fprintf(f, XML_TIMEOUT_MSG_ENTRY);
    } else {
        if (ts->test_status == CR_STATUS_FAILED) {
            fprintf(f, XML_TEST_FAILED_TEMPLATE_BEGIN, ts->failed_asserts);
            for (struct criterion_assert_stats *asrt = ts->asserts; asrt; asrt = asrt->next) {
                if (!asrt->passed) {
                    bool sf = criterion_options.short_filename;
                    char *dup = strdup(*asrt->message ? asrt->message : "");
                    char *saveptr = NULL;
                    char *line = strtok_r(dup, "\n", &saveptr);

                    fprintf(f, XML_FAILURE_MSG_ENTRY,
                            sf ? basename_compat(asrt->file) : asrt->file,
                            asrt->line,
                            line ? line : ""
                            );

                    while ((line = strtok_r(NULL, "\n", &saveptr)))
                        fprintf(f, "        %s" LF, line);
                    free(dup);
                }
            }
            fprintf(f, XML_TEST_FAILED_TEMPLATE_END);
        }
    }

    fprintf(f, XML_TEST_TEMPLATE_END);
}

static CR_INLINE float get_time_elapsed_suite(struct criterion_suite_stats *ss)
{
    float result = 0;

    for (struct criterion_test_stats *ts = ss->tests; ts; ts = ts->next)
        result += ts->elapsed_time;
    return result;
}

void xml_report(FILE *f, struct criterion_global_stats *stats)
{
    fprintf(f, XML_BASE_TEMPLATE_BEGIN,
            stats->nb_tests,
            stats->tests_failed,
            stats->tests_crashed,
            stats->tests_skipped
            );

    for (struct criterion_suite_stats *ss = stats->suites; ss; ss = ss->next) {
        fprintf_locale(f, XML_TESTSUITE_TEMPLATE_BEGIN,
                ss->suite->name,
                ss->nb_tests,
                ss->tests_failed,
                ss->tests_crashed,
                ss->tests_skipped,
                ss->tests_skipped,
                get_time_elapsed_suite(ss)
                );

        for (struct criterion_test_stats *ts = ss->tests; ts; ts = ts->next)
            print_test(f, ts);

        fprintf(f, XML_TESTSUITE_TEMPLATE_END);
    }

    fprintf(f, XML_BASE_TEMPLATE_END);
}
