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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "criterion/stats.h"
#include "criterion/logging.h"
#include "criterion/options.h"
#include "criterion/ordered-set.h"
#include "compat/posix.h"
#include "compat/time.h"
#include "config.h"
#include "common.h"

#ifdef _MSC_VER
# define strdup _strdup
#endif

static void print_prelude(struct criterion_global_stats *stats) {
    criterion_important("TAP version 13\n1.."
                                CR_SIZE_T_FORMAT
                                "\n", stats->nb_tests);
    criterion_important("# Criterion v%s\n", VERSION);
}

static void print_pre_suite(struct criterion_suite_stats *stats) {
    criterion_important("\n# Running "
                                CR_SIZE_T_FORMAT
                                " tests from %s\n",
            stats->nb_tests,
            stats->suite->name);
}

static INLINE bool is_disabled(struct criterion_test *t, struct criterion_suite *s) {
    return t->data->disabled || (s->data && s->data->disabled);
}

static void print_test_normal(struct criterion_test_stats *stats) {
    const char *format = can_measure_time() ? "%s - %s::%s %s (%3.2fs)\n"
                                            : "%s - %s::%s %s\n";
    criterion_important(format,
            stats->failed ? "not ok" : "ok",
            stats->test->category,
            stats->test->name,
            DEF(stats->test->data->description, ""),
            stats->elapsed_time);
    for (struct criterion_assert_stats *asrt = stats->asserts; asrt; asrt = asrt->next) {
        if (!asrt->passed) {
            char *dup = strdup(*asrt->message ? asrt->message : "");
#ifdef VANILLA_WIN32
            char *line = strtok(dup, "\n");
#else
            char *saveptr = NULL;
            char *line = strtok_r(dup, "\n", &saveptr);
#endif
            bool sf = criterion_options.short_filename;
            criterion_important("  %s:%u: Assertion failed: %s\n",
                    sf ? basename_compat(asrt->file) : asrt->file,
                    asrt->line,
                    line);
#ifdef VANILLA_WIN32
            while ((line = strtok(NULL, "\n")))
#else
            while ((line = strtok_r(NULL, "\n", &saveptr)))
#endif
                criterion_important("    %s\n", line);
            free(dup);
        }
    }
}

static void print_test_crashed(struct criterion_test_stats *stats) {
    bool sf = criterion_options.short_filename;
    criterion_important("not ok - %s::%s unexpected signal after %s:%u\n",
            stats->test->category,
            stats->test->name,
            sf ? basename_compat(stats->file) : stats->file,
            stats->progress);
}

static void print_test_timeout(struct criterion_test_stats *stats) {
    criterion_important("not ok - %s::%s timed out (%3.2fs)\n",
            stats->test->category,
            stats->test->name,
            stats->elapsed_time);
}

static void print_test(struct criterion_test_stats *ts,
                       struct criterion_suite_stats *ss) {

    if (is_disabled(ts->test, ss->suite)) {
        criterion_important("ok - %s::%s %s # SKIP %s is disabled\n",
                ts->test->category,
                ts->test->name,
                DEF(ts->test->data->description, ""),
                ts->test->data->disabled ? "test" : "suite");
    } else if (ts->crashed) {
        print_test_crashed(ts);
    } else if (ts->timed_out) {
        print_test_timeout(ts);
    } else {
        print_test_normal(ts);
    }
}

void tap_log_post_all(struct criterion_global_stats *stats) {
    print_prelude(stats);

    for (struct criterion_suite_stats *ss = stats->suites; ss; ss = ss->next) {
        print_pre_suite(ss);

        for (struct criterion_test_stats *ts = ss->tests; ts; ts = ts->next) {
            print_test(ts, ss);
        }
    }
}

struct criterion_output_provider tap_logging = {
    .log_post_all = tap_log_post_all,
};
