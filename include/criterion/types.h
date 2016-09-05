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
/*!
 * @file
 * @brief Types for tests
 *****************************************************************************/
#ifndef CRITERION_TYPES_H_
#define CRITERION_TYPES_H_

#include "alloc.h"
#ifdef __cplusplus
# include <cstddef>
# include <vector>
using std::size_t;
#else
# include <stdbool.h>
# include <stddef.h>
#endif
#include "internal/common.h"

/**
 *  Enumerates the supported languages for tests
 */
enum criterion_language {
    CR_LANG_C,      /* !< C */
    CR_LANG_CXX,    /* !< C++ */
    CR_LANG_OBJC,   /* !< Objective-C */
    CR_LANG_OBJCXX, /* !< Objective-C++ */

    CR_LANG_SIZE_   /* leave this at the end */
};

/**
 *  Enumerates the supported kinds of tests
 */
enum criterion_test_kind {
    CR_TEST_NORMAL,
    CR_TEST_PARAMETERIZED,
};

enum criterion_compiler {
    CR_COMP_UNKNOWN,
    CR_COMP_GCC,
    CR_COMP_CLANG,
    CR_COMP_MSVC,
};

/**
 *  Represents a set of parameters for a parameterized test.
 */
struct criterion_test_params;

/**
 *  Contains all the options that can be set for a test, through
 *  the Test and TestSuite macros, or other means.
 */
struct criterion_test_extra_data {
    /* / @cond CRITERION_TEST_EXTRA_DATA_PRIVATE_API
       Start of private API */
    /*
     * Warning: the fields below are not meant to be set manually.
     *  Setting them improperly *will* wreck havock in your tests.
     *
     * You've been warned.
     */
    int sentinel_;
    enum criterion_compiler compiler_;
    enum criterion_language lang_;
    enum criterion_test_kind kind_;
    struct criterion_test_params (*param_)(void);
    const char *identifier_;
    const char *file_;
    unsigned line_;
    /* Enf of private API
       / @endcond */

    /**
     * The setup test fixture.
     *
     * This function, if provided, will be executed during the initialization
     * of the test.
     */
    void (*init)(void);

    /**
     * The teardown test fixture.
     *
     * This function, if provided, will be executed during the finalization
     * of the test.
     */
    void (*fini)(void);

    /**
     * The expected signal to be raised by the test.
     *
     * If the test does not raise the specified signal, then the test is
     * marked as failed.
     *
     * A value of 0 means that is it not expected for the test to raise any
     * signal.
     */
    int signal;

    /**
     * The expected exit status to be returned by the test.
     *
     * By default, criterion exits the test process with a value of 0. If it
     * is expected for the test to exit with a non-zero status, this option
     * can be used.
     */
    int exit_code;

    /**
     * If `true`, skips the test.
     *
     * The test will still appear in the test list, but will be marked as
     * skipped and will not be executed.
     */
    bool disabled;

    /**
     * The long description of a test.
     *
     * If a description is provided, it will be printed in test reports, and
     * logged if the runner runs in verbose mode.
     */
    const char *description;

    /**
     * The timeout for the test, in seconds.
     *
     * If the realtime execution of a test takes longer than the specified
     * value, then the test is immediately aborted and reported as timing out.
     *
     * A value of `0` is equivalent to `+INFINITY` and means that the test
     * does not timeout.
     *
     * It is unspecified behaviour for the value of `timeout` to be negative
     * or `NaN`.
     */
    double timeout;

    /**
     * Extra user data.
     *
     * This field is currently unused.
     */
    void *data;
};

/**
 *  Represents a test
 */
struct criterion_test {
    const char *name;
    const char *category;
    void (*test)(void);
    struct criterion_test_extra_data *data;
};

/**
 *  Represents a test suite
 */
struct criterion_suite {
    const char *name;
    struct criterion_test_extra_data *data;
};

struct criterion_ordered_set;

struct criterion_suite_set {
    struct criterion_suite suite;
    struct criterion_ordered_set *tests;
};

struct criterion_test_set {
    struct criterion_ordered_set *suites;
    size_t tests;
};

#endif /* !CRITERION_TYPES_H_ */
