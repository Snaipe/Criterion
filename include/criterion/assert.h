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

# include "internal/assert.h"

// Base assertions

# define cr_assert_fail(...) CR_EXPAND(cr_fail(CR_FAIL_ABORT_,     __VA_ARGS__))
# define cr_expect_fail(...) CR_EXPAND(cr_fail(CR_FAIL_CONTINUES_, __VA_ARGS__))

# define cr_assert(...) CR_EXPAND(cr_assert_(__VA_ARGS__))
# define cr_expect(...) CR_EXPAND(cr_expect_(__VA_ARGS__))

# define cr_assert_not(...) CR_EXPAND(cr_assert_not_(__VA_ARGS__))
# define cr_expect_not(...) CR_EXPAND(cr_expect_not_(__VA_ARGS__))

// Common binary assertions

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

# define cr_assert_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_ABORT_,     ==, CRITERION_ASSERT_MSG_IS_NOT_NULL, __VA_ARGS__))
# define cr_expect_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_CONTINUES_, ==, CRITERION_ASSERT_MSG_IS_NOT_NULL, __VA_ARGS__))

# define cr_assert_not_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_ABORT_,     !=, CRITERION_ASSERT_MSG_IS_NULL, __VA_ARGS__))
# define cr_expect_not_null(...) CR_EXPAND(cr_assert_null_op_va_(CR_FAIL_CONTINUES_, !=, CRITERION_ASSERT_MSG_IS_NULL, __VA_ARGS__))

// Floating-point assertions

# define cr_assert_float_eq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_ABORT_,     cr_assert_float_eq_op_, __VA_ARGS__))
# define cr_expect_float_eq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_CONTINUES_, cr_assert_float_eq_op_, __VA_ARGS__))

# define cr_assert_float_neq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_ABORT_,     cr_assert_float_neq_op_, __VA_ARGS__))
# define cr_expect_float_neq(...) CR_EXPAND(cr_assert_float_op_va_(CR_FAIL_CONTINUES_, cr_assert_float_neq_op_, __VA_ARGS__))

// String assertions

# define cr_assert_str_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_ABORT_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))
# define cr_expect_str_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_CONTINUES_, ==, CRITERION_ASSERT_MSG_IS_NOT_EMPTY, __VA_ARGS__))

# define cr_assert_str_not_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_ABORT_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))
# define cr_expect_str_not_empty(...) CR_EXPAND(cr_assert_str_op_empty_va_(CR_FAIL_CONTINUES_, !=, CRITERION_ASSERT_MSG_IS_EMPTY, __VA_ARGS__))

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

# define cr_assert_arr_eq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_ABORT_,      ==, __VA_ARGS__))
# define cr_expect_arr_eq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_CONTINUES_,  ==, __VA_ARGS__))

# define cr_assert_arr_neq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_ABORT_,     !=, __VA_ARGS__))
# define cr_expect_arr_neq(...) CR_EXPAND(cr_assert_mem_op_va_(CR_FAIL_CONTINUES_, !=, __VA_ARGS__))

// Safe array comparison assertions

# if defined(__GNUC__) || defined(__clang__) || defined(__cplusplus)

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

#  define cr_assert_throw(...) CR_EXPAND(cr_assert_throw_va_(CR_FAIL_ABORT_,     __VA_ARGS__))
#  define cr_expect_throw(...) CR_EXPAND(cr_assert_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

#  define cr_assert_no_throw(...) CR_EXPAND(cr_assert_no_throw_va_(CR_FAIL_ABORT_,     __VA_ARGS__))
#  define cr_expect_no_throw(...) CR_EXPAND(cr_assert_no_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

#  define cr_assert_any_throw(...) CR_EXPAND(cr_assert_any_throw_va_(CR_FAIL_ABORT_,     __VA_ARGS__))
#  define cr_expect_any_throw(...) CR_EXPAND(cr_assert_any_throw_va_(CR_FAIL_CONTINUES_, __VA_ARGS__))

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
