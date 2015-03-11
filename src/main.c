#define _GNU_SOURCE
#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/options.h>
#include <stdio.h>
#include <getopt.h>

# define USAGE                                          \
    "usage: %s OPTIONS\n"                               \
    "options: \n"                                       \
    "    -h or --help: prints this message\n"           \
    "    --verbose [level]: sets verbosity to level\n"

int print_usage(char *progname) {
    fprintf(stderr, USAGE, progname);
    return 0;
}

int main(int argc, char *argv[]) {
    static struct option opts[] = {
        {"verbose",         optional_argument,  0, 'v'},
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

    for (int c; (c = getopt_long(argc, argv, "h", opts, NULL)) != -1;) {
        switch (c) {
            case 'v': criterion_options.logging_threshold = atoi(optarg ?: "1"); break;
            case 't': criterion_options.enable_tap_format = true; break;
            case 'y': criterion_options.always_succeed    = true; break;
            case 'z': criterion_options.no_early_exit     = true; break;
            case 'h':
            default : return print_usage(argv[0]);
        }
    }

    return criterion_run_all_tests();
}
