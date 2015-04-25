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

# include <string.h>
# include <stdlib.h>
# include <stdbool.h>
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
    const char *default_msg;
    int sentinel_;
};

# define cr_assert_impl(Kind, Condition, ...)                   \
    do {                                                        \
        struct criterion_assert_args args = {                   \
            __VA_ARGS__                                         \
        };                                                      \
        int passed = !!(Condition);                             \
        struct criterion_assert_stats stat = {                  \
            .kind = (Kind),                                     \
            .condition = #Condition,                            \
            .message = args.msg ? args.msg                      \
                : (args.default_msg ? args.default_msg : ""),   \
            .passed = passed,                                   \
            .file = __FILE__,                                   \
            .line = __LINE__,                                   \
        };                                                      \
        send_event(ASSERT, &stat, sizeof (stat));               \
        if (!passed && (Kind) == FATAL)                         \
            criterion_abort_test();                             \
    } while (0)

// Common asserts

# define cr_abort_test(Message)                                             \
    cr_assert(0,                                                            \
            .default_msg = "The conditions for this test were not met.",    \
            .msg = (Message)                                                \
        )

# define cr_assert(...) cr_assert_(__VA_ARGS__, .sentinel_ = 0)

# define cr_expect(...) cr_expect_(__VA_ARGS__, .sentinel_ = 0)

# define cr_assert_(Condition, ...) cr_assert_impl(FATAL,  Condition, __VA_ARGS__)
# define cr_expect_(Condition, ...) cr_assert_impl(NORMAL, Condition, __VA_ARGS__)

# define cr_assert_not(...) cr_assert_not_(__VA_ARGS__, .sentinel_ = 0)
# define cr_expect_not(...) cr_expect_not_(__VA_ARGS__, .sentinel_ = 0)

# define cr_assert_not_(Condition, ...) \
    cr_assert_impl(FATAL,  !(Condition), __VA_ARGS__)
# define cr_expect_not_(Condition, ...) \
    cr_expect_impl(NORMAL, !(Condition), __VA_ARGS__)

// Native asserts

# define cr_assert_op_(Op, Actual, Expected, ...) \
    cr_assert_impl(FATAL,  (Actual) Op (Expected), __VA_ARGS__)
# define cr_expect_op_(Op, Actual, Expected, ...) \
    cr_assert_impl(NORMAL, (Actual) Op (Expected), __VA_ARGS__)

# define cr_assert_eq(...) cr_assert_op_(==, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_eq(...) cr_expect_op_(==, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_neq(...) cr_assert_op_(!=, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_neq(...) cr_expect_op_(!=, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_lt(...) cr_assert_op_(<, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_lt(...) cr_expect_op_(<, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_gt(...) cr_assert_op_(>, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_gt(...) cr_expect_op_(>, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_leq(...) cr_assert_op_(<=, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_leq(...) cr_expect_op_(<=, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_geq(...) cr_assert_op_(>=, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_geq(...) cr_expect_op_(>=, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_null_(Value, ...) \
    cr_assert_impl(FATAL,  (Value) == NULL, __VA_ARGS__)
# define cr_expect_null_(Value, ...) \
    cr_assert_impl(NORMAL, (Value) == NULL, __VA_ARGS__)

# define cr_assert_null(...) cr_assert_null_(__VA_ARGS__, .sentinel_ = 0)
# define cr_expect_null(...) cr_expect_null_(__VA_ARGS__, .sentinel_ = 0)

# define cr_assert_not_null_(Value, ...) \
    cr_assert_impl(FATAL,  (Value) != NULL, __VA_ARGS__)
# define cr_expect_not_null_(Value, ...) \
    cr_assert_impl(NORMAL, (Value) != NULL, __VA_ARGS__)

# define cr_assert_not_null(...) cr_assert_not_null_(__VA_ARGS__, .sentinel_ = 0)
# define cr_expect_not_null(...) cr_expect_not_null_(__VA_ARGS__, .sentinel_ = 0)

// Floating-point asserts

# define cr_assert_float_eq(...) \
    cr_assert_float_eq_(__VA_ARGS__, .sentinel_ = 0)
# define cr_expect_float_eq(...) \
    cr_expect_float_eq_(__VA_ARGS__, .sentinel_ = 0)

# define cr_assert_float_eq_(Actual, Expected, Epsilon, ...) \
    cr_assert_impl(FATAL,  (Expected) - (Actual) <= (Epsilon)  \
                     && (Actual) - (Expected) <= (Epsilon), \
                __VA_ARGS__)
# define cr_expect_float_eq_(Actual, Expected, Epsilon, ...) \
    cr_assert_impl(NORMAL, (Expected) - (Actual) <= (Epsilon)  \
                     && (Actual) - (Expected) <= (Epsilon), \
                __VA_ARGS__)

# define cr_assert_float_neq(...) \
    cr_assert_float_neq_(__VA_ARGS__, .sentinel_ = 0)
# define cr_expect_float_neq(...) \
    cr_expect_float_neq_(__VA_ARGS__, .sentinel_ = 0)

# define cr_assert_float_neq_(Actual, Expected, Epsilon, ...) \
    cr_assert_impl(FATAL,  (Expected) - (Actual) > (Epsilon)  \
                     || (Actual) - (Expected) > (Epsilon), \
                __VA_ARGS__)
