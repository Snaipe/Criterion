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
#ifndef ATOMIC_H_
#define ATOMIC_H_

#include <stdint.h>

#ifdef __GNUC__
# define cri_atomic_add(Val, Add)    __sync_fetch_and_add(&(Val), (Add))
#else
# define cri_atomic_add(Val, Add)    cri_atomic_add_impl((uintptr_t *) &(Val), (Add))

# if defined (_WIN32)
#  include <windows.h>
# endif

static inline uintptr_t cri_atomic_add_impl(uintptr_t *val, uintptr_t add)
{
# if defined (_WIN64)
    return InterlockedExchangeAdd64(val, add);
# elif defined (_WIN32)
    return InterlockedExchangeAdd(val, add);
# else
#  error Atomics are not supported on your system
# endif
}
#endif

#endif /* !ATOMIC_H_ */
