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
/**
 * @file
 * @brief criterion options
 *****************************************************************************/
#ifndef CRITERION_OPTIONS_H_
#define CRITERION_OPTIONS_H_

#include <stdbool.h>
#include "logging.h"
#include "internal/common.h"

enum criterion_debugger {
    /**
     *  Do not run the underlying test in a debugger
     */
    CR_DBG_NONE,

    /**
     *  Run the test suspended, without a debugger, and print its PID.
     *
     *  Allows external debuggers to attach.
     */
    CR_DBG_IDLE,

    /**
     *  Run the test with a debugging server compatible with the compiler
     *  it was built with.
     */
    CR_DBG_NATIVE,

    /**
     *  Run the test with gdbserver
     */
    CR_DBG_GDB,

    /**
     *  Run the test with lldb-server
     */
    CR_DBG_LLDB,

    /**
     *  Run the test with windbg in server mode
     */
    CR_DBG_WINDBG,
};

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
     * This option doesn't do anything and is deprecated.
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
     *  This is deprecated. Set the color and the encoding fields instead.
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

    /**
     *  Whether criterion should wait for incoming connections in server mode
     *
     *  default: false
     */
    bool wait_for_clients;

    /**
     * Raise a debug trap to crash the test if an assert fails so that a
     * debugger can gain control.
     *
     * default: false
     */
    bool crash;

    /**
     *  Whether criterion should run its tests in a debugging server.
     *
     *  The server hangs until a connection from a debugger gets accepted.
     *
     *  This forces jobs = 1 and crash = true.
     *
     *  default: CR_DBG_NONE;
     */
    enum criterion_debugger debug;

    /**
     *  The TCP port of the debugging server.
     *
     *  default: 1234
     */
    unsigned debug_port;

    /**
     *  The default timeout for each test when none is specified, in seconds.
     *
     *  If the value is non-positive, no timeout is applied.
     *
     *  default: 0
     */
    double timeout;

    /**
     * Fully report statistics from test workers, including those that are
     * not reported by default (like passing assertions).
     *
     * Reporting everything leads to more accurate reports at the cost of
     * execution speed.
     *
     * default: false
     */
    bool full_stats;

    /**
     * The output encoding to assume.
     *
     * This is used to know whether we should use special characters to
     * make the output prettier.
     *
     * default: locale-deduced, or ANSI_X3.4-1968.
     */
    char encoding[64];

    /**
     * Whether the output should be colorized or not.
     *
     * default: terminal-deduced.
     */
    bool color;

    /**
     * When false, warnings will make criterion exit with a nonzero
     * exit status (e.g. when a test crashes during its teardown).
     *
     * default: false
     */
    bool ignore_warnings;

    const char *executable_name;
};

CR_BEGIN_C_API

/**
 *  The runtime options for the test runner.
 */
CR_API extern struct criterion_options criterion_options;

CR_END_C_API

#endif /*!CRITERION_OPTIONS_H_ */
