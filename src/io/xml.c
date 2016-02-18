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
#include "criterion/stats.h"
#include "criterion/options.h"
#include "criterion/internal/ordered-set.h"
#include "log/logging.h"
#include "compat/posix.h"
#include "compat/strtok.h"
#include "compat/time.h"
#include "config.h"
#include "common.h"

#define TESTSUITES_PROPERTIES               \
    "name=\"Criterion Tests\" "             \
    "tests=\"" CR_SIZE_T_FORMAT "\" "       \
    "failures=\"" CR_SIZE_T_FORMAT "\" "    \
    "errors=\"" CR_SIZE_T_FORMAT "\" "      \
    "disabled=\"" CR_SIZE_T_FORMAT "\""

#define XML_BASE_TEMPLATE_BEGIN                             \
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"          \
    "<!-- Tests compiled with Criterion v" VERSION " -->\n" \
    "<testsuites " TESTSUITES_PROPERTIES ">\n"              \

#define XML_BASE_TEMPLATE_END   \
    "</testsuites>\n"

#define TESTSUITE_PROPERTIES                \
    "name=\"%s\" "                          \
    "tests=\"" CR_SIZE_T_FORMAT "\" "       \
    "failures=\"" CR_SIZE_T_FORMAT "\" "    \
    "errors=\"" CR_SIZE_T_FORMAT "\" "      \
    "disabled=\"" CR_SIZE_T_FORMAT "\" "    \
    "skipped=\"" CR_SIZE_T_FORMAT "\""

#define XML_TESTSUITE_TEMPLATE_BEGIN            \
    "  <testsuite " TESTSUITE_PROPERTIES ">\n"

#define XML_TESTSUITE_TEMPLATE_END  \
    "  </testsuite>\n"

#define TEST_PROPERTIES                     \
    "name=\"%s\" "                          \
    "assertions=\"" CR_SIZE_T_FORMAT "\" "  \
    "status=\"%s\""

#define XML_TEST_TEMPLATE_BEGIN            \
    "    <testcase " TEST_PROPERTIES ">\n" \

#define XML_TEST_TEMPLATE_END   \
    "    </testcase>\n"

#define XML_TEST_SKIPPED "      <skipped/>\n"

#define LF "&#10;"

#define XML_FAILURE_MSG_ENTRY \
    "%s:%u: %s" LF

#define XML_TEST_FAILED_TEMPLATE_BEGIN                                  \
    "      <failure type=\"assert\" message=\"%d assertion(s) failed.\">"

#define XML_TEST_FAILED_TEMPLATE_END    \
    "</failure>\n"

#define XML_CRASH_MSG_ENTRY \
    "      <error type=\"crash\" message=\"The test crashed.\" />"

#define XML_TIMEOUT_MSG_ENTRY \
    "      <error type=\"timeout\" message=\"The test timed out.\" />"

static INLINE bool is_disabled(struct criterion_test *t, struct criterion_suite *s) {
    return t->data->disabled || (s->data && s->data->disabled);
}

static CR_INLINE
const char *get_status_string(struct criterion_test_stats *ts,
                              struct criterion_suite_stats *ss) {

    const char *status = "PASSED";
    if (ts->crashed || ts->timed_out)
        status = "ERRORED";
    else if (ts->failed)
        status = "FAILED";
    else if (is_disabled(ts->test, ss->suite))
        status = "SKIPPED";
    return status;
}

static void print_test(FILE *f,
                       struct criterion_test_stats *ts,
                       struct criterion_suite_stats *ss) {

    fprintf(f, XML_TEST_TEMPLATE_BEGIN,
            ts->test->name,
            (size_t) (ts->passed_asserts + ts->failed_asserts),
            get_status_string(ts, ss)
        );

    if (is_disabled(ts->test, ss->suite)) {
        fprintf(f, XML_TEST_SKIPPED);
    } else if (ts->crashed) {
        fprintf(f, XML_CRASH_MSG_ENTRY);
    } else if (ts->timed_out) {
        fprintf(f, XML_TIMEOUT_MSG_ENTRY);
    } else {
        if (ts->failed) {
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
                            line
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

void xml_report(FILE *f, struct criterion_global_stats *stats) {
    fprintf(f, XML_BASE_TEMPLATE_BEGIN,
            stats->nb_tests,
            stats->tests_failed,
            stats->tests_crashed,
            stats->tests_skipped
        );

    for (struct criterion_suite_stats *ss = stats->suites; ss; ss = ss->next) {

        fprintf(f, XML_TESTSUITE_TEMPLATE_BEGIN,
                ss->suite->name,
                ss->nb_tests,
                ss->tests_failed,
                ss->tests_crashed,
                ss->tests_skipped,
                ss->tests_skipped
            );

        for (struct criterion_test_stats *ts = ss->tests; ts; ts = ts->next) {
            print_test(f, ts, ss);
        }

        fprintf(f, XML_TESTSUITE_TEMPLATE_END);
    }

    fprintf(f, XML_BASE_TEMPLATE_END);
}
