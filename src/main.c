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
#include <getopt.h>
#include <csptr/smart_ptr.h>
#include "runner.h"

# define VERSION "v1.0.0"
# define VERSION_MSG "Tests compiled with Criterion " VERSION "\n"

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
    "    --pattern [PATTERN]: run tests matching the "      \
            "given pattern\n"                               \
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
    smart struct criterion_test_set *set = criterion_init();

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
                    is_disabled(&s->suite, t) ? "(disabled)" : "");
        }
    }
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
        {"pattern",         required_argument,  0, 'p'},
        {"always-succeed",  no_argument,        0, 'y'},
        {"no-early-exit",   no_argument,        0, 'z'},
        {0,                 0,                  0,  0 }
    };

    criterion_options = (struct criterion_options) {
        .always_succeed    = !strcmp("1", getenv("CRITERION_ALWAYS_SUCCEED") ?: "0"),
        .no_early_exit     = !strcmp("1", getenv("CRITERION_NO_EARLY_EXIT")  ?: "0"),
        .enable_tap_format = !strcmp("1", getenv("CRITERION_ENABLE_TAP")     ?: "0"),
        .logging_threshold = atoi(getenv("CRITERION_VERBOSITY_LEVEL") ?: "2"),
    };

    bool do_list_tests = false;
    bool do_print_version = false;
    bool do_print_usage = false;
    for (int c; (c = getopt_long(argc, argv, "hvlf", opts, NULL)) != -1;) {
        switch (c) {
            case 'b': criterion_options.logging_threshold = atoi(optarg ?: "1"); break;
            case 't': criterion_options.enable_tap_format = true; break;
            case 'y': criterion_options.always_succeed    = true; break;
            case 'z': criterion_options.no_early_exit     = true; break;
            case 'k': criterion_options.use_ascii         = true; break;
            case 'f': criterion_options.fail_fast         = true; break;
            case 'p': criterion_options.pattern           = optarg; break;
            case 'l': do_list_tests = true; break;
            case 'v': do_print_version = true; break;
            case 'h':
            default : do_print_usage = true; break;
        }
    }
    if (do_print_usage)
        return print_usage(argv[0]);
    if (do_print_version)
        return print_version();
    if (do_list_tests)
        return list_tests(!criterion_options.use_ascii);

    return !criterion_run_all_tests();
}
