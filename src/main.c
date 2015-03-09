#define _GNU_SOURCE
#include <criterion/criterion.h>
#include <criterion/logging.h>
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
        {"verbose", optional_argument,  0, 'v'},
        {"help",    no_argument,        0, 'h'},
        {0,         0,                  0,  0 }
    };

    for (int c; (c = getopt_long(argc, argv, "h", opts, NULL)) != -1;) {
        switch (c) {
            case 'v': logging_threshold = atoi(optarg ?: "1"); break;
            case 'h':
            default : return print_usage(argv[0]);
        }
    }

    return criterion_run_all_tests();
}
