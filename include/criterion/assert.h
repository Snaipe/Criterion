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
#  include <cstring>
#  include <cstdlib>
using std::strcmp;
using std::memcmp;
# else
#  include <string.h>
#  include <stdlib.h>
#  include <stdbool.h>
# endif
# include "designated-initializer-compat.h"
# include "types.h"
# include "stats.h"
# include "hooks.h"
# include "event.h"
# include "abort.h"

enum criterion_assert_kind {
    NORMAL,
    FATAL
};

struct criterion_assert_args {
    const char *msg;
    int sentinel_;

#ifdef __cplusplus
    constexpr criterion_assert_args(const char *msg) : msg(msg), sentinel_(0) {}
    constexpr criterion_assert_args(const char *msg, int sentinel_) : msg(msg), sentinel_(sentinel_) {}
#endif
};

# define CR_GET_CONDITION(Condition, ...) Condition
# define CR_GET_CONDITION_STR(Condition, ...) #Condition
# define CR_VA_SKIP(_, ...) __VA_ARGS__

# ifndef __cplusplus
#  define CR_ZERO_FILL(Arg) memset(&(Arg), 0, sizeof (Arg))
# else
#  define CR_ZERO_FILL(Arg) std::memset(&(Arg), 0, sizeof (Arg))
# endif

# define cr_assert_impl(Kind, ...)                                          \
    do {                                                                    \
        struct criterion_assert_args args = {                               \
            CR_EXPAND(CR_VA_SKIP(__VA_ARGS__))                              \
        };                                                                  \
        bool passed = !!(CR_EXPAND(CR_GET_CONDITION(__VA_ARGS__)));         \
        struct criterion_assert_stats stat;                                 \
        CR_ZERO_FILL(stat);                                                 \
        stat.kind = (Kind);                                                 \
        stat.condition = CR_EXPAND(CR_GET_CONDITION_STR(__VA_ARGS__));      \
        stat.message = args.msg ? args.msg : "";                            \
        stat.passed = passed;                                               \
        stat.file = __FILE__;                                               \
        stat.line = __LINE__;                                               \
        send_event(ASSERT, &stat, sizeof (stat));                           \
        if (!passed && (Kind) == FATAL)                                     \
            criterion_abort_test();                                         \
    } while (0)

// Common asserts

# define cr_abort_test(Message)                                                \
    do {                                                                       \
        const char *msg = (Message);                                           \
        cr_assert(0, msg ? msg : "The conditions for this test were not met.");\
    } while (0)

# ifdef __cplusplus
#  define CR_SENTINEL 0
# else
#  define CR_SENTINEL .sentinel_ = 0
# endif

# define cr_assert(...) CR_EXPAND(cr_assert_(__VA_ARGS__, CR_SENTINEL))

# define cr_expect(...) CR_EXPAND(cr_expect_(__VA_ARGS__, CR_SENTINEL))

# define cr_assert_(Condition, ...) CR_EXPAND(cr_assert_impl(FATAL,  Condition, __VA_ARGS__))
# define cr_expect_(Condition, ...) CR_EXPAND(cr_assert_impl(NORMAL, Condition, __VA_ARGS__))

# define cr_assert_not(...) CR_EXPAND(cr_assert_not_(__VA_ARGS__, CR_SENTINEL))
# define cr_expect_not(...) CR_EXPAND(cr_expect_not_(__VA_ARGS__, CR_SENTINEL))

# define cr_assert_not_(Condition, ...) \
    CR_EXPAND(cr_assert_impl(FATAL,  !(Condition), __VA_ARGS__))
# define cr_expect_not_(Condition, ...) \
    CR_EXPAND(cr_expect_impl(NORMAL, !(Condition), __VA_ARGS__))

// Native asserts

# define cr_assert_op_(Op, Actual, Expected, ...) \
    CR_EXPAND(cr_assert_impl(FATAL,  (Actual) Op (Expected), __VA_ARGS__))
# define cr_expect_op_(Op, Actual, Expected, ...) \
    CR_EXPAND(cr_assert_impl(NORMAL, (Actual) Op (Expected), __VA_ARGS__))

