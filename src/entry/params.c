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
#include <locale.h>
#include <getopt.h>
#include <csptr/smalloc.h>
#include "criterion/criterion.h"
#include "criterion/options.h"
#include "criterion/internal/ordered-set.h"
#include "compat/posix.h"
#include "compat/strtok.h"
#include "core/runner.h"
#include "io/output.h"
#include "config.h"
#include "common.h"

#if ENABLE_NLS
# include <libintl.h>
#endif

# define VERSION_MSG "Tests compiled with Criterion v" VERSION "\n"

# define USAGE                                              \
    VERSION_MSG "\n"                                        \
    "usage: %s OPTIONS\n"                                   \
    "options: \n"                                           \
    "    -h or --help: prints this message\n"               \
    "    -q or --quiet: disables all logging\n"             \
    "    -v or --version: prints the version of criterion " \
            "these tests have been linked against\n"        \
    "    -l or --list: prints all the tests in a list\n"    \
    "    -jN or --jobs N: use N concurrent jobs\n"          \
    "    -f or --fail-fast: exit after the first failure\n" \
    "    --ascii: don't use fancy unicode symbols "         \
            "or colors in the output\n"                     \
    "    -S or --short-filename: only display the base "    \
            "name of the source file on a failure\n"        \
    "    --pattern [PATTERN]: run tests matching the "      \
            "given pattern\n"                               \
    "    --tap[=FILE]: writes TAP report in FILE "          \
            "(no file or \"-\" means stderr)\n"             \
    "    --xml[=FILE]: writes XML report in FILE "          \
            "(no file or \"-\" means stderr)\n"             \
    "    --always-succeed: always exit with 0\n"            \
    "    --no-early-exit: do not exit the test worker "     \
            "prematurely after the test\n"                  \
    "    --verbose[=level]: sets verbosity to level "       \
            "(1 by default)\n"                              \
    "    --crash: crash failing assertions rather than "    \
            "aborting (for debugging purposes)\n"           \
    "    -OP:F or --output=PROVIDER=FILE: write test "      \
            "report to FILE using the specified provider\n"

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

        printf("%s: " CR_SIZE_T_FORMAT " test%s\n",
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

int atou(const char *str) {
    int res = atoi(str);
    return res < 0 ? 0 : res;
}

CR_API int criterion_handle_args(int argc, char *argv[], bool handle_unknown_arg) {
    static struct option opts[] = {
        {"verbose",         optional_argument,  0, 'b'},
        {"quiet",           no_argument,        0, 'q'},
        {"version",         no_argument,        0, 'v'},
        {"tap",             optional_argument,  0, 't'},
        {"xml",             optional_argument,  0, 'x'},
        {"json",            optional_argument,  0, 'n'},
        {"help",            no_argument,        0, 'h'},
        {"list",            no_argument,        0, 'l'},
        {"ascii",           no_argument,        0, 'k'},
        {"jobs",            required_argument,  0, 'j'},
        {"fail-fast",       no_argument,        0, 'f'},
        {"short-filename",  no_argument,        0, 'S'},
        {"single",          required_argument,  0, 's'},
        {"pattern",         required_argument,  0, 'p'},
        {"always-succeed",  no_argument,        0, 'y'},
        {"no-early-exit",   no_argument,        0, 'z'},
        {"output",          required_argument,  0, 'O'},
        {"wait",            no_argument,        0, 'w'},
        {"crash",           no_argument,        0, 'c'},
        {0,                 0,                  0,  0 }
    };

    setlocale(LC_ALL, "");
#if ENABLE_NLS
    textdomain (PACKAGE "-test");
#endif

    if (!handle_unknown_arg)
        opterr = 0;

    char *env_always_succeed    = getenv("CRITERION_ALWAYS_SUCCEED");
    char *env_no_early_exit     = getenv("CRITERION_NO_EARLY_EXIT");
    char *env_fail_fast         = getenv("CRITERION_FAIL_FAST");
    char *env_use_ascii         = getenv("CRITERION_USE_ASCII");
    char *env_jobs              = getenv("CRITERION_JOBS");
    char *env_logging_threshold = getenv("CRITERION_VERBOSITY_LEVEL");
    char *env_short_filename    = getenv("CRITERION_SHORT_FILENAME");

    bool is_term_dumb = !strcmp("dumb", DEF(getenv("TERM"), "dumb"));

    struct criterion_options *opt = &criterion_options;
    if (env_always_succeed)
        opt->always_succeed    = !strcmp("1", env_always_succeed);
    if (env_no_early_exit)
        opt->no_early_exit     = !strcmp("1", env_no_early_exit);
    if (env_fail_fast)
        opt->fail_fast         = !strcmp("1", env_fail_fast);
    if (env_use_ascii)
        opt->use_ascii         = !strcmp("1", env_use_ascii) || is_term_dumb;
    if (env_jobs)
        opt->jobs              = atou(env_jobs);
    if (env_logging_threshold)
        opt->logging_threshold = (enum criterion_logging_level) atou(env_logging_threshold);
    if (env_short_filename)
        opt->short_filename    = !strcmp("1", env_short_filename);

    char *env_pattern = getenv("CRITERION_TEST_PATTERN");
    if (env_pattern)
        opt->pattern = env_pattern;

    opt->measure_time = !!strcmp("1", DEF(getenv("CRITERION_DISABLE_TIME_MEASUREMENTS"), "0"));

    bool quiet = false;

    // CRITERION_ENABLE_TAP backward compatibility.
    // The environment variable is otherwise deprecated.
    if (!strcmp("1", DEF(getenv("CRITERION_ENABLE_TAP"), "0"))) {
        quiet = true;
        criterion_add_output("tap", DEF(optarg, "-"));
    }

    bool do_list_tests = false;
    bool do_print_version = false;
    bool do_print_usage = false;

    const char *outputs = getenv("CRITERION_OUTPUTS");
    if (outputs) {
        char *out = strdup(outputs);
        char *buf = NULL;
        strtok_r(out, ",", &buf);

        for (char *s = out; s; s = strtok_r(NULL, ",", &buf)) {
            s = strdup(s);
            char *buf2      = NULL;
            char *provider  = strtok_r(s, ":", &buf2);
            char *path      = strtok_r(NULL, ":", &buf2);

            if (provider == NULL || path == NULL) {
                do_print_usage = true;
                goto end;
            }

            quiet = true;
            criterion_add_output(provider, path);
        }
        free(out);
    }

    for (int c; (c = getopt_long(argc, argv, "hvlfj:SqO:w", opts, NULL)) != -1;) {
        switch (c) {
            case 'b': criterion_options.logging_threshold = (enum criterion_logging_level) atou(DEF(optarg, "1")); break;
            case 'y': criterion_options.always_succeed    = true; break;
            case 'z': criterion_options.no_early_exit     = true; break;
            case 'k': criterion_options.use_ascii         = true; break;
            case 'j': criterion_options.jobs              = atou(optarg); break;
            case 'f': criterion_options.fail_fast         = true; break;
            case 'S': criterion_options.short_filename    = true; break;
            case 's': run_single_test_by_name(optarg); return 0;
            case 'p': criterion_options.pattern           = optarg; break;
            case 'q': quiet = true; break;

            {
                const char *provider;
            case 't': provider = "tap";  goto provider_def;
            case 'x': provider = "xml";  goto provider_def;
            case 'n': provider = "json"; goto provider_def;

            provider_def: {}
                const char *path = DEF(optarg, "-");
                quiet = !strcmp(path, "-");
                criterion_add_output(provider, path);
            } break;

            case 'l': do_list_tests = true; break;
            case 'v': do_print_version = true; break;
            case 'h': do_print_usage = true; break;
            case 'O': {
                char *arg = strdup(optarg);
                char *buf = NULL;
                strtok_r(arg,  ":", &buf);

                char *path = strtok_r(NULL, ":", &buf);
                if (arg == NULL || path == NULL) {
                    do_print_usage = true;
                    break;
                }

                quiet = !strcmp(path, "-");
                criterion_add_output(arg, path);
            } break;
            case 'w': criterion_options.wait_for_clients = true; break;
            case '?':
            case 'c': criterion_options.crash            = true; break;
            default : do_print_usage = handle_unknown_arg; break;
        }
    }

end:
    if (quiet)
        criterion_options.logging_threshold = CRITERION_LOG_LEVEL_QUIET;

    if (do_print_usage)
        return print_usage(argv[0]);
    if (do_print_version)
        return print_version();
    if (do_list_tests)
        return list_tests(!criterion_options.use_ascii);

    return 1;
}
