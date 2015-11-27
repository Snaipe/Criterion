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
#ifndef CRITERION_TYPES_H_
# define CRITERION_TYPES_H_

# include "alloc.h"
# ifdef __cplusplus
#  include <cstddef>
#  include <vector>
using std::size_t;
# else
#  include <stdbool.h>
#  include <stddef.h>
# endif
# include "internal/common.h"

/**
 *  Enumerates the supported languages for tests
 */
enum criterion_language {
    CR_LANG_C,              /// C
    CR_LANG_CXX,            /// C++
    CR_LANG_OBJC,           /// Objective-C
    CR_LANG_OBJCXX,         /// Objective-C++

    CR_LANG_SIZE_ // leave this at the end
};

extern const char *const cr_language_names[CR_LANG_SIZE_];

/**
 *  Enumerates the supported kinds of tests
 */
enum criterion_test_kind {
    CR_TEST_NORMAL,
    CR_TEST_PARAMETERIZED,
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
    // Start of private API
    /*
     * Warning: the fields below are not meant to be set manually.
     *  Setting them improperly *will* wreck havock in your tests.
     *
     * You've been warned.
     */
    int sentinel_;
    enum criterion_language lang_;
    enum criterion_test_kind kind_;
    struct criterion_test_params (*param_)(void);
    const char *identifier_;
    const char *file_;
    unsigned line_;
    // Enf of private API

    void (*init)(void);         /// The setup test fixture
    void (*fini)(void);         /// The setup test fixture
    int signal;                 /// The expected signal raised by the test (or 0 if none)
    int exit_code;              /// The expected exit code returned by the test
    bool disabled;              /// Whether the test is disabled or not
    const char *description;    /// The description of a test
    double timeout;             /// A timeout for the test in seconds
    void *data;                 /// Extra user data
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
