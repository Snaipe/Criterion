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
#ifndef CRITERION_ASSERT_H_
# define CRITERION_ASSERT_H_

# ifdef __cplusplus
#  include <algorithm>
# endif

# include "assert_base.h"

// Common binary assertions

# define cr_assert_op_(Fail, Op, Actual, Expected, ...)                     \
    CR_EXPAND(cr_assert_impl(                                               \
            Fail,                                                           \
            (Actual) Op (Expected),                                         \
            dummy,                                                          \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                \
            (CR_STR((Actual) Op (Expected))),                               \
            __VA_ARGS__                                                     \
    ))

# define cr_assert_op_va_(Fail, Op, ...)                                    \
    CR_EXPAND(cr_assert_op_(                                                \
            Fail,                                                           \
            Op,                                                             \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                            \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                             \
    ))

# define cr_assert_eq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,      ==, __VA_ARGS__))
# define cr_expect_eq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_,  ==, __VA_ARGS__))

# define cr_assert_neq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,     !=, __VA_ARGS__))
# define cr_expect_neq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

# define cr_assert_lt(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,      <, __VA_ARGS__))
# define cr_expect_lt(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_,  <, __VA_ARGS__))

# define cr_assert_leq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,     <=, __VA_ARGS__))
# define cr_expect_leq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))

# define cr_assert_gt(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,      >, __VA_ARGS__))
# define cr_expect_gt(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_,  >, __VA_ARGS__))

# define cr_assert_geq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_ABORT_,     >=, __VA_ARGS__))
# define cr_expect_geq(...) CR_EXPAND(cr_assert_op_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))

// Common unary assertions

# define cr_assert_null_op_(Fail, Op, Msg, Value, ...)  \
    CR_EXPAND(cr_assert_impl(                           \
            Fail,                                       \
            (Value) Op NULL,                            \
            dummy,                                      \
            Msg,                                        \
            (CR_STR(Value)),                            \
            __VA_ARGS__                                 \
    ))

# define cr_assert_null_op_va_(Fail, Op, Msg, ...)      \
    CR_EXPAND(cr_assert_null_op_(                       \
            Fail,                                       \
            Op,                                         \
            Msg,                                        \
            CR_VA_HEAD(__VA_ARGS__),                    \
            CR_VA_TAIL(__VA_ARGS__)                     \
    ))

# define cr_assert_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_ABORT_,     ==, CRITERION_ASSERT_MSG_IS_NOT_NULL, __VA_ARGS__))
# define cr_expect_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_CONTINUES_, ==, CRITERION_ASSERT_MSG_IS_NOT_NULL, __VA_ARGS__))

# define cr_assert_not_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_ABORT_,     !=, CRITERION_ASSERT_MSG_IS_NULL, __VA_ARGS__))
# define cr_expect_not_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_CONTINUES_, !=, CRITERION_ASSERT_MSG_IS_NULL, __VA_ARGS__))

// Floating-point assertions

# define cr_assert_float_eq_op_(Actual, Expected, Epsilon) \
    (Expected) - (Actual) <= (Epsilon) && (Actual) - (Expected) <= (Epsilon)

# define cr_assert_float_neq_op_(Actual, Expected, Epsilon) \
    (Expected) - (Actual) > (Epsilon) || (Actual) - (Expected) > (Epsilon)

# define cr_assert_float_op_(Fail, Op, Actual, Expected, Epsilon, ...)         \
    CR_EXPAND(cr_assert_impl(                                                  \
            Fail,                                                              \
            Op(Actual, Expected, Epsilon),                                     \
            dummy,                                                             \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                                   \
            (CR_STR(Op(Actual, Expected, Epsilon))),                           \
            __VA_ARGS__                                                        \
    ))

# define cr_assert_float_op_va_(Fail, Op, ...)                  \
    CR_EXPAND(cr_assert_float_op_(                              \
            Fail,                                               \
            Op,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                            \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),    \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))     \
    ))

# define cr_assert_float_eq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_ABORT_,     cr_assert_float_eq_op_, __VA_ARGS__))
# define cr_expect_float_eq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_CONTINUES_, cr_assert_float_eq_op_, __VA_ARGS__))

# define cr_assert_float_neq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_ABORT_,     cr_assert_float_neq_op_, __VA_ARGS__))
# define cr_expect_float_neq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_CONTINUES_, cr_assert_float_neq_op_, __VA_ARGS__))

// String assertions

# define cr_assert_str_op_empty_(Fail, Op, Msg, Value, ...) \
    CR_EXPAND(cr_assert_impl(                               \
            Fail,                                           \
            (Value)[0] Op '\0',                             \
            dummy,                                          \
            Msg,                                            \
            (CR_STR(Value)),                                \
            __VA_ARGS__                                     \
    ))

