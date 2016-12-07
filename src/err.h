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
#ifndef ERR_H_
#define ERR_H_

#include <errno.h>

#include "common.h"

CR_FORMAT(printf, 1, 2)
CR_NORETURN void cr_panic(const char *str, ...);

#define errno_ignore(Stmt) \
    do {                   \
        int err = errno;   \
        Stmt;              \
        errno = err;       \
    } while (0)

#define errno_return(Err, Val) \
    do {                       \
        errno = (Err);         \
        return (Val);          \
    } while (0)

#undef assert
#define assert(Cnd)                                                          \
    do {                                                                     \
        if (!(Cnd)) {                                                        \
            cr_panic("Assertion failed: %s\n\t@ %s:%d (%s)", #Cnd, __FILE__, \
                    __LINE__, __func__);                                     \
        }                                                                    \
    } while (0)

#ifdef NDEBUG
# define debug_assert(Cnd)
#else
# define debug_assert(Cnd)    assert(Cnd)
#endif

#endif /* !ERR_H_ */
