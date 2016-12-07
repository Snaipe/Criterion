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
 * @brief Parameterized tests
 *****************************************************************************/
#ifndef CRITERION_PARAMETERIZED_H_
#define CRITERION_PARAMETERIZED_H_

#include "alloc.h"
#include "assert.h"

/**
 * @defgroup ParameterizedBase Parameterized test & generator macros
 * @{
 */

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
 *  @param ...     An optional sequence of designated initializer key/value
 *    pairs as described in the `criterion_test_extra_data` structure
 *    (see criterion/types.h).
 *    Example: `.exit_code = 1`
 */
#define ParameterizedTest(Type, Suite, Name, ...)    internal

/**
 *  Defines the parameter generator prototype for the associated parameterized
 *  test.
 *
 *  @param Suite   The name of the test suite containing the test.
 *  @param Test    The name of the test.
 *  @returns A constructed instance of criterion::parameters, or the result of
 *    the cr_make_param_array macro.
 */
#define ParameterizedTestParameters(Suite, Name)    internal

/**
 *  Constructs a parameter list used as a return value for a parameter generator.
 *
 *  This is only recommended for C sources. For C++, use `criterion::parameters`
 *  or `criterion_test_params`.
 *
 *  @param Type     The type of the array subscript.
 *  @param Array    The array of parameters.
 *  @param Len      The length of the array.
 *  @param Cleanup  The optional cleanup function for the array.
 *  @returns The parameter list.
 */
#define cr_make_param_array(Type, Array, Len, Cleanup)    internal

/** @} */

#ifdef __cplusplus
# include <vector>

namespace criterion
{
/**
 *  Represents a C++ dynamic parameter list for a parameter generator.
 *
 *  @ingroup ParameterizedBase
 *
 *  @param T The type of the parameter.
 */
template <typename T>
using parameters = std::vector<T, criterion::allocator<T> >;
}
#endif

#include "internal/parameterized.h"

#endif /* !CRITERION_PARAMETERIZED_H_ */
