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
#define CRITERION_LOGGING_COLORS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "criterion/stats.h"
#include "criterion/options.h"
#include "criterion/internal/ordered-set.h"
#include "protocol/protocol.h"
#include "log/logging.h"
#include "compat/posix.h"
#include "compat/strtok.h"
#include "compat/time.h"
#include "string/i18n.h"
#include "string/xxd.h"
#include "string/diff.h"
#include "config.h"
#include "common.h"

#define LOG_DIFF_THRESHOLD    25

typedef const char *const msg_t;

static msg_t msg_pre_all = N_("Criterion v%s\n");
static msg_t msg_desc = N_("  %s\n");

#ifdef ENABLE_NLS
static msg_t msg_pre_init = N_("%1$s::%2$s\n");
static msg_t msg_post_test = N_("%1$s::%2$s\n");
static msg_t msg_post_test_timed = N_("%1$s::%2$s: (%3$3.2fs)\n");
static msg_t msg_post_test_skip = N_("%1$s::%2$s: Test was skipped\n");
static msg_t msg_test_disabled = N_("%1$s::%2$s: Test is disabled\n");
static msg_t msg_assert_fail = N_("%1$s%2$s%3$s:%4$s%5$d%6$s: %7$s\n");
static msg_t msg_assert_repr = N_("  %1$s: %2$s\n");
static msg_t msg_assert_eq_short = N_("    %1$s: %2$s[-%3$s-]%4$s%5$s{+%6$s+}%7$s\n");
static msg_t msg_assert_param = N_("    %1$s: %2$s%3$s%4$s\n");
static msg_t msg_theory_fail = N_("  Theory %1$s::%2$s failed with the following parameters: (%3$s)\n");
static msg_t msg_test_timeout = N_("%1$s::%2$s: Timed out. (%3$3.2fs)\n");
static msg_t msg_test_crash_line = N_("%1$s%2$s%3$s:%4$s%5$u%6$s: Unexpected signal caught below this line!\n");
static msg_t msg_test_crash = N_("%1$s::%2$s: CRASH!\n");
static msg_t msg_test_generic = N_("%1$s::%2$s: %3$s\n");
static msg_t msg_test_other_crash = N_("%1$sWarning! The test `%2$s::%3$s` crashed during its setup or teardown.%4$s\n");
static msg_t msg_test_abnormal_exit = N_("%1$sWarning! The test `%2$s::%3$s` exited during its setup or teardown.%4$s\n");
static msg_t msg_pre_suite[] = N_s("Running %1$s%2$lu%3$s test from %4$s%5$s%6$s:\n",
                "Running %1$s%2$lu%3$s tests from %4$s%5$s%6$s:\n");
static msg_t msg_post_all = N_("%1$sSynthesis: Tested: %2$s%3$lu%4$s "
                "| Passing: %5$s%6$lu%7$s "
                "| Failing: %8$s%9$lu%10$s "
                "| Crashing: %11$s%12$lu%13$s "
                "%14$s\n");
static msg_t msg_line = N_("%2$*1$s%3$s%4$s%5$s\n");
#else
static msg_t msg_pre_init = "%s::%s\n";
static msg_t msg_post_test = "%s::%s\n";
static msg_t msg_post_test_timed = "%s::%s: (%3.2fs)\n";
static msg_t msg_post_test_skip = "%s::%s: Test was skipped\n";
static msg_t msg_test_disabled = "%s::%s: Test is disabled\n";
static msg_t msg_assert_fail = "%s%s%s:%s%d%s: %s\n";
static msg_t msg_assert_repr = "  %s: %s\n";
static msg_t msg_assert_eq_short = "    %s: %s[-%s-]%s%s{+%s+}%s\n";
static msg_t msg_assert_param = "    %s: %s%s%s\n";
static msg_t msg_theory_fail = "  Theory %s::%s failed with the following parameters: (%s)\n";
static msg_t msg_test_timeout = "%s::%s: Timed out. (%3.2fs)\n";
static msg_t msg_test_crash_line = "%s%s%s:%s%u%s: Unexpected signal caught below this line!\n";
static msg_t msg_test_crash = "%s::%s: CRASH!\n";
static msg_t msg_test_generic = "%s::%s: %s\n";
static msg_t msg_test_other_crash = "%sWarning! The test `%s::%s` crashed during its setup or teardown.%s\n";
static msg_t msg_test_abnormal_exit = "%sWarning! The test `%s::%s` exited during its setup or teardown.%s\n";
static msg_t msg_pre_suite[] = { "Running %s%lu%s test from %s%s%s:\n",
                                 "Running %s%lu%s tests from %s%s%s:\n" };
