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
#ifndef CRITERION_PARAMETERIZED_H_
# define CRITERION_PARAMETERIZED_H_

# include "alloc.h"
# include "assert.h"
# include "internal/parameterized.h"

/**
 *  ParameterizedTest(Type *param, Suite, Name, [Options...]) { Function Body }
 *
 *  Defines a new parameterized test.
 *
 *  A parameterized test only takes one parameter -- to pass multiple parameters,
 *  use a structure type.
 *
 *  @param Type    The type of the parameter.
 *  @param Suite   The name of the test suite containing this test.
 *  @param Name    The name of the test.
 *  @param Options An optional sequence of designated initializer key/value
 *    pairs as described in the `criterion_test_extra_data` structure
 *    (see criterion/types.h).
 *    Example: .exit_code = 1
 */
# define ParameterizedTest(...) CR_EXPAND(CR_PARAM_TEST_BASE(__VA_ARGS__, .sentinel_ = 0))

/**
 *  ParameterizedTestParameters(Suite, Test) { Function Body }
 *
 *  Defines the parameter generator for the associated parameterized test.
 *
 *  @param Suite   The name of the test suite containing the test.
 *  @param Test    The name of the test.
 *  @returns A constructed instance of criterion::parameters, or the result of
 *    the cr_make_param_array macro.
 */
# define ParameterizedTestParameters(Suite, Name) CR_PARAM_TEST_PARAMS(Suite, Name)

/**
 *  cr_make_param_array(Type, Array, Len, [Cleanup]);
 *
 *  Constructs a parameter list used as a return value for a parameter generator.
 *
 *  @param Type     The type of the array subscript.
 *  @param Array    The array of parameters.
 *  @param Len      The length of the array.
 *  @param Cleanup  The optional cleanup function for the array.
 *  @returns The parameter list.
 */
# define cr_make_param_array(...) CR_EXPAND(cr_make_param_array_(__VA_ARGS__))

# ifdef __cplusplus
#  include <vector>

namespace criterion {
    template <typename T>
    using parameters = std::vector<T, criterion::allocator<T>>;
}
# endif

#endif /* !CRITERION_PARAMETERIZED_H_ */
