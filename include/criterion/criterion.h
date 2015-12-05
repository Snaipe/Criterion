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
#ifndef CRITERION_H_
# define CRITERION_H_

# include "types.h"
# include "assert.h"
# include "alloc.h"

# include "internal/test.h"

/**
 *  Test(Suite, Name, [Options...]) { Function body }
 *
 *  Defines a new test.
 *
 *  @param Suite   The name of the test suite containing this test.
 *  @param Name    The name of the test.
 *  @param Options An optional sequence of designated initializer key/value
 *    pairs as described in the `criterion_test_extra_data` structure
 *    (see criterion/types.h).
 *    Example: .exit_code = 1
 */
# define Test(...) CR_EXPAND(CR_TEST_BASE(__VA_ARGS__, .sentinel_ = 0))

/**
 *  TestSuite(Name, [Options...]);
 *
 *  Explicitely defines a test suite and its options.
 *
 *  @param Name    The name of the test suite.
 *  @param Options An optional sequence of designated initializer key/value
 *    pairs as described in the `criterion_test_extra_data` structure
 *    (see criterion/types.h).
 *    These options will provide the defaults for each test.
 */
# define TestSuite(...) CR_EXPAND(CR_SUITE_BASE(__VA_ARGS__, .sentinel_ = 0))

CR_BEGIN_C_API

/**
 *  Initializes criterion and builds a set of all discovered tests.
 *
 *  Using any of the functions and macros provided by criterion before calling
 *  this results in undefined behaviour.
 *
 *  @returns the set of tests
 */
CR_API struct criterion_test_set *criterion_initialize(void);

/**
 *  Release all resources allocated by criterion.
 *
 *  Using any of the functions and macros provided by criterion except
 *  criterion_initialize after this function is called results in undefined
 *  behaviour.
 */
CR_API void criterion_finalize(struct criterion_test_set *tests);

/**
 *  Run all the tests in the test set.
 *
 *  @param[in] tests The set of tests that are to be executed.
 *
 *  @returns 1 if all tests succeeded or criterion_options.always_succeed
 *    is true, 0 otherwise.
 */
CR_API int criterion_run_all_tests(struct criterion_test_set *tests);

/**
 *  Handles all default command-line parameters, as documented in:
 *  <http://criterion.readthedocs.org/en/latest/env.html>, and appropriately
 *  sets criterion_options.
 *
 *  @param[in] argc The number of arguments in argv.
 *  @param[in] argv A null-terminated array of strings representing the arguments.
 *  @param[in] handle_unknown_arg Whether the function should print a message
 *    and exit when an unknown parameter is encountered. Use false if you want
 *    to handle additional parameters yourself.
 *
 *  @returns 0 if the process should exit immediately after, for instance after
 *    printing the help message.
 */
CR_API int criterion_handle_args(int argc, char *argv[], bool handle_unknown_arg);

/**
 *  Manually registers a new test within the specified test set.
 *
 *  @param[in] tests The set of tests you want to insert the test in.
 *  @param[in] test  The newly created test.
 */
CR_API void criterion_register_test(struct criterion_test_set *tests,
                                    struct criterion_test *test);

extern const struct criterion_test  *const criterion_current_test;
extern const struct criterion_suite *const criterion_current_suite;

CR_END_C_API

#endif /* !CRITERION_H_ */