# define cr_assert_str_op_empty_va_(Fail, Op, Msg, ...) \
    CR_EXPAND(cr_assert_str_op_empty_(                  \
            Fail,                                       \
            Op,                                         \
            Msg,                                        \
            CR_VA_HEAD(__VA_ARGS__),                    \
            CR_VA_TAIL(__VA_ARGS__)                     \
    ))

# define cr_assert_str_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_ABORT_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))
# define cr_expect_str_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_CONTINUES_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))

# define cr_assert_str_not_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_ABORT_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))
# define cr_expect_str_not_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_CONTINUES_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))

# define cr_assert_str_op_(Fail, Op, Actual, Expected, ...) \
    CR_EXPAND(cr_assert_impl(                               \
            Fail,                                           \
            CR_STDN strcmp((Actual), (Expected)) Op 0,      \
            dummy,                                          \
            CRITERION_ASSERT_MSG_EXPR_AS_STRINGS_FALSE,     \
            (CR_STR((Actual) Op (Expected))),               \
            __VA_ARGS__                                     \
    ))

# define cr_assert_str_op_va_(Fail, Op, ...)        \
    CR_EXPAND(cr_assert_str_op_(                    \
            Fail,                                   \
            Op,                                     \
            CR_VA_HEAD(__VA_ARGS__),                \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),    \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))     \
    ))

# define cr_assert_str_eq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,      ==, __VA_ARGS__))
# define cr_expect_str_eq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_,  ==, __VA_ARGS__))

# define cr_assert_str_neq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,     !=, __VA_ARGS__))
# define cr_expect_str_neq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

# define cr_assert_str_lt(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,      <, __VA_ARGS__))
# define cr_expect_str_lt(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_,  <, __VA_ARGS__))

# define cr_assert_str_leq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,     <=, __VA_ARGS__))
# define cr_expect_str_leq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))

# define cr_assert_str_gt(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,      >, __VA_ARGS__))
# define cr_expect_str_gt(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_,  >, __VA_ARGS__))

# define cr_assert_str_geq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_ABORT_,     >=, __VA_ARGS__))
# define cr_expect_str_geq(...) CR_EXPAND(cr_assert_str_op_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))

// Array assertions

# define cr_assert_mem_op_(Fail, Op, Actual, Expected, Size, ...)       \
    CR_EXPAND(cr_assert_impl(                                           \
            Fail,                                                       \
            CR_STDN memcmp((Actual), (Expected), (Size)) Op 0,          \
            dummy,                                                      \
            CRITERION_ASSERT_MSG_EXPR_FALSE,                            \
            (CR_STR((Actual)[0 .. Size] Op (Expected)[0 .. Size])),     \
            __VA_ARGS__                                                 \
    ))

# define cr_assert_mem_op_va_(Fail, Op, ...)                    \
    CR_EXPAND(cr_assert_mem_op_(                                \
            Fail,                                               \
            Op,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                            \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),    \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))     \
    ))

# define cr_assert_arr_eq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_ABORT_,      ==, __VA_ARGS__))
# define cr_expect_arr_eq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_CONTINUES_,  ==, __VA_ARGS__))

# define cr_assert_arr_neq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_ABORT_,     !=, __VA_ARGS__))
# define cr_expect_arr_neq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

// Safe array comparison assertions

# if defined(__GNUC__) || defined(__clang__) || defined(__cplusplus)

#  ifdef __cplusplus
#   define CR_ARR_COMPARE_(A, B, Size, Cmp, Result)                     \
    int Result = std::lexicographical_compare((A), (A) + Size, (B), (B) + Size, Cmp)
#  else
#   define CR_ARR_COMPARE_(A, B, Size, Cmp, Result)                     \
        __typeof__(&(A)[0]) first = (A);                                \
        __typeof__(&(B)[0]) second = (B);                               \
        int Result = 0;                                                 \
        size_t i, size;                                                 \
        for (i = 0, size = (Size); !Result && i < size; ++i)            \
            Result = Cmp(first + i, second + i)
#  endif

#  define cr_assert_arr_op_cmp_(Fail, Op, Actual, Expected, Size, Cmp, ...) \
    do {                                                                    \
        CR_ARR_COMPARE_(Actual, Expected, Size, Cmp, order);                \
        CR_EXPAND(cr_assert_impl(                                           \
                Fail,                                                       \
                order Op 0,                                                 \
                dummy,                                                      \
                CRITERION_ASSERT_MSG_EXPR_FALSE,                            \
                (CR_STR((Actual)[0 .. Size] Op (Expected)[0 .. Size])),     \
                __VA_ARGS__                                                 \
        ));                                                                 \
    } while (0)

