#define _GNU_SOURCE
#include <criterion/criterion.h>
#include <criterion/options.h>
#include <stdio.h>
#include <getopt.h>

# define VERSION "v1.0.0"
# define VERSION_MSG "Tests compiled with Criterion " VERSION "\n"

# define USAGE                                              \
    VERSION_MSG "\n"                                        \
    "usage: %s OPTIONS\n"                                   \
    "options: \n"                                           \
    "    -h or --help: prints this message\n"               \
    "    -v or --version: prints the version of criterion " \
            "these tests have been linked against\n"        \
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

int main(int argc, char *argv[]) {
    static struct option opts[] = {
        {"verbose",         optional_argument,  0, 'b'},
        {"version",         no_argument,        0, 'v'},
        {"tap",             no_argument,        0, 't'},
        {"help",            no_argument,        0, 'h'},
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

    for (int c; (c = getopt_long(argc, argv, "hv", opts, NULL)) != -1;) {
        switch (c) {
            case 'b': criterion_options.logging_threshold = atoi(optarg ?: "1"); break;
            case 't': criterion_options.enable_tap_format = true; break;
            case 'y': criterion_options.always_succeed    = true; break;
            case 'z': criterion_options.no_early_exit     = true; break;
            case 'v': return print_version();
            case 'h':
            default : return print_usage(argv[0]);
        }
    }

    return !criterion_run_all_tests();
}