static msg_t msg_post_all = "%sSynthesis: Tested: %s%lu%s "
        "| Passing: %s%lu%s "
        "| Failing: %s%lu%s "
        "| Crashing: %s%lu%s "
        "%s\n";
static msg_t msg_line = "%*s%s%s%s\n";
#endif

void normal_log_pre_all(CR_UNUSED struct criterion_test_set *set)
{
    criterion_pinfo(CRITERION_PREFIX_DASHES, _(msg_pre_all), VERSION);
}

void normal_log_pre_init(struct criterion_suite *suite, struct criterion_test *test)
{
    if (test->data->disabled || (suite->data && suite->data->disabled)) {
        criterion_pinfo(CRITERION_PREFIX_SKIP, _(msg_test_disabled),
                test->category,
                test->name);
    } else {
        criterion_pinfo(CRITERION_PREFIX_RUN, _(msg_pre_init),
                test->category,
                test->name);
    }

    if (test->data->description)
        criterion_pinfo(CRITERION_PREFIX_DASHES, _(msg_desc),
                test->data->description);
}

void normal_log_post_test(struct criterion_test_stats *stats)
{
    const char *format = criterion_options.measure_time ?
        msg_post_test_timed : msg_post_test;

    const enum criterion_logging_level level =
            stats->test_status == CR_STATUS_FAILED ? CRITERION_IMPORTANT : CRITERION_INFO;
    const struct criterion_prefix_data *prefix =
            stats->test_status == CR_STATUS_FAILED ? CRITERION_PREFIX_FAIL
            : CRITERION_PREFIX_PASS;

    if (stats->test_status == CR_STATUS_SKIPPED) {
        if (!stats->message) {
            criterion_pinfo(CRITERION_PREFIX_SKIP, _(msg_post_test_skip),
                    stats->test->category,
                    stats->test->name);
        } else {
            criterion_pinfo(CRITERION_PREFIX_SKIP, _(msg_test_generic),
                    stats->test->category,
                    stats->test->name,
                    stats->message);
        }
    } else {
        criterion_plog(level, prefix, _(format),
                stats->test->category,
                stats->test->name,
                stats->elapsed_time);
    }
}

void normal_log_post_all(struct criterion_global_stats *stats)
{
    size_t tested = stats->nb_tests - stats->tests_skipped;
    char *tests_crashed_color = (stats->tests_crashed) ? CR_FG_RED : CR_RESET;
    char *tests_failed_color  = (stats->tests_failed)  ? CR_FG_RED : CR_RESET;

    criterion_pimportant(CRITERION_PREFIX_EQUALS,
            _(msg_post_all),
            CR_FG_BOLD,
            CR_FG_BLUE, (unsigned long) tested, CR_FG_BOLD,
            CR_FG_GREEN, (unsigned long) stats->tests_passed, CR_FG_BOLD,
            tests_failed_color, (unsigned long) stats->tests_failed, CR_FG_BOLD,
            tests_crashed_color, (unsigned long) stats->tests_crashed, CR_FG_BOLD,
            CR_RESET);
}

void normal_log_assert(struct criterion_assert_stats *stats)
{
    if (!stats->passed) {
        int dynpath = 0;
        const char *path = NULL;
        if (criterion_options.short_filename) {
            path = basename_compat(stats->file);
        } else if (cri_path_isrelative(stats->file)) {
            path = stats->file;
        } else {
            path = cri_path_relativeof(stats->file);
            if (!path) {
                path = stats->file;
            } else {
                dynpath = 1;
            }
        }

        criterion_pimportant(CRITERION_PREFIX_DASHES,
                _(msg_assert_fail),
                CR_FG_BOLD, path, CR_RESET,
                CR_FG_RED, stats->line, CR_RESET,
                _(N_("Assertion Failed")));

        if (dynpath)
            free((char *) path);

        if (stats->message && *stats->message) {
            criterion_pimportant(CRITERION_PREFIX_DASHES, _(msg_desc), "");

            char *dup       = strdup(stats->message);
            char *saveptr   = NULL;
            char *line      = strtok_r(dup, "\n", &saveptr);

            do {
                criterion_pimportant(CRITERION_PREFIX_DASHES, _(msg_desc), line);
            } while ((line = strtok_r(NULL, "\n", &saveptr)));
            free(dup);

            criterion_pimportant(CRITERION_PREFIX_DASHES, _(msg_desc), "");
        }
    }
}

