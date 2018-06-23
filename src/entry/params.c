/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2018 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#include "err.h"

#if ENABLE_NLS
# include <libintl.h>
#endif

#ifdef HAVE_ISATTY
# include <unistd.h>
#endif

#ifdef HAVE_NL_LANGINFO
# include <langinfo.h>
#endif

#define VERSION_MSG    "Tests compiled with Criterion v" VERSION "\n"

#define USAGE                                               \
    VERSION_MSG "\n"                                        \
    "usage: %s OPTIONS\n"                                   \
    "options: \n"                                           \
    "    -h or --help: prints this message\n"               \
    "    -q or --quiet: disables all logging\n"             \
    "    -v or --version: prints the version of criterion " \
    "these tests have been linked against\n"                \
    "    -l or --list: prints all the tests in a list\n"    \
    "    -jN or --jobs N: use N concurrent jobs\n"          \
    "    -f or --fail-fast: exit after the first failure\n" \
    "    --color=<auto|always|never>: colorize the output\n"\
    "    --encoding=<ENCODING>: use the specified encoding "\
    "for the output (default: locale-deduced)\n"            \
    "    --ascii: don't use fancy unicode symbols "         \
    "or colors in the output\n"                             \
    "    -S or --short-filename: only display the base "    \
    "name of the source file on a failure\n"                \
    "    --filter [PATTERN]: run tests matching the "       \
    "given pattern\n"                                       \
    "    --timeout [TIMEOUT]: set a timeout (in seconds) "  \
    "for all tests\n"                                       \
    "    --tap[=FILE]: writes TAP report in FILE "          \
    "(no file or \"-\" means stderr)\n"                     \
    "    --xml[=FILE]: writes XML report in FILE "          \
    "(no file or \"-\" means stderr)\n"                     \
    "    --always-succeed: always exit with 0\n"            \
    "    --verbose[=level]: sets verbosity to level "       \
    "(1 by default)\n"                                      \
    "    --crash: crash failing assertions rather than "    \
    "aborting (for debugging purposes)\n"                   \
    "    --debug[=TYPE]: run tests with a debugging "       \
    "server, listening on localhost:1234 by "               \
    "default. TYPE may be gdb, lldb, or wingbd.\n"          \
    "    --debug-transport=VAL: the transport to use by "   \
    "the debugging server. `tcp:1234` by default\n"         \
    "    --full-stats: Tests must fully report statistics " \
    "(causes massive slowdown for large number of "         \
    "assertions but is more accurate).\n"                   \
    "    --ignore-warnings: Ignore warnings, do not exit "  \
    "with a non-zero exit status.\n"                        \
    "    -OP:F or --output=PROVIDER=FILE: write test "      \
    "report to FILE using the specified provider\n"

int print_usage(char *progname)
{
    fprintf(stderr, USAGE, progname);
    return 0;
}

int print_version(void)
{
    fputs(VERSION_MSG, stderr);
    return 0;
}

#define UTF8_TREE_NODE     "├"
#define UTF8_TREE_END      "└"
#define UTF8_TREE_JOIN     "──"

#define ASCII_TREE_NODE    "|"
#define ASCII_TREE_END     "`"
#define ASCII_TREE_JOIN    "--"

bool is_disabled(struct criterion_suite *s, struct criterion_test *t)
{
    return (s->data && s->data->disabled) || t->data->disabled;
}

int list_tests(void)
{
    bool unicode = !strcmp(criterion_options.encoding, "UTF-8");
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
                    is_disabled(&s->suite, t) ? " (skipped)" : "");
        }
    }

    sfree(set);
    return 0;
}

int atou(const char *str)
{
    int res = atoi(str);

    return res < 0 ? 0 : res;
}

static int parse_dbg_transport(const char *arg)
{
    int ok = 1;
    char *dup = strdup(arg);

    char *sptr;
    char *transport = strtok_r(dup, ":", &sptr);

    if (!transport) {
        fprintf(stderr, "Invalid --debug parameter '%s'\n", arg);
        goto err;
    }
    char *val = dup + strlen(transport) + 1;

    if (!strcmp(transport, "tcp")) {
        criterion_options.debug_port = atou(val);
    } else {
        fprintf(stderr, "Unknown transport '%s'\n", transport);
        goto err;
    }

    ok = 1;
err:
    free(dup);
    return ok;
}

