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

# include <stdlib.h>
# include <stdbool.h>
# include "criterion.h"
# include "event.h"

enum criterion_assert_kind {
    NORMAL,
    FATAL
};

# define assertImpl(Kind, Condition, ...)                       \
    do {                                                        \
        int passed = !!(Condition);                             \
        struct criterion_assert_stats stat = {                  \
            .kind = Kind,                                       \
            .condition = #Condition,                            \
            .message = "" __VA_ARGS__,                          \
            .passed = passed,                                   \
            .file = __FILE__,                                   \
            .line = __LINE__,                                   \
        };                                                      \
        send_event(ASSERT, &stat, sizeof (stat));               \
        if (!passed && Kind == FATAL)                           \
            return;                                             \
    } while (0)

# define expect(Condition, ...) assertImpl(NORMAL, Condition, __VA_ARGS__)
# define assert(Condition, ...) assertImpl(FATAL, Condition, __VA_ARGS__)

# define assertArrayEquals(A, B, Size, ...) \
    assertImpl(NORMAL, !memcmp((A), (B), Size), __VA_ARGS__)
# define expectArrayEquals(A, B, Size, ...) \
    assertImpl(FATAL, !memcmp((A), (B), Size), __VA_ARGS__)

#endif /* !CRITERION_ASSERT_H_ */