void normal_log_assert_sub(struct criterion_assert_stats *stats,
        const char *repr, const char *msg)
{
    (void) stats;

    criterion_pimportant(CRITERION_PREFIX_DASHES,
            _(msg_assert_repr), repr, msg ? msg : "");
}

void normal_log_assert_formatted(struct criterion_assert_stats *stats,
        const char *formatted)
{
    (void) stats;

    char *dup       = strdup(formatted);
    char *saveptr   = NULL;
    char *line      = strtok_r(dup, "\n", &saveptr);

    for (; line; line = strtok_r(NULL, "\n", &saveptr)) {
        char *color;
        switch (line[0]) {
            case '+':   color = CR_FG_GREEN; break;
            case '-':   color = CR_FG_RED; break;
            case '@':   color = CR_FG_BLUE; break;
            default:    color = ""; break;
        }
        criterion_pimportant(CRITERION_PREFIX_DASHES, _(msg_line),
                4, "", /* 4 space indent */
                color, line, CR_RESET);
    }
    free(dup);
}

void normal_log_assert_param_eq(struct criterion_assert_stats *stats,
        struct cr_log_assert_param *actual,
        struct cr_log_assert_param *expected)
{
    (void) stats;

    if (!expected->data || !actual->data)
        return;

    if (expected->size + actual->size <= LOG_DIFF_THRESHOLD
            && expected->kind == CR_LOG_PARAM_STR
            && !strchr(expected->data, '\n')
            && actual->kind == CR_LOG_PARAM_STR
            && !strchr(actual->data, '\n')) {
        criterion_pimportant(CRITERION_PREFIX_DASHES,
                _(msg_assert_eq_short),
                "diff",
                CR_FG_RED, (char *) actual->data, CR_RESET,
                CR_FG_GREEN, (char *) expected->data, CR_RESET);
        return;
    }

    struct cri_diff_buffer bexpected = {
        .ptr  = strdup(expected->data),
        .size = expected->size + 1,
    };
    struct cri_diff_buffer bactual = {
        .ptr  = strdup(actual->data),
        .size = actual->size + 1,
    };
    struct cri_diff_buffer diff;

    if (expected->kind != CR_LOG_PARAM_STR || actual->kind != CR_LOG_PARAM_STR) {
        char *xxd = cri_string_xxd(expected->data, 0, expected->size);
        free(bexpected.ptr);
        bexpected.ptr = xxd;
        bexpected.size = strlen(bexpected.ptr) + 1;

        xxd = cri_string_xxd(actual->data, 0, actual->size);
        free(bactual.ptr);
        bactual.ptr = xxd;
        bactual.size = strlen(bactual.ptr) + 1;
    }

    /* Replace null-terminating byte by newline. This is fine
       since we don't actually want to process this as a string
       but as a regular char buffer.

       This is done to normalize the diff inputs to be
       newline-terminated. */
    bexpected.ptr[bexpected.size - 1] = '\n';
    bactual.ptr[bactual.size - 1] = '\n';

    int rc = cri_diff_buffer_to_buffer(&bexpected, &bactual, &diff);
    if (rc >= 0) {
        normal_log_assert_formatted(stats, diff.ptr);
    }

    free(diff.ptr);
    free(bexpected.ptr);
    free(bactual.ptr);
}

void normal_log_assert_param(struct criterion_assert_stats *stats,
        struct cr_log_assert_param *param)
{
    (void) stats;

    if (!param->name || !param->name[0] || !param->data)
        return;

    if (param->kind == CR_LOG_PARAM_STR) {
        if (!*(char *) param->data)
            return;

        const char *color = !strcmp(param->name, "expected") ? CR_FG_GREEN
                : !strcmp(param->name, "reference") ? CR_FG_GREEN
                : !strcmp(param->name, "actual") ? CR_FG_RED
                : !strcmp(param->name, "value") ? CR_FG_RED
                : "";

        char *line = cri_strtokc((char *) param->data, '\n');

        if (line) {
            criterion_pimportant(CRITERION_PREFIX_DASHES,
                    _(msg_assert_param), param->name, color, line, CR_RESET);

            while ((line = cri_strtokc(NULL, '\n')))
                criterion_pimportant(CRITERION_PREFIX_DASHES, "      %s%s%s\n", color, line, CR_RESET);
        }
    } else if (param->kind == CR_LOG_PARAM_RAW) {
        criterion_pimportant(CRITERION_PREFIX_DASHES,
                _(msg_assert_param), param->name, "", "", "");

        char *raw = cri_string_xxd(param->data, 0, param->size);
        for (char *c = raw, *start = raw; *c; ++c) {
            if (*c == '\n') {
                *c = '\0';
                criterion_pimportant(CRITERION_PREFIX_DASHES,
                        _(msg_desc), start);
                start = c + 1;
            }
        }
        free(raw);
    }
}

