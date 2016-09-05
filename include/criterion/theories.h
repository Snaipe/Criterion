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
 * @brief Theory tests
 *****************************************************************************/
#ifndef CRITERION_THEORIES_H_
#define CRITERION_THEORIES_H_

#include "criterion.h"

CR_BEGIN_C_API

/**
 *  Aborts the current theory iteration.
 *  This function does not return.
 */
CR_API void cr_theory_abort(void);

CR_END_C_API

/**
 * @defgroup TheoryBase Theory and datapoint macros
 * @{
 */

/**
 *  Defines a new theory test.
 *
 *  The parameters are selected from a cartesian product defined by a
 *  TheoryDataPoints macro.
 *
 *  Example:
 *  @code{.c}
 *  Theory((int arg0, double arg1), suite, test) {
 *      // function body
 *  };
 *  @endcode
 *
 *  @param Params  A list of function parameters.
 *  @param Suite   The name of the test suite containing this test.
 *  @param Name    The name of the test.
 *  @param ...     An optional sequence of designated initializer key/value
 *    pairs as described in the `criterion_test_extra_data` structure
 *    (see criterion/types.h).
 *    Example: .exit_code = 1
 */
#define Theory(Params, Suite, Name, ...)    internal

/**
 *  Defines an array of data points.
 *
 *  The types of the specified data points *must* match the types of the
 *  associated theory.
 *
 *  Each entry in the array must be the result of the `DataPoints` macro.
 *
 *  Example:
 *  @code{.c}
 *  TheoryDataPoints(suite, test) = {
 *      DataPoints(int, 1, 2, 3),               // first theory parameter
 *      DataPoints(double, 4.2, 0, -INFINITY),  // second theory parameter
 *  };
 *  @endcode
 *
 *  @param Suite   The name of the test suite containing this test.
 *  @param Name    The name of the test.
 */
#define TheoryDataPoints(Suite, Name)       CR_TH_INTERNAL_TDPS(Suite, Name)

/**
 *  Defines a new set of data points.
 *
 *  @param Type The type of each data point in the set.
 *  @param ...  The data points in the set.
 */
#define DataPoints(Type, ...)               CR_EXPAND(CR_TH_INTERNAL_DP(Type, __VA_ARGS__))

/**@}*/

/**
 * @defgroup TheoryInvariants Theory invariants
 * @{
 */

/**
 * Assumes `Condition` is true
 *
 * Evaluates `Condition` and continues execution if it is true.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Condition Condition to test
 *
 *****************************************************************************/
#define cr_assume(Condition)    \
    do {                        \
        if (!(Condition))       \
            cr_theory_abort();  \
    } while (0)

/**
 * Assumes `Condition` is false
 *
 * Evaluates `Condition` and continues execution if it is false.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Condition Condition to test
 *
 *****************************************************************************/
#define cr_assume_not(Condition)             cr_assume(!(Condition))

/**
 * Assumes `Actual` is equal to `Expected`
 *
 * Continues execution if `Actual` is equal to `Expected`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Expected Expected value
 *
 *****************************************************************************/
#define cr_assume_eq(Actual, Expected)       cr_assume_op_(==, Actual, Expected)

/**
 * Assumes `Actual` is not equal to `Unexpected`
 *
 * Continues execution if `Actual` is not equal to `Unexpected`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Unexpected Unexpected value
 *
 *****************************************************************************/
#define cr_assume_neq(Actual, Unexpected)    cr_assume_op_(!=, Actual, Unexpected)

/**
 * Assumes `Actual` is greater than `Reference`
 *
 * Continues execution if `Actual` is greater than `Reference`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 *
 *****************************************************************************/
#define cr_assume_gt(Actual, Reference)      cr_assume_op_(>, Actual, Reference)

/**
 * Assumes `Actual` is greater or equal to `Reference`
 *
 * Continues execution if `Actual` is greater or equal to `Reference`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 *
 *****************************************************************************/
#define cr_assume_geq(Actual, Reference)     cr_assume_op_(>=, Actual, Reference)

/**
 * Assumes `Actual` is less than `Reference`
 *
 * Continues execution if `Actual` is less than `Reference`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 *
 *****************************************************************************/
#define cr_assume_lt(Actual, Reference)      cr_assume_op_(<, Actual, Reference)

/**
 * Assumes `Actual` is less or equal to `Reference`
 *
 * Continues execution if `Actual` is less or equal to `Reference`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 *
 *****************************************************************************/
#define cr_assume_leq(Actual, Reference)     cr_assume_op_(<=, Actual, Reference)

/**
 * Assumes `Value` is NULL.
 *
 * Continues execution if `Value` is NULL.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Value Value to test
 *
 *****************************************************************************/
#define cr_assume_null(Value)                cr_assume_eq(Value, NULL)

/**
 * Assumes `Value` is not NULL.
 *
 * Continues execution if `Value` is not NULL.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Value Value to test
 *
 *****************************************************************************/
#define cr_assume_not_null(Value)            cr_assume_neq(Value, NULL)

/**
 * Assumes `Actual` is equal to `Expected` with a tolerance of `Epsilon`
 *
 * Continues execution if `Actual` is equal to `Expected` with a tolerance of Epsilon.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @note Use this to test equality between floats
 *
 * @param[in] Actual Value to test
 * @param[in] Expected Expected value
 * @param[in] Epsilon Tolerance between Actual and Expected
 *
 *****************************************************************************/
