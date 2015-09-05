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
#include <criterion/criterion.h>
#include <criterion/options.h>
#include <criterion/ordered-set.h>
#include <stdio.h>
#include <locale.h>
#include <getopt.h>
#include <csptr/smalloc.h>
#include "runner.h"
#include "config.h"
#include "common.h"

#if ENABLE_NLS
# include <libintl.h>
#endif

# define VERSION_MSG "Tests compiled with Criterion v" VERSION "\n"

#ifdef HAVE_PCRE
# define PATTERN_USAGE                                      \
    "    --pattern [PATTERN]: run tests matching the "      \
            "given pattern\n"
#else
# define PATTERN_USAGE
#endif

# define USAGE                                              \
    VERSION_MSG "\n"                                        \
    "usage: %s OPTIONS\n"                                   \
    "options: \n"                                           \
    "    -h or --help: prints this message\n"               \
    "    -v or --version: prints the version of criterion " \
            "these tests have been linked against\n"        \
    "    -l or --list: prints all the tests in a list\n"    \
    "    -f or --fail-fast: exit after the first failure\n" \
    "    --ascii: don't use fancy unicode symbols "         \
            "or colors in the output\n"                     \
    "    -S or --short-filename: only display the base "    \
            "name of the source file on a failure\n"        \
    PATTERN_USAGE                                           \
    "    --tap: enables TAP formatting\n"                   \
    "    --always-succeed: always exit with 0\n"            \
    "    --no-early-exit: do not exit the test worker "     \
            "prematurely after the test\n"                  \
    "    --verbose[=level]: sets verbosity to level "       \
            "(1 by default)\n"

int print_usage(char *progname) {
    fprintf(stderr, USAGE, progname);
    return 0;
}

int print_version(void) {
    fputs(VERSION_MSG, stderr);
    return 0;
}

# define UTF8_TREE_NODE "├"
# define UTF8_TREE_END  "└"
# define UTF8_TREE_JOIN "──"

# define ASCII_TREE_NODE "|"
# define ASCII_TREE_END  "`"
# define ASCII_TREE_JOIN "--"

bool is_disabled(struct criterion_suite *s, struct criterion_test *t) {
    return (s->data && s->data->disabled) || t->data->disabled;
}

int list_tests(bool unicode) {
    struct criterion_test_set *set = criterion_init();

    const char *node = unicode ? UTF8_TREE_NODE : ASCII_TREE_NODE;
    const char *join = unicode ? UTF8_TREE_JOIN : ASCII_TREE_JOIN;
    const char *end  = unicode ? UTF8_TREE_END  : ASCII_TREE_END;

    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        size_t tests = s->tests ? s->tests->size : 0;
        if (!tests)
            continue;

        printf("%s: " SIZE_T_FORMAT " test%s\n",
                s->suite.name,
                tests,
                tests == 1 ? "" : "s");

        FOREACH_SET(struct criterion_test *t, s->tests) {
            printf("%s%s %s%s\n",
                    --tests == 0 ? end : node,
                    join,
                    t->name,
                    is_disabled(&s->suite, t) ? " (disabled)" : "");
        }
    }

    sfree(set);
    return 0;
}

int main(int argc, char *argv[]) {
    static struct option opts[] = {
        {"verbose",         optional_argument,  0, 'b'},
        {"version",         no_argument,        0, 'v'},
        {"tap",             no_argument,        0, 't'},
        {"help",            no_argument,        0, 'h'},
        {"list",            no_argument,        0, 'l'},
        {"ascii",           no_argument,        0, 'k'},
        {"fail-fast",       no_argument,        0, 'f'},
        {"short-filename",  no_argument,        0, 'S'},
#ifdef HAVE_PCRE
        {"pattern",         required_argument,  0, 'p'},
#endif
        {"always-succeed",  no_argument,        0, 'y'},
        {"no-early-exit",   no_argument,        0, 'z'},
        {0,                 0,                  0,  0 }
    };

    bool use_ascii = !strcmp("1", DEF(getenv("CRITERION_USE_ASCII"), "0"))
                  || !strcmp("dumb", DEF(getenv("TERM"), "dumb"));

    setlocale(LC_ALL, "");
#if ENABLE_NLS
    textdomain (PACKAGE "-test");
#endif

    struct criterion_options *opt = &criterion_options;
    opt->always_succeed    = !strcmp("1", DEF(getenv("CRITERION_ALWAYS_SUCCEED"), "0"));
    opt->no_early_exit     = !strcmp("1", DEF(getenv("CRITERION_NO_EARLY_EXIT") , "0"));
    opt->fail_fast         = !strcmp("1", DEF(getenv("CRITERION_FAIL_FAST")     , "0"));
    opt->use_ascii         = use_ascii;
    opt->logging_threshold = atoi(DEF(getenv("CRITERION_VERBOSITY_LEVEL"), "2"));
    opt->short_filename    = !strcmp("1", DEF(getenv("CRITERION_SHORT_FILENAME"), "0"));
#ifdef HAVE_PCRE
    opt->pattern           = getenv("CRITERION_TEST_PATTERN");
#endif

    bool use_tap = !strcmp("1", DEF(getenv("CRITERION_ENABLE_TAP"), "0"));

    bool do_list_tests = false;
    bool do_print_version = false;
    bool do_print_usage = false;
    for (int c; (c = getopt_long(argc, argv, "hvlfS", opts, NULL)) != -1;) {
        switch (c) {
            case 'b': criterion_options.logging_threshold = atoi(DEF(optarg, "1")); break;
            case 'y': criterion_options.always_succeed    = true; break;
            case 'z': criterion_options.no_early_exit     = true; break;
            case 'k': criterion_options.use_ascii         = true; break;
            case 'f': criterion_options.fail_fast         = true; break;
            case 'S': criterion_options.short_filename    = true; break;
#ifdef HAVE_PCRE
            case 'p': criterion_options.pattern           = optarg; break;
#endif
            case 't': use_tap = true; break;
            case 'l': do_list_tests = true; break;
            case 'v': do_print_version = true; break;
            case 'h':
            default : do_print_usage = true; break;
        }
    }
    if (use_tap)
        criterion_options.output_provider = TAP_LOGGING;
    if (do_print_usage)
        return print_usage(argv[0]);
    if (do_print_version)
        return print_version();
    if (do_list_tests)
        return list_tests(!criterion_options.use_ascii);

    return !criterion_run_all_tests();
}