#  define cr_assert_arr_op_cmp_va_(Fail, Op, ...)                           \
    CR_EXPAND(cr_assert_arr_op_cmp_(                                        \
            Fail,                                                           \
            Op,                                                             \
            CR_VA_HEAD(__VA_ARGS__),                                        \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                            \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),                \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))),    \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))))     \
    ))

#  define cr_assert_arr_eq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,      ==, __VA_ARGS__))
#  define cr_expect_arr_eq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_,  ==, __VA_ARGS__))

#  define cr_assert_arr_neq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,     !=, __VA_ARGS__))
#  define cr_expect_arr_neq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

#  define cr_assert_arr_lt_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,      <, __VA_ARGS__))
#  define cr_expect_arr_lt_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_,  <, __VA_ARGS__))

#  define cr_assert_arr_leq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,     <=, __VA_ARGS__))
#  define cr_expect_arr_leq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, <=, __VA_ARGS__))

#  define cr_assert_arr_gt_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,      >, __VA_ARGS__))
#  define cr_expect_arr_gt_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_,  >, __VA_ARGS__))

#  define cr_assert_arr_geq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_ABORT_,     >=, __VA_ARGS__))
#  define cr_expect_arr_geq_cmp(...) CR_EXPAND(cr_assert_arr_op_cmp_va_(CR_FAIL_CONTINUES_, >=, __VA_ARGS__))

# else

#  define CRITERION_GNUC_WARN__(Msg) \
    _Pragma(#Msg)

#  define CRITERION_GNUC_WARN_(Name) CRITERION_GNUC_WARN__(             \
        message                                                         \
        "The `" #Name "` macro is only available on GNU C compilers."   \
    )

#  define cr_assert_arr_eq_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_eq_cmp) CR_NOOP
#  define cr_expect_arr_eq_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_eq_cmp) CR_NOOP

#  define cr_assert_arr_neq_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_neq_cmp) CR_NOOP
#  define cr_expect_arr_neq_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_neq_cmp) CR_NOOP

#  define cr_assert_arr_lt_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_lt_cmp) CR_NOOP
#  define cr_expect_arr_lt_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_lt_cmp) CR_NOOP

#  define cr_assert_arr_leq_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_leq_cmp) CR_NOOP
#  define cr_expect_arr_leq_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_leq_cmp) CR_NOOP

#  define cr_assert_arr_gt_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_gt_cmp) CR_NOOP
#  define cr_expect_arr_gt_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_gt_cmp) CR_NOOP

#  define cr_assert_arr_geq_cmp(...) CRITERION_GNUC_WARN_(cr_assert_arr_geq_cmp) CR_NOOP
#  define cr_expect_arr_geq_cmp(...) CRITERION_GNUC_WARN_(cr_expect_arr_geq_cmp) CR_NOOP

# endif

# ifdef __cplusplus

#  define cr_assert_throw_abort_(Fail, Msg, MsgArgs, ...)               \
    CR_EXPAND(cr_assert_impl(                                           \
            Fail,                                                       \
            0,                                                          \
            dummy,                                                      \
            Msg,                                                        \
            MsgArgs,                                                    \
            CR_VA_TAIL(__VA_ARGS__)                                     \
    ))

#  define cr_assert_throw_(Fail, Statement, Exception, ...)             \
    try {                                                               \
        Statement;                                                      \
    } catch (Exception const &) {                                       \
    } catch (...) {                                                     \
        CR_EXPAND(cr_assert_throw_abort_(                               \
                Fail,                                                   \
                CRITERION_ASSERT_MSG_NO_THROW,                          \
                (CR_STR(Statement), CR_STR(Exception)),                 \
                __VA_ARGS__));                                          \
    }

#  define cr_assert_throw_va_(...)                                      \
    CR_EXPAND(cr_assert_throw_(                                         \
            CR_VA_HEAD(__VA_ARGS__),                                    \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                        \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),            \
            dummy,                                                      \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))             \
    ))

#  define cr_assert_throw(...) CR_EXPAND(cr_assert_throw_va_(CR_FAIL_ABORT_,     __VA_ARGS__))
#  define cr_expect_throw(...) CR_EXPAND(cr_assert_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

#  define cr_assert_no_throw_(Fail, Statement, Exception, ...)          \
    try {                                                               \
        Statement;                                                      \
    } catch (Exception const &) {                                       \
        CR_EXPAND(cr_assert_throw_abort_(                               \
                Fail,                                                   \
                CRITERION_ASSERT_MSG_THROW,                             \
                (CR_STR(Statement), CR_STR(Exception)),                 \
                __VA_ARGS__));                                          \
    }

#  define cr_assert_no_throw_va_(...)                                   \
    CR_EXPAND(cr_assert_no_throw_(                                      \
            CR_VA_HEAD(__VA_ARGS__),                                    \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                        \
            CR_VA_HEAD(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))),            \
            dummy,                                                      \
            CR_VA_TAIL(CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__)))             \
    ))

