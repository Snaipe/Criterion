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
        .always_succeed    = strcmp("1", getenv("CRITERION_ALWAYS_SUCCEED") ?: "0"),
        .no_early_exit     = strcmp("1", getenv("CRITERION_NO_EARLY_EXIT")  ?: "0"),
        .enable_tap_format = strcmp("1", getenv("CRITERION_ENABLE_TAP")     ?: "0"),
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
