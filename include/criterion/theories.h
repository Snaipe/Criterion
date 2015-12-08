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
#ifndef CRITERION_THEORIES_H_
# define CRITERION_THEORIES_H_

# include "criterion.h"
# include "internal/theories.h"

CR_BEGIN_C_API

/**
 *  Aborts the current theory iteration.
 *  This function does not return.
 */
CR_API void cr_theory_abort(void);

CR_END_C_API

// Theory and datapoint macros

/**
 *  Theory((Params...), Suite, Name, [Options...]) { Function Body }
 *
 *  Defines a new theory test.
 *
 *  The parameters are selected from a cartesian product defined by a
 *  TheoryDataPoints macro.
 *
 *  @param Params  A list of function parameters.
 *  @param Suite   The name of the test suite containing this test.
 *  @param Name    The name of the test.
 *  @param Options An optional sequence of designated initializer key/value
 *    pairs as described in the `criterion_test_extra_data` structure
 *    (see criterion/types.h).
 *    Example: .exit_code = 1
 */
# define Theory(Args, ...) CR_EXPAND(CR_THEORY_BASE(Args, __VA_ARGS__))

/**
 *  TheoryDataPoints(Suite, Name) = { Datapoints... };
 *
 *  Defines an array of data points.
 *
 *  The types of the specified data points *must* match the types of the
 *  associated theory.
 *
 *  Each entry in the array must be the result of the DataPoints macro.
 *
 *  @param Suite   The name of the test suite containing this test.
 *  @param Name    The name of the test.
 */
# define TheoryDataPoints(Category, Name) CR_TH_INTERNAL_TDPS(Category, Name)

/**
 *  DataPoints(Type, Values...)
 *
 *  Defines a new set of data points.
 *
 *  @param Type    The type of each data point in the set.
 *  @param Values  The data points in the set.
 */
# define DataPoints(Type, ...) CR_EXPAND(CR_TH_INTERNAL_DP(Type, __VA_ARGS__))

// Theory invariants

# define cr_assume(Condition) \
    do { \
        if (!(Condition)) \
            cr_theory_abort(); \
    } while (0)

# define cr_assume_not(Condition) cr_assume(!(Condition))

# define cr_assume_op_(Op, Actual, Expected) cr_assume((Actual) Op (Expected))
# define cr_assume_eq(Actual, Expected)  cr_assume_op_(==, Actual, Expected)
# define cr_assume_neq(Actual, Expected) cr_assume_op_(!=, Actual, Expected)
# define cr_assume_gt(Actual, Expected)  cr_assume_op_(>, Actual, Expected)
# define cr_assume_geq(Actual, Expected) cr_assume_op_(>=, Actual, Expected)
# define cr_assume_lt(Actual, Expected)  cr_assume_op_(<, Actual, Expected)
# define cr_assume_leq(Actual, Expected) cr_assume_op_(<=, Actual, Expected)

# define cr_assume_null(Value)     cr_assume_eq(Value, NULL)
# define cr_assume_not_null(Value) cr_assume_neq(Value, NULL)

# define cr_assume_float_eq(Actual, Expected, Epsilon)  \
    cr_assume((Expected) - (Actual) <= (Epsilon)        \
           && (Actual) - (Expected) <= (Epsilon))

# define cr_assume_float_neq(Actual, Expected, Epsilon) \
    cr_assume((Expected) - (Actual) > (Epsilon)         \
           || (Actual) - (Expected) > (Epsilon))

# define cr_assume_str_op_(Op, Actual, Expected) \
    cr_assume(strcmp((Actual), (Expected)) Op 0)

# define cr_assume_str_eq(Actual, Expected)  cr_assume_str_op_(==, Actual, Expected)
# define cr_assume_str_neq(Actual, Expected) cr_assume_str_op_(!=, Actual, Expected)
# define cr_assume_str_lt(Actual, Expected)  cr_assume_str_op_(<, Actual, Expected)
# define cr_assume_str_leq(Actual, Expected) cr_assume_str_op_(<=, Actual, Expected)
# define cr_assume_str_gt(Actual, Expected)  cr_assume_str_op_(>, Actual, Expected)
# define cr_assume_str_geq(Actual, Expected) cr_assume_str_op_(>=, Actual, Expected)

# define cr_assume_arr_eq(Actual, Expected, Size)  cr_assume(!memcmp((A), (B), (Size)))
# define cr_assume_arr_neq(Actual, Expected, Size) cr_assume(memcmp((A), (B), (Size)))

// Deprecated

# ifndef CRITERION_NO_COMPAT
#  define cr_assume_strings_eq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_eq, cr_assume_str_eq) cr_assume_str_eq(__VA_ARGS__)
#  define cr_assume_strings_neq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_neq, cr_assume_str_neq) cr_assume_str_neq(__VA_ARGS__)
#  define cr_assume_strings_lt(...) CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_lt, cr_assume_str_lt) cr_assume_str_lt(__VA_ARGS__)
#  define cr_assume_strings_leq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_leq, cr_assume_str_leq) cr_assume_str_leq(__VA_ARGS__)
#  define cr_assume_strings_gt(...) CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_gt, cr_assume_str_gt) cr_assume_str_gt(__VA_ARGS__)
#  define cr_assume_strings_geq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assume_strings_geq, cr_assume_str_geq) cr_assume_str_geq(__VA_ARGS__)

#  define cr_assume_arrays_eq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assume_arrays_eq, cr_assume_arr_eq) cr_assume_arr_eq(__VA_ARGS__)
#  define cr_assume_arrays_neq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assume_arrays_neq, cr_assume_arr_neq) cr_assume_arr_neq(__VA_ARGS__)
# endif

#endif /* !CRITERION_THEORIES_H_ */
