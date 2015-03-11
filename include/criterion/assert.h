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
# include "criterion.h"
# include "stats.h"
# include "hooks.h"
# include "event.h"

enum criterion_assert_kind {
    NORMAL,
    FATAL
};

# define assert_impl(Kind, Condition, ...)                       \
    do {                                                        \
        int passed = !!(Condition);                             \
        struct criterion_assert_stats stat = {                  \
            .kind = (Kind),                                     \
            .condition = #Condition,                            \
            .message = "" __VA_ARGS__,                          \
            .passed = passed,                                   \
            .file = __FILE__,                                   \
            .line = __LINE__,                                   \
        };                                                      \
        send_event(ASSERT, &stat, sizeof (stat));               \
        if (!passed && (Kind) == FATAL)                         \
            return;                                             \
    } while (0)

// Common asserts

# define assert(Condition, ...) assert_impl(FATAL, Condition, "" __VA_ARGS__)
# define expect(Condition, ...) assert_impl(NORMAL, Condition, "" __VA_ARGS__)

# define assert_not(Condition, ...) assert(!(Condition), "" __VA_ARGS__)
# define expect_not(Condition, ...) expect(!(Condition), "" __VA_ARGS__)

// Native asserts

# define assert_op(Actual, Expected, Op, ...) \
    assert((Actual) Op (Expected), "" __VA_ARGS__)
# define expect_op(Actual, Expected, Op, ...) \
    expect((Actual) Op (Expected), "" __VA_ARGS__)

# define assert_equal(Actual, Expected, ...) \
    assert_op(Actual, Expected, ==, "" __VA_ARGS__)
# define expect_equal(Actual, Expected, ...) \
    expect_op(Actual, Expected, ==, "" __VA_ARGS__)

# define assert_not_equal(Actual, Expected, ...) \
    assert_op(Actual, Expected, !=, "" __VA_ARGS__)
# define expectNotEqual(Actual, Expected, ...) \
    expect_op(Actual, Expected, !=, "" __VA_ARGS__)

# define assert_lt(Actual, Expected, ...) \
    assert_op(Actual, Expected, <, "" __VA_ARGS__)
# define expect_lt(Actual, Expected, ...) \
    expect_op(Actual, Expected, <, "" __VA_ARGS__)

# define assert_gt(Actual, Expected, ...) \
    assert_op(Actual, Expected, >, "" __VA_ARGS__)
# define expect_gt(Actual, Expected, ...) \
    expect_op(Actual, Expected, >, "" __VA_ARGS__)

# define assert_leq(Actual, Expected, ...) \
    assert_op(Actual, Expected, <=, "" __VA_ARGS__)
# define expect_leq(Actual, Expected, ...) \
    expect_op(Actual, Expected, <=, "" __VA_ARGS__)

# define assert_geq(Actual, Expected, ...) \
    assert_op(Actual, Expected, >=, "" __VA_ARGS__)
# define expect_geq(Actual, Expected, ...) \
    expect_op(Actual, Expected, >=, "" __VA_ARGS__)

// Floating-point asserts

# define assert_float_equal(Actual, Expected, Epsilon, ...) \
    assert((Expected) - (Actual) <= (Epsilon) && (Actual) - (Expected) <= (Epsilon), "" __VA_ARGS__)
# define expect_float_equal(Actual, Expected, Epsilon, ...) \
    expect((Expected) - (Actual) <= (Epsilon) && (Actual) - (Expected) <= (Epsilon), "" __VA_ARGS__)

# define assert_float_not_equal(Actual, Expected, Epsilon, ...) \
    assert((Expected) - (Actual) > (Epsilon) || (Actual) - (Expected) > (Epsilon), "" __VA_ARGS__)
# define expect_float_not_equal(Actual, Expected, Epsilon, ...) \
    expect((Expected) - (Actual) > (Epsilon) || (Actual) - (Expected) > (Epsilon), "" __VA_ARGS__)

// String asserts

# define assert_strings(Actual, Expected, Op, ...) \
    assert(strcmp((Actual), (Expected)) Op 0, "" __VA_ARGS__)
# define expect_strings(Actual, Expected, Op, ...) \
    expect(strcmp((Actual), (Expected)) Op 0, "" __VA_ARGS__)

# define assert_strings_equal(Actual, Expected, ...) \
    assert_strings(Actual, Expected, ==, "" __VA_ARGS__)
# define expect_strings_equal(Actual, Expected, ...) \
    expect_strings(Actual, Expected, ==, "" __VA_ARGS__)

# define assert_strings_gt(Actual, Expected, ...) \
    assert_strings(Actual, Expected, >, "" __VA_ARGS__)
# define expect_strings_gt(Actual, Expected, ...) \
    expect_strings(Actual, Expected, >, "" __VA_ARGS__)

# define assert_strings_lt(Actual, Expected, ...) \
    assert_strings(Actual, Expected, <, "" __VA_ARGS__)
# define expect_strings_lt(Actual, Expected, ...) \
    expect_strings(Actual, Expected, <, "" __VA_ARGS__)

# define assert_strings_geq(Actual, Expected, ...) \
    assert_strings(Actual, Expected, >=, "" __VA_ARGS__)
# define expect_strings_geq(Actual, Expected, ...) \
    expect_strings(Actual, Expected, >=, "" __VA_ARGS__)

# define assert_strings_leq(Actual, Expected, ...) \
    assert_strings(Actual, Expected, <=, "" __VA_ARGS__)
# define expect_strings_leq(Actual, Expected, ...) \
    expect_strings(Actual, Expected, <=, "" __VA_ARGS__)

# define assert_strings_not_equal(Actual, Expected, ...) \
    assert_strings(Actual, Expected, !=, "" __VA_ARGS__)
# define expect_strings_not_equal(Actual, Expected, ...) \
    expect_strings(Actual, Expected, !=, "" __VA_ARGS__)

// Array asserts

# define assert_arrays_equal(A, B, Size, ...) \
    assert(!memcmp((A), (B), (Size)), "" __VA_ARGS__)
# define expect_arrays_equal(A, B, Size, ...) \
    expect(!memcmp((A), (B), (Size)), "" __VA_ARGS__)

# define assert_arrays_not_equal(A, B, Size, ...) \
    assert(memcmp((A), (B), (Size)), "" __VA_ARGS__)
# define expect_arrays_not_equal(A, B, Size, ...) \
    expect(memcmp((A), (B), (Size)), "" __VA_ARGS__)

#endif /* !CRITERION_ASSERT_H_ */