void normal_log_test_crash(struct criterion_test_stats *stats)
{
    bool sf = criterion_options.short_filename;

    criterion_pimportant(CRITERION_PREFIX_DASHES,
            _(msg_test_crash_line),
            CR_FG_BOLD, sf ? basename_compat(stats->file) : stats->file, CR_RESET,
            CR_FG_RED, stats->progress, CR_RESET);
    criterion_pimportant(CRITERION_PREFIX_FAIL, _(msg_test_crash),
            stats->test->category,
            stats->test->name);
}

void normal_log_other_crash(CR_UNUSED struct criterion_test_stats *stats)
{
    criterion_pimportant(CRITERION_PREFIX_DASHES,
            _(msg_test_other_crash),
            CR_FG_BOLD, stats->test->category, stats->test->name, CR_RESET);
}

void normal_log_abnormal_exit(CR_UNUSED struct criterion_test_stats *stats)
{
    criterion_pimportant(CRITERION_PREFIX_DASHES,
            _(msg_test_abnormal_exit),
            CR_FG_BOLD, stats->test->category, stats->test->name, CR_RESET);
}

void normal_log_pre_suite(struct criterion_suite_set *set)
{
    criterion_pinfo(CRITERION_PREFIX_EQUALS,
            _s(msg_pre_suite[0], msg_pre_suite[1], set->tests->size),
            CR_FG_BLUE, (unsigned long) set->tests->size, CR_RESET,
            CR_FG_GOLD, set->suite.name, CR_RESET);

    if (set->suite.data && set->suite.data->description)
        criterion_pinfo(CRITERION_PREFIX_DASHES, _(msg_desc),
                set->suite.data->description);
}

void normal_log_theory_fail(struct criterion_theory_stats *stats)
{
    criterion_pimportant(CRITERION_PREFIX_DASHES,
            _(msg_theory_fail),
            stats->stats->test->category,
            stats->stats->test->name,
            stats->formatted_args);
}

void normal_log_test_timeout(CR_UNUSED struct criterion_test_stats *stats)
{
    criterion_pimportant(CRITERION_PREFIX_FAIL,
            _(msg_test_timeout),
            stats->test->category,
            stats->test->name,
            stats->elapsed_time);
}

void normal_log_test_abort(CR_UNUSED struct criterion_test_stats *stats, const char *msg)
{
    char *dup       = strdup(msg);
    char *saveptr   = NULL;
    char *line      = strtok_r(dup, "\n", &saveptr);

    criterion_pimportant(CRITERION_PREFIX_DASHES,
            _(msg_test_generic),
            stats->test->category,
            stats->test->name,
            line);

    while ((line = strtok_r(NULL, "\n", &saveptr)))
        criterion_pimportant(CRITERION_PREFIX_DASHES, _(msg_desc), line);

    free(dup);
}

void normal_log_message(enum criterion_severity severity, const char *msg)
{
    char *dup       = strdup(msg);
    char *saveptr   = NULL;
    char *line      = strtok_r(dup, "\n", &saveptr);

    do {
        if (*line != '\0')
            criterion_log_noformat(severity, line);
    } while ((line = strtok_r(NULL, "\n", &saveptr)));
    free(dup);
}

struct criterion_logger normal_logging = {
    .log_pre_all            = normal_log_pre_all,
    .log_pre_init           = normal_log_pre_init,
    .log_pre_suite          = normal_log_pre_suite,
    .log_assert             = normal_log_assert,
    .log_assert_sub         = normal_log_assert_sub,
    .log_assert_param       = normal_log_assert_param,
    .log_assert_param_eq    = normal_log_assert_param_eq,
    .log_assert_formatted   = normal_log_assert_formatted,
    .log_theory_fail        = normal_log_theory_fail,
    .log_test_timeout       = normal_log_test_timeout,
    .log_test_crash         = normal_log_test_crash,
    .log_test_abort         = normal_log_test_abort,
    .log_other_crash        = normal_log_other_crash,
    .log_abnormal_exit      = normal_log_abnormal_exit,
    .log_post_test          = normal_log_post_test,
    .log_post_all           = normal_log_post_all,
    .log_message            = normal_log_message,
};