# define cr_expect_float_neq_(Actual, Expected, Epsilon, ...) \
    cr_assert_impl(NORMAL, (Expected) - (Actual) > (Epsilon)  \
                     || (Actual) - (Expected) > (Epsilon), \
                __VA_ARGS__)

// String asserts

# define cr_assert_strings_(Op, Actual, Expected, ...) \
    cr_assert_impl(FATAL, strcmp((Actual), (Expected)) Op 0, __VA_ARGS__)
# define cr_expect_strings_(Op, Actual, Expected, ...) \
    cr_assert_impl(NORMAL, strcmp((Actual), (Expected)) Op 0, __VA_ARGS__)

# define cr_assert_strings_eq(...) \
    cr_assert_strings_(==, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_strings_eq(...) \
    cr_expect_strings_(==, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_strings_neq(...) \
    cr_assert_strings_(!=, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_strings_neq(...) \
    cr_expect_strings_(!=, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_strings_gt(...) cr_assert_strings_(>, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_strings_gt(...) cr_expect_strings_(>, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_strings_lt(...) cr_assert_strings_(<, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_strings_lt(...) cr_expect_strings_(<, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_strings_leq(...) cr_assert_strings_(<=, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_strings_leq(...) cr_expect_strings_(<=, __VA_ARGS__, .sentinel_ = 0)

# define cr_assert_strings_geq(...) cr_assert_strings_(>=, __VA_ARGS__, .sentinel_ = 0)
# define cr_expect_strings_geq(...) cr_expect_strings_(>=, __VA_ARGS__, .sentinel_ = 0)

// Array asserts

# define cr_assert_arrays_eq(...) \
    cr_assert_arrays_eq_(__VA_ARGS__, .sentinel_ = 0)
# define cr_expect_arrays_eq(...) \
    cr_expect_arrays_eq_(__VA_ARGS__, .sentinel_ = 0)

# define cr_assert_arrays_neq(...) \
    cr_assert_arrays_neq_(__VA_ARGS__, .sentinel_ = 0)
# define cr_expect_arrays_neq(...) \
    cr_expect_arrays_neq_(__VA_ARGS__, .sentinel_ = 0)

# define cr_assert_arrays_eq_(A, B, Size, ...)          \
    cr_assert_impl(FATAL, !memcmp((A), (B), (Size)),    \
            .default_msg = "Arrays are not equal.",     \
            __VA_ARGS__)
# define cr_expect_arrays_eq_(A, B, Size, ...) \
    cr_assert_impl(NORMAL, !memcmp((A), (B), (Size)),   \
            .default_msg = "Arrays are not equal.",     \
            __VA_ARGS__)

# define cr_assert_arrays_neq_(A, B, Size, ...)         \
    cr_assert_impl(FATAL, memcmp((A), (B), (Size)),     \
            .default_msg = "Arrays are equal",          \
            __VA_ARGS__)
# define cr_expect_arrays_neq_(A, B, Size, ...)         \
    cr_assert_impl(NORMAL, memcmp((A), (B), (Size)),    \
            .default_msg = "Arrays are equal",          \
            __VA_ARGS__)

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
                .default_msg = "Arrays are not equal",                  \
                __VA_ARGS__);                                           \
    } while (0)

#  define cr_expect_arrays_eq_cmp_(A, B, Size, Cmp, ...)                \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        cr_assert_impl(NORMAL, equals,                                  \
                .default_msg = "Arrays are not equal",                  \
                __VA_ARGS__);                                           \
    } while (0)

#  define cr_assert_arrays_eq_cmp(...) \
    cr_assert_arrays_eq_cmp_(__VA_ARGS__, .sentinel_ = 0)
#  define cr_expect_arrays_eq_cmp(...) \
    cr_expect_arrays_eq_cmp_(__VA_ARGS__, .sentinel_ = 0)

#  define cr_assert_arrays_neq_cmp_(A, B, Size, Cmp, ...)               \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        cr_assert_impl(FATAL, !equals,                                  \
                .default_msg = "Arrays not equal",                      \
                __VA_ARGS__);                                           \
    } while (0)

#  define cr_expect_arrays_neq_cmp_(A, B, Size, Cmp, ...)               \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        cr_assert_impl(NORMAL, equals,                                  \
                .default_msg = "Arrays not equal",                      \
                __VA_ARGS__);                                           \
    } while (0)

#  define cr_assert_arrays_neq_cmp(...) \
    cr_assert_arrays_eq_cmp_(__VA_ARGS__, .sentinel_ = 0)
#  define cr_expect_arrays_neq_cmp(...) \
    cr_expect_arrays_eq_cmp_(__VA_ARGS__, .sentinel_ = 0)
# endif /* !__GNUC__ */

// The section below is here for backward compatibility purposes.
// It shall be removed in the text major version of Criterion
# ifndef CRITERION_NO_COMPAT

/*
#  define CRITERION_ASSERT_DEPRECATED_(Name)    \
    _Pragma("message \""                        \
        "The " #Name " macro is deprecated, "   \
        "please use cr_" #Name " instead."      \
        "\""                                    \
    )
    */

#  define CRITERION_ASSERT_DEPRECATED_(Name) \
    _Pragma("message \"This macro is deprecated, please use its `cr_`-prefixed alternative instead.\"")

#  ifndef assert
#   define assert(...) CRITERION_ASSERT_DEPRECATED_(assert) cr_assert(__VA_ARGS__)

// this is needed to make the POSIX assert.h redefine assert if
// subsequently included
#   define _ASSERT_H 1
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