# define cr_assert_eq(...) CR_EXPAND(cr_assert_op_(==, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_eq(...) CR_EXPAND(cr_expect_op_(==, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_neq(...) CR_EXPAND(cr_assert_op_(!=, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_neq(...) CR_EXPAND(cr_expect_op_(!=, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_lt(...) CR_EXPAND(cr_assert_op_(<, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_lt(...) CR_EXPAND(cr_expect_op_(<, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_gt(...) CR_EXPAND(cr_assert_op_(>, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_gt(...) CR_EXPAND(cr_expect_op_(>, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_leq(...) CR_EXPAND(cr_assert_op_(<=, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_leq(...) CR_EXPAND(cr_expect_op_(<=, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_geq(...) CR_EXPAND(cr_assert_op_(>=, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_geq(...) CR_EXPAND(cr_expect_op_(>=, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_null_(Value, ...) \
    CR_EXPAND(cr_assert_impl(FATAL,  (Value) == NULL, __VA_ARGS__))
# define cr_expect_null_(Value, ...) \
    CR_EXPAND(cr_assert_impl(NORMAL, (Value) == NULL, __VA_ARGS__))

# define cr_assert_null(...) CR_EXPAND(cr_assert_null_(__VA_ARGS__, CR_SENTINEL))
# define cr_expect_null(...) CR_EXPAND(cr_expect_null_(__VA_ARGS__, CR_SENTINEL))

# define cr_assert_not_null_(Value, ...) \
    CR_EXPAND(cr_assert_impl(FATAL,  (Value) != NULL, __VA_ARGS__))
# define cr_expect_not_null_(Value, ...) \
    CR_EXPAND(cr_assert_impl(NORMAL, (Value) != NULL, __VA_ARGS__))

# define cr_assert_not_null(...) CR_EXPAND(cr_assert_not_null_(__VA_ARGS__, CR_SENTINEL))
# define cr_expect_not_null(...) CR_EXPAND(cr_expect_not_null_(__VA_ARGS__, CR_SENTINEL))

// Floating-point asserts

# define cr_assert_float_eq(...) \
    CR_EXPAND(cr_assert_float_eq_(__VA_ARGS__, CR_SENTINEL))
# define cr_expect_float_eq(...) \
    CR_EXPAND(cr_expect_float_eq_(__VA_ARGS__, CR_SENTINEL))

# define cr_assert_float_eq_(Actual, Expected, Epsilon, ...) \
    CR_EXPAND(cr_assert_impl(FATAL,  (Expected) - (Actual) <= (Epsilon)  \
                     && (Actual) - (Expected) <= (Epsilon), \
                __VA_ARGS__))
# define cr_expect_float_eq_(Actual, Expected, Epsilon, ...) \
    CR_EXPAND(cr_assert_impl(NORMAL, (Expected) - (Actual) <= (Epsilon)  \
                     && (Actual) - (Expected) <= (Epsilon), \
                __VA_ARGS__))

# define cr_assert_float_neq(...) \
    CR_EXPAND(cr_assert_float_neq_(__VA_ARGS__, CR_SENTINEL))
# define cr_expect_float_neq(...) \
    CR_EXPAND(cr_expect_float_neq_(__VA_ARGS__, CR_SENTINEL))

# define cr_assert_float_neq_(Actual, Expected, Epsilon, ...) \
    CR_EXPAND(cr_assert_impl(FATAL,  (Expected) - (Actual) > (Epsilon)  \
                     || (Actual) - (Expected) > (Epsilon), \
                __VA_ARGS__))
# define cr_expect_float_neq_(Actual, Expected, Epsilon, ...) \
    CR_EXPAND(cr_assert_impl(NORMAL, (Expected) - (Actual) > (Epsilon)  \
                     || (Actual) - (Expected) > (Epsilon), \
                __VA_ARGS__))

// String asserts

# define cr_assert_strings_(Op, Actual, Expected, ...) \
    CR_EXPAND(cr_assert_impl(FATAL, strcmp((Actual), (Expected)) Op 0, __VA_ARGS__))