static int parse_dbg(const char *arg)
{
    if (!arg)
        return criterion_options.debug = CR_DBG_NATIVE;

    static struct { char *name; enum criterion_debugger dbg; } values[] = {
        { "gdb",    CR_DBG_GDB    },
        { "lldb",   CR_DBG_LLDB   },
        { "windbg", CR_DBG_WINDBG },
        { "idle",   CR_DBG_IDLE   },
        { NULL,     0             },
    };

    for (size_t i = 0; values[i].name; ++i) {
        if (!strcmp(values[i].name, arg)) {
            criterion_options.debug = values[i].dbg;
            return 1;
        }
    }

    fprintf(stderr, "Invalid argument for --debug: %s.\n", arg);
    return 0;
}

static void set_encoding(char *encoding)
{
    size_t maxlen = sizeof (criterion_options.encoding) - 1;
    strncpy(criterion_options.encoding, encoding, maxlen);
    criterion_options.encoding[maxlen] = 0;
}

static bool deduce_color(char *arg)
{
    bool color;
    if (!arg || !strcmp(arg, "always")) {
        color = true;
    } else if (!strcmp(arg, "auto")) {
        color = true
#ifdef HAVE_ISATTY
            && isatty(STDERR_FILENO)
#endif
            && strcmp("dumb", DEF(getenv("TERM"), "dumb")) != 0;

    } else if (!strcmp(arg, "never")) {
        color = false;
    } else {
        fprintf(stderr, "Unknown color mode '%s'.\n", arg);
        exit(3);
    }
    return color;
}

static bool must_be_quiet(const char *output_path)
{
    return !strcmp(output_path, "-")
        || !strcmp(output_path, "/dev/stderr");
}

