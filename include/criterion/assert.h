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

# define assert_impl(Kind, Condition, ...)                      \
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

# define abort_test(Message)                                                \
    assert(0,                                                               \
            .default_msg = "The conditions for this test were not met.",    \
            .msg = (Message)                                                \
        )

# define assert(...) assert_(__VA_ARGS__, .sentinel_ = 0)
# define expect(...) expect_(__VA_ARGS__, .sentinel_ = 0)

# define assert_(Condition, ...) assert_impl(FATAL,  Condition, __VA_ARGS__)
# define expect_(Condition, ...) assert_impl(NORMAL, Condition, __VA_ARGS__)

# define assert_not(...) assert_not_(__VA_ARGS__, .sentinel_ = 0)
# define expect_not(...) expect_not_(__VA_ARGS__, .sentinel_ = 0)

# define assert_not_(Condition, ...) \
    assert_impl(FATAL,  !(Condition), __VA_ARGS__)
# define expect_not_(Condition, ...) \
    expect_impl(NORMAL, !(Condition), __VA_ARGS__)

// Native asserts

# define assert_op_(Op, Actual, Expected, ...) \
    assert_impl(FATAL,  (Actual) Op (Expected), __VA_ARGS__)
# define expect_op_(Op, Actual, Expected, ...) \
    assert_impl(NORMAL, (Actual) Op (Expected), __VA_ARGS__)

# define assert_eq(...) assert_op_(==, __VA_ARGS__, .sentinel_ = 0)
# define expect_eq(...) expect_op_(==, __VA_ARGS__, .sentinel_ = 0)

# define assert_neq(...) assert_op_(!=, __VA_ARGS__, .sentinel_ = 0)
# define expect_neq(...) expect_op_(!=, __VA_ARGS__, .sentinel_ = 0)

# define assert_lt(...) assert_op_(<, __VA_ARGS__, .sentinel_ = 0)
# define expect_lt(...) expect_op_(<, __VA_ARGS__, .sentinel_ = 0)

# define assert_gt(...) assert_op_(>, __VA_ARGS__, .sentinel_ = 0)
# define expect_gt(...) expect_op_(>, __VA_ARGS__, .sentinel_ = 0)

# define assert_leq(...) assert_op_(<=, __VA_ARGS__, .sentinel_ = 0)
# define expect_leq(...) expect_op_(<=, __VA_ARGS__, .sentinel_ = 0)

# define assert_geq(...) assert_op_(>=, __VA_ARGS__, .sentinel_ = 0)
# define expect_geq(...) expect_op_(>=, __VA_ARGS__, .sentinel_ = 0)

# define assert_null_(Value, ...) \
    assert_impl(FATAL,  (Value) == NULL, __VA_ARGS__)
# define expect_null_(Value, ...) \
    assert_impl(NORMAL, (Value) == NULL, __VA_ARGS__)

# define assert_null(...) assert_null_(__VA_ARGS__, .sentinel_ = 0)
# define expect_null(...) expect_null_(__VA_ARGS__, .sentinel_ = 0)

# define assert_not_null_(Value, ...) \
    assert_impl(FATAL,  (Value) != NULL, __VA_ARGS__)
# define expect_not_null_(Value, ...) \
    assert_impl(NORMAL, (Value) != NULL, __VA_ARGS__)

# define assert_not_null(...) assert_not_null_(__VA_ARGS__, .sentinel_ = 0)
# define expect_not_null(...) expect_not_null_(__VA_ARGS__, .sentinel_ = 0)

// Floating-point asserts

# define assert_float_eq(...) \
    assert_float_eq_(__VA_ARGS__, .sentinel_ = 0)
# define expect_float_eq(...) \
    expect_float_eq_(__VA_ARGS__, .sentinel_ = 0)

# define assert_float_eq_(Actual, Expected, Epsilon, ...) \
    assert_impl(FATAL,  (Expected) - (Actual) <= (Epsilon)  \
                     && (Actual) - (Expected) <= (Epsilon), \
                __VA_ARGS__)
# define expect_float_eq_(Actual, Expected, Epsilon, ...) \
    assert_impl(NORMAL, (Expected) - (Actual) <= (Epsilon)  \
                     && (Actual) - (Expected) <= (Epsilon), \
                __VA_ARGS__)

# define assert_float_neq(...) \
    assert_float_neq_(__VA_ARGS__, .sentinel_ = 0)
# define expect_float_neq(...) \
    expect_float_neq_(__VA_ARGS__, .sentinel_ = 0)

# define assert_float_neq_(Actual, Expected, Epsilon, ...) \
    assert_impl(FATAL,  (Expected) - (Actual) > (Epsilon)  \
                     || (Actual) - (Expected) > (Epsilon), \
                __VA_ARGS__)
# define expect_float_neq_(Actual, Expected, Epsilon, ...) \
    assert_impl(NORMAL, (Expected) - (Actual) > (Epsilon)  \
                     || (Actual) - (Expected) > (Epsilon), \
                __VA_ARGS__)

// String asserts

# define assert_strings_(Op, Actual, Expected, ...) \
    assert_impl(FATAL, strcmp((Actual), (Expected)) Op 0, __VA_ARGS__)