#define cr_assume_float_eq(Actual, Expected, Epsilon) \
    cr_assume((Expected) - (Actual) <= (Epsilon)      \
            && (Actual) - (Expected) <= (Epsilon))

/**
 * Assumes `Actual` is not equal to `Expected` with a tolerance of `Epsilon`
 *
 * Continues execution if `Actual` is not equal to `Expected` with a tolerance of Epsilon.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @note Use this to test equality between floats
 *
 * @param[in] Actual Value to test
 * @param[in] Expected Expected value
 * @param[in] Epsilon Tolerance between Actual and Expected
 *
 *****************************************************************************/
#define cr_assume_float_neq(Actual, Expected, Epsilon) \
    cr_assume((Expected) - (Actual) > (Epsilon)        \
            || (Actual) - (Expected) > (Epsilon))

/**
 * Assumes `Actual` is lexicographically equal to `Expected`
 *
 * Continues execution if `Actual` is lexicographically equal to `Expected`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual String to test
 * @param[in] Expected Expected string
 *
 *****************************************************************************/
#define cr_assume_str_eq(Actual, Expected)             cr_assume_str_op_(==, Actual, Expected)

/**
 * Assumes `Actual` is not lexicographically equal to `Unexpected`
 *
 * Continues execution if `Actual` is not lexicographically equal to `Unexpected`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual String to test
 * @param[in] Unexpected Unexpected string
 *
 *****************************************************************************/
#define cr_assume_str_neq(Actual, Unexpected)          cr_assume_str_op_(!=, Actual, Unexpected)

/**
 * Assumes `Actual` is lexicographically less than `Reference`
 *
 * Continues execution if `Actual` is lexicographically less than `Reference`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 *
 *****************************************************************************/
#define cr_assume_str_lt(Actual, Reference)            cr_assume_str_op_(<, Actual, Reference)

/**
 * Assumes `Actual` is lexicographically less or equal to `Reference`
 *
 * Continues execution if `Actual` is lexicographically less or equal to `Reference`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 *
 *****************************************************************************/
#define cr_assume_str_leq(Actual, Reference)           cr_assume_str_op_(<=, Actual, Reference)

/**
 * Assumes `Actual` is lexicographically greater than `Reference`
 *
 * Continues execution if `Actual` is lexicographically greater than `Reference`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 *
 *****************************************************************************/
#define cr_assume_str_gt(Actual, Reference)            cr_assume_str_op_(>, Actual, Reference)

/**
 * Assumes `Actual` is lexicographically greater or equal to `Reference`
 *
 * Continues execution if `Actual` is lexicographically greater or equal to `Reference`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @param[in] Actual Value to test
 * @param[in] Reference Reference value
 *
 *****************************************************************************/
#define cr_assume_str_geq(Actual, Reference)           cr_assume_str_op_(>=, Actual, Reference)

/**
 * Assumes `Actual` is byte-to-byte equal to `Expected`
 *
 * Continues execution if `Actual` is byte-to-byte equal to `Expected`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @warning This should not be used on struct arrays
 *
 * @param[in] Actual Array to test
 * @param[in] Expected Expected array
 * @param[in] Size The size of both arrays
 *
 *****************************************************************************/
#define cr_assume_arr_eq(Actual, Expected, Size)       cr_assume(!memcmp((Actual), (Expected), (Size)))
/**
 * Assumes `Actual` is not byte-to-byte equal to `Unexpected`
 *
 * Continues execution if `Actual` is not byte-to-byte equal to `Unexpected`.
 * Otherwise the current theory iteration aborts without marking the test as
 * failure.
 *
 * @warning This should not be used on struct arrays
 *
 * @param[in] Actual Array to test
 * @param[in] Unexpected Unexpected array
 * @param[in] Size The size of both arrays
 *
 *****************************************************************************/
#define cr_assume_arr_neq(Actual, Unexpected, Size)    cr_assume(memcmp((Actual), (Unexpected), (Size)))

/**@}*/

/* Deprecated */

#ifndef CRITERION_NO_COMPAT
# define cr_assume_strings_eq(...)                     CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_eq, cr_assume_str_eq) cr_assume_str_eq(__VA_ARGS__)
# define cr_assume_strings_neq(...)                    CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_neq, cr_assume_str_neq) cr_assume_str_neq(__VA_ARGS__)
# define cr_assume_strings_lt(...)                     CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_lt, cr_assume_str_lt) cr_assume_str_lt(__VA_ARGS__)
# define cr_assume_strings_leq(...)                    CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_leq, cr_assume_str_leq) cr_assume_str_leq(__VA_ARGS__)
# define cr_assume_strings_gt(...)                     CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_gt, cr_assume_str_gt) cr_assume_str_gt(__VA_ARGS__)
# define cr_assume_strings_geq(...)                    CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_geq, cr_assume_str_geq) cr_assume_str_geq(__VA_ARGS__)

# define cr_assume_arrays_eq(...)                      CRITERION_ASSERT_DEPRECATED_B(cr_assume_arrays_eq, cr_assume_arr_eq) cr_assume_arr_eq(__VA_ARGS__)
# define cr_assume_arrays_neq(...)                     CRITERION_ASSERT_DEPRECATED_B(cr_assume_arrays_neq, cr_assume_arr_neq) cr_assume_arr_neq(__VA_ARGS__)
#endif

#include "internal/theories.h"

#endif /* !CRITERION_THEORIES_H_ */
