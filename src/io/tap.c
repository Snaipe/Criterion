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
#include "compat/posix.h"
#include "compat/strtok.h"
#include "compat/time.h"
#include "config.h"
#include "common.h"

static void print_prelude(FILE *f, struct criterion_global_stats *stats)
{
    fprintf(f, "TAP version 13\n1.."
            CR_SIZE_T_FORMAT
            "\n", stats->nb_tests);
    fprintf(f, "# Criterion v%s\n", VERSION);
}

static void print_pre_suite(FILE *f, struct criterion_suite_stats *stats)
{
    fprintf(f, "\n# Running "
            CR_SIZE_T_FORMAT
            " tests from %s\n",
            stats->nb_tests,
            stats->suite->name);
}

static void print_test_normal(FILE *f, struct criterion_test_stats *stats)
{
    const char *format = "%s - %s::%s %s (%3.2fs)\n";

    fprintf(f, format,
            stats->test_status == CR_STATUS_FAILED ? "not ok" : "ok",
            stats->test->category,
            stats->test->name,
            DEF(stats->test->data->description, ""),
            stats->elapsed_time);
    for (struct criterion_assert_stats *asrt = stats->asserts; asrt; asrt = asrt->next) {
        if (!asrt->passed) {
            char *dup = strdup(*asrt->message ? asrt->message : "");
            char *saveptr = NULL;
            char *line = strtok_r(dup, "\n", &saveptr);
            bool sf = criterion_options.short_filename;
            fprintf(f, "  %s:%u: Assertion failed: %s\n",
                    sf ? basename_compat(asrt->file) : asrt->file,
                    asrt->line,
                    line);

            while ((line = strtok_r(NULL, "\n", &saveptr)))
                fprintf(f, "    %s\n", line);
            free(dup);
        }
    }
}

static void print_test_crashed(FILE *f, struct criterion_test_stats *stats)
{
    bool sf = criterion_options.short_filename;

    fprintf(f, "not ok - %s::%s unexpected signal after %s:%u\n",
            stats->test->category,
            stats->test->name,
            sf ? basename_compat(stats->file) : stats->file,
            stats->progress);
}

static void print_test_timeout(FILE *f, struct criterion_test_stats *stats)
{
    fprintf(f, "not ok - %s::%s timed out (%3.2fs)\n",
            stats->test->category,
            stats->test->name,
            stats->elapsed_time);
}

static void print_test(FILE *f, struct criterion_test_stats *ts)
{
    if (ts->test_status == CR_STATUS_SKIPPED) {
        fprintf(f, "ok - %s::%s %s # SKIP %s\n",
                ts->test->category,
                ts->test->name,
                DEF(ts->test->data->description, ""),
                ts->message ? ts->message : "test was skipped");
    } else if (ts->crashed) {
        print_test_crashed(f, ts);
    } else if (ts->timed_out) {
        print_test_timeout(f, ts);
    } else {
        print_test_normal(f, ts);
    }
}

void tap_report(FILE *f, struct criterion_global_stats *stats)
{
    print_prelude(f, stats);

    for (struct criterion_suite_stats *ss = stats->suites; ss; ss = ss->next) {
        print_pre_suite(f, ss);

        for (struct criterion_test_stats *ts = ss->tests; ts; ts = ts->next)
            print_test(f, ts);
    }
}
