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
#ifndef CRITERION_OPTIONS_H_
# define CRITERION_OPTIONS_H_

# include <stdbool.h>
# include "logging.h"

struct criterion_options {

    /**
     *  The current logging threshold.
     *
     *  default: 1
     */
    enum criterion_logging_level logging_threshold;

    /**
     *  The logger that will be used during the execution of the runner.
     *
     *  default: normal logger
     */
    struct criterion_logger *logger;

    /**
     *  Don't exit the child immediately after finishing to run the test
     *  function, and perform a full cleanup.
     *
     *  Useful when tracking memory leaks, and is immediately implied when
     *  running the process under valgrind.
     *
     *  default: false
     */
    bool no_early_exit;

    /**
     *  Always return a success from criterion_run_all_tests.
     *
     *  default: false
     */
    bool always_succeed;

    /**
     *  Disable unicode and ansi coloring from the logging system.
     *
     *  default: false
     */
    bool use_ascii;

    /**
     *  Exit immediately after the first test failure.
     *
     *  default: false
     */
    bool fail_fast;

    /**
     *  Disable all tests not matching this extglob pattern.
     *  if NULL, don't filter tests.
     *
     *  default: NULL
     */
    const char *pattern;

    /**
     *  Only print the base file name compound of the source file containing
     *  the tests during reporting.
     *
     *  default: false
     */
    bool short_filename;

    /**
     *  The maximum number of parallel jobs that the test runner will spawn.
     *  0 means that this number shall be the number of cores on your system.
     *
     *  default: 0
     */
    size_t jobs;

    /**
     *  Measure and report times.
     *
     *  default: true
     */
    bool measure_time;
};

CR_BEGIN_C_API

/**
 *  The runtime options for the test runner.
 */
extern struct criterion_options criterion_options;

CR_END_C_API

#endif /*!CRITERION_OPTIONS_H_ */