# define cr_expect_strings_(Op, Actual, Expected, ...) \
    CR_EXPAND(cr_assert_impl(NORMAL, strcmp((Actual), (Expected)) Op 0, __VA_ARGS__))

# define cr_assert_strings_eq(...) \
    CR_EXPAND(cr_assert_strings_(==, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_strings_eq(...) \
    CR_EXPAND(cr_expect_strings_(==, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_strings_neq(...) \
    CR_EXPAND(cr_assert_strings_(!=, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_strings_neq(...) \
    CR_EXPAND(cr_expect_strings_(!=, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_strings_gt(...) CR_EXPAND(cr_assert_strings_(>, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_strings_gt(...) CR_EXPAND(cr_expect_strings_(>, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_strings_lt(...) CR_EXPAND(cr_assert_strings_(<, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_strings_lt(...) CR_EXPAND(cr_expect_strings_(<, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_strings_leq(...) CR_EXPAND(cr_assert_strings_(<=, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_strings_leq(...) CR_EXPAND(cr_expect_strings_(<=, __VA_ARGS__, CR_SENTINEL))

# define cr_assert_strings_geq(...) CR_EXPAND(cr_assert_strings_(>=, __VA_ARGS__, CR_SENTINEL))
# define cr_expect_strings_geq(...) CR_EXPAND(cr_expect_strings_(>=, __VA_ARGS__, CR_SENTINEL))

// Array asserts

# define cr_assert_arrays_eq(...) \
    CR_EXPAND(cr_assert_arrays_eq_(__VA_ARGS__, CR_SENTINEL))
# define cr_expect_arrays_eq(...) \
    CR_EXPAND(cr_expect_arrays_eq_(__VA_ARGS__, CR_SENTINEL))

# define cr_assert_arrays_neq(...) \
    CR_EXPAND(cr_assert_arrays_neq_(__VA_ARGS__, CR_SENTINEL))
# define cr_expect_arrays_neq(...) \
    CR_EXPAND(cr_expect_arrays_neq_(__VA_ARGS__, CR_SENTINEL))

# define cr_assert_arrays_eq_(A, B, Size, ...)                    \
    CR_EXPAND(cr_assert_impl(FATAL, !memcmp((A), (B), (Size)),    \
            __VA_ARGS__))
# define cr_expect_arrays_eq_(A, B, Size, ...)                    \
    CR_EXPAND(cr_assert_impl(NORMAL, !memcmp((A), (B), (Size)),   \
            __VA_ARGS__))

# define cr_assert_arrays_neq_(A, B, Size, ...)                   \
    CR_EXPAND(cr_assert_impl(FATAL, memcmp((A), (B), (Size)),     \
            __VA_ARGS__))
# define cr_expect_arrays_neq_(A, B, Size, ...)                   \
    CR_EXPAND(cr_assert_impl(NORMAL, memcmp((A), (B), (Size)),    \
            __VA_ARGS__))

# ifdef __GNUC__
#  define CRIT_ARR_COMPARE_(A, B, Size, Cmp, Result)                \
    __typeof__(&(A)[0]) first = (A);                                \
    __typeof__(&(B)[0]) second = (B);                               \
    int equals = 1;                                                 \
    size_t i, size;                                                 \
    for (i = 0, size = (Size); equals && i < size; ++i)             \
        equals = equals && !Cmp(first + i, second + i)

#  define cr_assert_arrays_eq_cmp_(A, B, Size, Cmp, ...)                \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        cr_assert_impl(FATAL, equals,                                   \
                __VA_ARGS__);                                           \
    } while (0)

#  define cr_expect_arrays_eq_cmp_(A, B, Size, Cmp, ...)                \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        cr_assert_impl(NORMAL, equals,                                  \
                __VA_ARGS__);                                           \
    } while (0)

#  define cr_assert_arrays_eq_cmp(...) \
    cr_assert_arrays_eq_cmp_(__VA_ARGS__, CR_SENTINEL)
#  define cr_expect_arrays_eq_cmp(...) \
    cr_expect_arrays_eq_cmp_(__VA_ARGS__, CR_SENTINEL)

#  define cr_assert_arrays_neq_cmp_(A, B, Size, Cmp, ...)               \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        cr_assert_impl(FATAL, !equals,                                  \
                __VA_ARGS__);                                           \
    } while (0)

#  define cr_expect_arrays_neq_cmp_(A, B, Size, Cmp, ...)               \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        cr_assert_impl(NORMAL, equals,                                  \
                __VA_ARGS__);                                           \
    } while (0)

#  define cr_assert_arrays_neq_cmp(...) \
    cr_assert_arrays_eq_cmp_(__VA_ARGS__, CR_SENTINEL)
#  define cr_expect_arrays_neq_cmp(...) \
    cr_expect_arrays_eq_cmp_(__VA_ARGS__, CR_SENTINEL)
# endif /* !__GNUC__ */

// The section below is here for backward compatibility purposes.
// It shall be removed in the text major version of Criterion
# ifndef CRITERION_NO_COMPAT

#  define CRITERION_ASSERT_DEPRECATED_(Name) CRITERION_ASSERT_DEPRECATED__(    \
        message                                                                \
        "The `" #Name "` macro is deprecated, "                                \
        "please use `cr_" #Name "` instead."                                   \
    )

#  define CRITERION_ASSERT_DEPRECATED__(Msg) \
    _Pragma(#Msg)

#  ifndef assert
#   define assert(...) CRITERION_ASSERT_DEPRECATED_(assert) cr_assert(__VA_ARGS__)

// this is needed to make the POSIX assert.h redefine assert if
// subsequently included
#   ifndef _ASSERT_H
#    define _ASSERT_H 1
#   endif /* !_ASSERT_H */
#  endif /* !assert */

#  define abort_test(Message) CRITERION_ASSERT_DEPRECATED_(abort_test) cr_abort_test(Message)
#  define expect(...) CRITERION_ASSERT_DEPRECATED_(expect) cr_expect(__VA_ARGS__)
#  define assert_not(...) CRITERION_ASSERT_DEPRECATED_(assert_not) cr_assert_not(__VA_ARGS__)
#  define expect_not(...) CRITERION_ASSERT_DEPRECATED_(expect_not) cr_expect_not(__VA_ARGS__)
#  define assert_eq(...) CRITERION_ASSERT_DEPRECATED_(assert_eq) cr_assert_eq(__VA_ARGS__)
#  define expect_eq(...) CRITERION_ASSERT_DEPRECATED_(expect_eq) cr_expect_eq(__VA_ARGS__)
#  define assert_neq(...) CRITERION_ASSERT_DEPRECATED_(assert_neq) cr_assert_neq(__VA_ARGS__)
#  define expect_neq(...) CRITERION_ASSERT_DEPRECATED_(expect_neq) cr_expect_neq(__VA_ARGS__)
#  define assert_lt(...) CRITERION_ASSERT_DEPRECATED_(assert_lt) cr_assert_lt(__VA_ARGS__)
#  define expect_lt(...) CRITERION_ASSERT_DEPRECATED_(expect_lt) cr_expect_lt(__VA_ARGS__)
#  define assert_gt(...) CRITERION_ASSERT_DEPRECATED_(assert_gt) cr_assert_gt(__VA_ARGS__)
#  define expect_gt(...) CRITERION_ASSERT_DEPRECATED_(expect_gt) cr_expect_gt(__VA_ARGS__)
#  define assert_leq(...) CRITERION_ASSERT_DEPRECATED_(assert_leq) cr_assert_leq(__VA_ARGS__)
#  define expect_leq(...) CRITERION_ASSERT_DEPRECATED_(expect_leq) cr_expect_leq(__VA_ARGS__)
#  define assert_geq(...) CRITERION_ASSERT_DEPRECATED_(assert_geq) cr_assert_geq(__VA_ARGS__)
#  define expect_geq(...) CRITERION_ASSERT_DEPRECATED_(expect_geq) cr_expect_geq(__VA_ARGS__)
#  define assert_null(...) CRITERION_ASSERT_DEPRECATED_(assert_null) cr_assert_null(__VA_ARGS__)
#  define expect_null(...) CRITERION_ASSERT_DEPRECATED_(expect_null) cr_expect_null(__VA_ARGS__)
#  define assert_not_null(...) CRITERION_ASSERT_DEPRECATED_(assert_not_null) cr_assert_not_null(__VA_ARGS__)
#  define expect_not_null(...) CRITERION_ASSERT_DEPRECATED_(expect_not_null) cr_expect_not_null(__VA_ARGS__)
#  define assert_float_eq(...) CRITERION_ASSERT_DEPRECATED_(assert_float_eq) cr_assert_float_eq(__VA_ARGS__)
#  define expect_float_eq(...) CRITERION_ASSERT_DEPRECATED_(expect_float_eq) cr_expect_float_eq(__VA_ARGS__)
#  define assert_float_neq(...) CRITERION_ASSERT_DEPRECATED_(assert_float_neq) cr_assert_float_neq(__VA_ARGS__)
#  define expect_float_neq(...) CRITERION_ASSERT_DEPRECATED_(expect_float_neq) cr_expect_float_neq(__VA_ARGS__)
#  define assert_strings_eq(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_eq) cr_assert_strings_eq(__VA_ARGS__)
#  define expect_strings_eq(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_eq) cr_expect_strings_eq(__VA_ARGS__)
#  define assert_strings_neq(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_neq) cr_assert_strings_neq(__VA_ARGS__)
#  define expect_strings_neq(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_neq) cr_expect_strings_neq(__VA_ARGS__)
#  define assert_strings_gt(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_gt) cr_assert_strings_gt(__VA_ARGS__)
#  define expect_strings_gt(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_gt) cr_expect_strings_gt(__VA_ARGS__)
#  define assert_strings_lt(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_lt) cr_assert_strings_lt(__VA_ARGS__)
#  define expect_strings_lt(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_lt) cr_expect_strings_lt(__VA_ARGS__)
#  define assert_strings_leq(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_leq) cr_assert_strings_leq(__VA_ARGS__)
#  define expect_strings_leq(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_leq) cr_expect_strings_leq(__VA_ARGS__)
#  define assert_strings_geq(...) CRITERION_ASSERT_DEPRECATED_(assert_strings_geq) cr_assert_strings_geq(__VA_ARGS__)
#  define expect_strings_geq(...) CRITERION_ASSERT_DEPRECATED_(expect_strings_geq) cr_expect_strings_geq(__VA_ARGS__)
#  define assert_arrays_eq(...) CRITERION_ASSERT_DEPRECATED_(assert_arrays_eq) cr_assert_arrays_eq(__VA_ARGS__)
#  define expect_arrays_eq(...) CRITERION_ASSERT_DEPRECATED_(expect_arrays_eq) cr_expect_arrays_eq(__VA_ARGS__)
#  define assert_arrays_neq(...) CRITERION_ASSERT_DEPRECATED_(assert_arrays_neq) cr_assert_arrays_neq(__VA_ARGS__)
#  define expect_arrays_neq(...) CRITERION_ASSERT_DEPRECATED_(expect_arrays_neq) cr_expect_arrays_neq(__VA_ARGS__)
#  define assert_arrays_eq_cmp(...) CRITERION_ASSERT_DEPRECATED_(assert_arrays_eq_cmp) cr_assert_arrays_eq_cmp(__VA_ARGS__)
#  define expect_arrays_eq_cmp(...) CRITERION_ASSERT_DEPRECATED_(expect_arrays_eq_cmp) cr_expect_arrays_eq_cmp(__VA_ARGS__)
#  define assert_arrays_neq_cmp(...) CRITERION_ASSERT_DEPRECATED_(assert_arrays_neq_cmp) cr_assert_arrays_neq_cmp(__VA_ARGS__)
#  define expect_arrays_neq_cmp(...) CRITERION_ASSERT_DEPRECATED_(expect_arrays_neq_cmp) cr_expect_arrays_neq_cmp(__VA_ARGS__)

# endif

#endif /* !CRITERION_ASSERT_H_ */