#  define cr_assert_no_throw(...) CR_EXPAND(cr_assert_no_throw_va_(CR_FAIL_ABORT_,     __VA_ARGS__))
#  define cr_expect_no_throw(...) CR_EXPAND(cr_assert_no_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

#  define cr_assert_any_throw_(Fail, Statement, ...)                    \
    try {                                                               \
        Statement;                                                      \
        CR_EXPAND(cr_assert_throw_abort_(                               \
                Fail,                                                   \
                CRITERION_ASSERT_MSG_ANY_THROW,                         \
                (CR_STR(Statement)),                                    \
                __VA_ARGS__));                                          \
    } catch (...) {}

#  define cr_assert_any_throw_va_(...)                                  \
    CR_EXPAND(cr_assert_any_throw_(                                     \
            CR_VA_HEAD(__VA_ARGS__),                                    \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                        \
            dummy,                                                      \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                         \
    ))

#  define cr_assert_any_throw(...) CR_EXPAND(cr_assert_any_throw_va_(CR_FAIL_ABORT_,     __VA_ARGS__))
#  define cr_expect_any_throw(...) CR_EXPAND(cr_assert_any_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

#  define cr_assert_none_throw_(Fail, Statement, ...)                   \
    try {                                                               \
        Statement;                                                      \
    } catch (...) {                                                     \
        CR_EXPAND(cr_assert_throw_abort_(                               \
                Fail,                                                   \
                CRITERION_ASSERT_MSG_NONE_THROW,                        \
                (CR_STR(Statement)),                                    \
                __VA_ARGS__));                                          \
    }

#  define cr_assert_none_throw_va_(...)                                 \
    CR_EXPAND(cr_assert_none_throw_(                                    \
            CR_VA_HEAD(__VA_ARGS__),                                    \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                        \
            dummy,                                                      \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                         \
    ))

#  define cr_assert_none_throw(...) CR_EXPAND(cr_assert_none_throw_va_(CR_FAIL_ABORT_,     __VA_ARGS__))
#  define cr_expect_none_throw(...) CR_EXPAND(cr_assert_none_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

# endif

// The section below is here for backward compatibility purposes.
// It shall be removed in the next major version of Criterion
# ifndef CRITERION_NO_COMPAT

#  define CRITERION_ASSERT_DEPRECATED_B(Name, Newname) \
    CRITERION_ASSERT_DEPRECATED__(                     \
        message                                        \
        ("The `" #Name "` macro is deprecated, "       \
        "please use `" #Newname "` instead.")          \
    )

# ifdef _MSC_VER
#  define CRITERION_ASSERT_DEPRECATED__(Msg) \
    __pragma(Msg)
# else
#  define CRITERION_ASSERT_DEPRECATED__(Msg) \
    _Pragma(#Msg)
# endif

// scheduled for removal after 2.0
#  define cr_abort_test(Message) CRITERION_ASSERT_DEPRECATED_B(cr_abort_test, cr_assert_fail) cr_assert_fail(Message)
#  define cr_assert_strings_eq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_eq, cr_assert_str_eq) cr_assert_str_eq(__VA_ARGS__)
#  define cr_assert_strings_neq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_neq, cr_assert_str_neq) cr_assert_str_neq(__VA_ARGS__)
#  define cr_assert_strings_lt(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_lt, cr_assert_str_lt) cr_assert_str_lt(__VA_ARGS__)
#  define cr_assert_strings_leq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_leq, cr_assert_str_leq) cr_assert_str_leq(__VA_ARGS__)
#  define cr_assert_strings_gt(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_gt, cr_assert_str_gt) cr_assert_str_gt(__VA_ARGS__)
#  define cr_assert_strings_geq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_strings_geq, cr_assert_str_geq) cr_assert_str_geq(__VA_ARGS__)

#  define cr_assert_arrays_eq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_eq, cr_assert_arr_eq) cr_assert_arr_eq(__VA_ARGS__)
#  define cr_assert_arrays_neq(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_neq, cr_assert_arr_neq) cr_assert_arr_neq(__VA_ARGS__)

#  define cr_assert_arrays_eq_cmp(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_eq_cmp, cr_assert_arr_eq_cmp) cr_assert_arr_eq_cmp(__VA_ARGS__)
#  define cr_assert_arrays_neq_cmp(...) CRITERION_ASSERT_DEPRECATED_B(cr_assert_arrays_neq_cmp, cr_assert_arr_neq_cmp) cr_assert_arr_neq_cmp(__VA_ARGS__)

# endif

#endif /* !CRITERION_ASSERT_H_ */