CR_API int criterion_handle_args(int argc, char *argv[],
        bool handle_unknown_arg)
{
    static struct option opts[] = {
        { "verbose",         optional_argument, 0, 'b' },
        { "quiet",           no_argument,       0, 'q' },
        { "version",         no_argument,       0, 'v' },
        { "tap",             optional_argument, 0, 'T' },
        { "xml",             optional_argument, 0, 'x' },
        { "json",            optional_argument, 0, 'n' },
        { "help",            no_argument,       0, 'h' },
        { "list",            no_argument,       0, 'l' },
        { "ascii",           no_argument,       0, 'k' },
        { "jobs",            required_argument, 0, 'j' },
        { "timeout",         required_argument, 0, 't' },
        { "fail-fast",       no_argument,       0, 'f' },
        { "short-filename",  no_argument,       0, 'S' },
        { "single",          required_argument, 0, 's' },
        { "pattern",         required_argument, 0, 'p' },
        { "filter",          required_argument, 0, 'F' },
        { "always-succeed",  no_argument,       0, 'y' },
        { "no-early-exit",   no_argument,       0, 'z' },
        { "output",          required_argument, 0, 'O' },
        { "wait",            no_argument,       0, 'w' },
        { "crash",           no_argument,       0, 'c' },
        { "debug",           optional_argument, 0, 'd' },
        { "debug-transport", required_argument, 0, 'D' },
        { "full-stats",      no_argument,       0, 'U' },
        { "color",           optional_argument, 0, 'C' },
        { "encoding",        required_argument, 0, 'e' },
        { "ignore-warnings", no_argument,       0, 'N' },
        { 0,                 0,                 0, 0   }
    };

    setlocale(LC_ALL, "");
#if ENABLE_NLS
    textdomain(PACKAGE "-test");
#endif

#ifdef HAVE_NL_LANGINFO
    set_encoding(nl_langinfo(CODESET));
#else
    set_encoding("ANSI_X3.4-1968");
#endif
    criterion_options.color = deduce_color("auto");

    if (!handle_unknown_arg)
        opterr = 0;

    char *env_always_succeed    = getenv("CRITERION_ALWAYS_SUCCEED");
    char *env_fail_fast         = getenv("CRITERION_FAIL_FAST");
    char *env_use_ascii         = getenv("CRITERION_USE_ASCII");
    char *env_jobs              = getenv("CRITERION_JOBS");
    char *env_logging_threshold = getenv("CRITERION_VERBOSITY_LEVEL");
    char *env_short_filename    = getenv("CRITERION_SHORT_FILENAME");

    bool is_term_dumb = !strcmp("dumb", DEF(getenv("TERM"), "dumb"));

    struct criterion_options *opt = &criterion_options;
    if (env_always_succeed)
        opt->always_succeed    = !strcmp("1", env_always_succeed);
    if (env_fail_fast)
        opt->fail_fast         = !strcmp("1", env_fail_fast);
    if (env_use_ascii) {
        fprintf(stderr, "CRITERION_USE_ASCII is deprecated. Use LANG/LC_* "
                "variables to control encoding, and TERM/--color to control "
                "colors.\n");
        opt->use_ascii         = !strcmp("1", env_use_ascii) || is_term_dumb;
    }
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

    /* CRITERION_ENABLE_TAP backward compatibility.
       The environment variable is otherwise deprecated. */
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

            quiet = must_be_quiet(path);
            criterion_add_output(provider, path);
        }
        free(out);
    }

    criterion_options.executable_name = argv[0];

    for (int c; (c = getopt_long(argc, argv, "hvlfj:SqO:wt:", opts, NULL)) != -1;) {
        switch (c) {
            case 'b': criterion_options.logging_threshold = (enum criterion_logging_level) atou(DEF(optarg, "1")); break;
            case 'y': criterion_options.always_succeed    = true; break;
            case 'z': fprintf(stderr, "--no-early-exit is now deprecated as it no longer does anything.\n"); break;
            case 'k':
                fprintf(stderr, "--ascii is deprecated. Use LANG/LC_* "
                        "variables to control encoding, and TERM/--color to control "
                        "colors.\n");
                criterion_options.use_ascii = true;
                break;
            case 'j': criterion_options.jobs              = atou(optarg); break;
            case 'f': criterion_options.fail_fast         = true; break;
            case 'S': criterion_options.short_filename    = true; break;

            case 'p':
                fprintf(stderr, "--pattern has been renamed as --filter and is now deprecated.\n");
            /* fallthrough */
            case 'F': criterion_options.pattern           = optarg; break;
            case 'q': quiet = true; break;

            case 't': criterion_options.timeout = atof(optarg); break;

            case 'd':
                if (!parse_dbg(optarg))
                    exit(3);
                break;
            case 'D':
                if (!parse_dbg_transport(optarg))
                    exit(3);
                break;

                /* *INDENT-OFF* - Duff devices are often mishandled by formatters */
            {
                const char *provider;
            case 'T': provider = "tap";  goto provider_def;
            case 'x': provider = "xml";  goto provider_def;
            case 'n': provider = "json"; goto provider_def;

            provider_def: {}
                const char *path = DEF(optarg, "-");
                quiet = must_be_quiet(path);
                criterion_add_output(provider, path);
            } break;
                /* *INDENT-ON* */

            case 'l': do_list_tests = true; break;
            case 'v': do_print_version = true; break;
            case 'h': do_print_usage = true; break;
            case 'O': {
                char *arg = strdup(optarg);
                char *buf = NULL;
                strtok_r(arg, ":", &buf);

                char *path = strtok_r(NULL, ":", &buf);
                if (arg == NULL || path == NULL) {
                    do_print_usage = true;
                    break;
                }

                quiet = must_be_quiet(path);
                criterion_add_output(arg, path);
            } break;
            case 'w': criterion_options.wait_for_clients = true; break;
            case 's':
                fprintf(stderr, "--single has been removed. Use --debug instead.");
                exit(3);
            case 'c': criterion_options.crash = true; break;
            case 'U': criterion_options.full_stats = true; break;
            case 'C': criterion_options.color = deduce_color(optarg); break;
            case 'e': set_encoding(optarg); break;
            case 'N': criterion_options.ignore_warnings = true; break;
            case '?':
            default: do_print_usage = handle_unknown_arg; break;
        }
    }

end:
    if (quiet)
        criterion_options.logging_threshold = CRITERION_LOG_LEVEL_QUIET;

    if (criterion_options.use_ascii) {
        strcpy(criterion_options.encoding, "ANSI_X3.4-1968");
        criterion_options.color = false;
    }

    if (do_print_usage)
        return print_usage(argv[0]);
    if (do_print_version)
        return print_version();
    if (do_list_tests)
        return list_tests();

    return 1;
}
