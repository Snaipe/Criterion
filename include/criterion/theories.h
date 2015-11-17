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

# ifdef __cplusplus
#  include <cstddef>
using std::size_t;
# else
#  include <stddef.h>
# endif

# include "criterion.h"

# ifdef __cplusplus
template <typename... T>
constexpr size_t criterion_va_num__(const T &...) {
    return sizeof...(T);
}
# endif

CR_BEGIN_C_API

CR_API void cr_theory_abort(void);

# define TheoryDataPoints(Category, Name) \
    static struct criterion_datapoints CR_IDENTIFIER_(Category, Name, dps)[]

# define TheoryDataPoint(Category, Name) \
    (CR_IDENTIFIER_(Category, Name, dps))

# ifdef __cplusplus
#  define CR_TH_VA_NUM(Type, ...)     criterion_va_num__(__VA_ARGS__)
#  define CR_TH_TEMP_ARRAY(Type, ...) []() { static Type arr[] = { __VA_ARGS__ }; return &arr; }()
# else
#  define CR_TH_VA_NUM(Type, ...) sizeof ((Type[]) { __VA_ARGS__ }) / sizeof (Type)
#  define CR_TH_TEMP_ARRAY(Type, ...) &(Type[]) { __VA_ARGS__ }
# endif

# define DataPoints(Type, ...) { \
        sizeof (Type), \
        CR_EXPAND(CR_TH_VA_NUM(Type, __VA_ARGS__)), \
        #Type, \
        CR_EXPAND(CR_TH_TEMP_ARRAY(Type, __VA_ARGS__)), \
    }

struct criterion_datapoints {
    size_t size;
    size_t len;
    const char *name;
    void *arr;
};

# define CR_NB_DATAPOINTS(Var) \
    (sizeof (Var) / sizeof (struct criterion_datapoints))

# define cr_assume(Condition) \
    do { \
        if (!(Condition)) \
            cr_theory_abort(); \
    } while (0);

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

CR_API void cr_theory_main(struct criterion_datapoints *dps, size_t datapoints, void (*fnptr)(void));

# define CR_VAARG_ID(Suffix, Category, Name, ...) \
    CR_IDENTIFIER_(Category, Name, Suffix)

# define Theory(Args, ...)                                                      \
    void CR_EXPAND(CR_VAARG_ID(theory, __VA_ARGS__,))Args;                      \
    CR_EXPAND(Test_(__VA_ARGS__, .sentinel_ = 0)) {                             \
        cr_theory_main(                                                         \
                CR_EXPAND(CR_VAARG_ID(dps, __VA_ARGS__,)),                      \
                CR_NB_DATAPOINTS(CR_EXPAND(CR_VAARG_ID(dps, __VA_ARGS__,))),    \
                (void(*)(void)) CR_EXPAND(CR_VAARG_ID(theory, __VA_ARGS__,))    \
            );                                                                  \
    }                                                                           \
    void CR_EXPAND(CR_VAARG_ID(theory, __VA_ARGS__,))Args

CR_END_C_API

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