# define expect_strings_(Op, Actual, Expected, ...) \
    assert_impl(NORMAL, strcmp((Actual), (Expected)) Op 0, __VA_ARGS__)

# define assert_strings_eq(...) \
    assert_strings_(==, __VA_ARGS__, .sentinel_ = 0)
# define expect_strings_eq(...) \
    expect_strings_(==, __VA_ARGS__, .sentinel_ = 0)

# define assert_strings_neq(...) \
    assert_strings_(!=, __VA_ARGS__, .sentinel_ = 0)
# define expect_strings_neq(...) \
    expect_strings_(!=, __VA_ARGS__, .sentinel_ = 0)

# define assert_strings_gt(...) assert_strings_(>, __VA_ARGS__, .sentinel_ = 0)
# define expect_strings_gt(...) expect_strings_(>, __VA_ARGS__, .sentinel_ = 0)

# define assert_strings_lt(...) assert_strings_(<, __VA_ARGS__, .sentinel_ = 0)
# define expect_strings_lt(...) expect_strings_(<, __VA_ARGS__, .sentinel_ = 0)

# define assert_strings_leq(...) assert_strings_(<=, __VA_ARGS__, .sentinel_ = 0)
# define expect_strings_leq(...) expect_strings_(<=, __VA_ARGS__, .sentinel_ = 0)

# define assert_strings_geq(...) assert_strings_(>=, __VA_ARGS__, .sentinel_ = 0)
# define expect_strings_geq(...) expect_strings_(>=, __VA_ARGS__, .sentinel_ = 0)

// Array asserts

# define assert_arrays_eq(...) \
    assert_arrays_eq_(__VA_ARGS__, .sentinel_ = 0)
# define expect_arrays_eq(...) \
    expect_arrays_eq_(__VA_ARGS__, .sentinel_ = 0)

# define assert_arrays_neq(...) \
    assert_arrays_neq_(__VA_ARGS__, .sentinel_ = 0)
# define expect_arrays_neq(...) \
    expect_arrays_neq_(__VA_ARGS__, .sentinel_ = 0)

# define assert_arrays_eq_(A, B, Size, ...)             \
    assert_impl(FATAL, !memcmp((A), (B), (Size)),       \
            .default_msg = "Arrays are not equal.",     \
            __VA_ARGS__)
# define expect_arrays_eq_(A, B, Size, ...) \
    assert_impl(NORMAL, !memcmp((A), (B), (Size)),      \
            .default_msg = "Arrays are not equal.",     \
            __VA_ARGS__)

# define assert_arrays_neq_(A, B, Size, ...)            \
    assert_impl(FATAL, memcmp((A), (B), (Size)),        \
            .default_msg = "Arrays are equal",          \
            __VA_ARGS__)
# define expect_arrays_neq_(A, B, Size, ...)            \
    assert_impl(NORMAL, memcmp((A), (B), (Size)),       \
            .default_msg = "Arrays are equal",          \
            __VA_ARGS__)

# ifdef __GNUC__
#  define CRIT_ARR_COMPARE_(A, B, Size, Cmp, Result)                \
    __typeof__(&(A)[0]) first = (A);                                \
    __typeof__(&(B)[0]) second = (B);                               \
    int equals = 1;                                                 \
    for (size_t i = 0, size = (Size); equals && i < size; ++i)      \
        equals = equals && !Cmp(first + i, second + i)

#  define assert_arrays_eq_cmp_(A, B, Size, Cmp, ...)                   \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        assert_impl(FATAL, equals,                                      \
                .default_msg = "Arrays are not equal",                  \
                __VA_ARGS__);                                           \
    } while (0)

#  define expect_arrays_eq_cmp_(A, B, Size, Cmp, ...)                   \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        assert_impl(NORMAL, equals,                                     \
                .default_msg = "Arrays are not equal",                  \
                __VA_ARGS__);                                           \
    } while (0)

#  define assert_arrays_eq_cmp(...) \
    assert_arrays_eq_cmp_(__VA_ARGS__, .sentinel_ = 0)
#  define expect_arrays_eq_cmp(...) \
    expect_arrays_eq_cmp_(__VA_ARGS__, .sentinel_ = 0)

#  define assert_arrays_neq_cmp_(A, B, Size, Cmp, ...)                  \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        assert_impl(FATAL, !equals,                                     \
                .default_msg = "Arrays not equal",                      \
                __VA_ARGS__);                                           \
    } while (0)

#  define expect_arrays_neq_cmp_(A, B, Size, Cmp, ...)                  \
    do {                                                                \
        CRIT_ARR_COMPARE_(A, B, Size, Cmp, equals);                     \
        assert_impl(NORMAL, equals,                                     \
                .default_msg = "Arrays not equal",                      \
                __VA_ARGS__);                                           \
    } while (0)

#  define assert_arrays_neq_cmp(...) \
    assert_arrays_eq_cmp_(__VA_ARGS__, .sentinel_ = 0)
#  define expect_arrays_neq_cmp(...) \
    expect_arrays_eq_cmp_(__VA_ARGS__, .sentinel_ = 0)
# endif /* !__GNUC__ */

#endif /* !CRITERION_ASSERT_H_ */
