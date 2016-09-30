/*
 * The MIT License (MIT)
 *
 * Copyright © 2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef UNWIND_H_
#define UNWIND_H_

#include <stdint.h>
#include <setjmp.h>
#include "config.h"
#include "common.h"

#if defined (HAVE_GNU_UNWINDER)
# include <unwind.h>
#elif defined (HAVE_MS_UNWINDER)
# include <windows.h>
#endif

#if defined (HAVE_ADDRESS_OF_RETURN_ADDRESS)
# include <intrin.h>
#endif

/* We can't define this in a function since we would need to return */
#if defined (HAVE_BUILTIN_DWARF_CFA)
# define cri_unwind_setjmp(Buf)                     \
    ((void) ((Buf)->frame = __builtin_dwarf_cfa()), \
    setjmp((Buf)->buf))
#elif defined (HAVE_ADDRESS_OF_RETURN_ADDRESS)
# define cri_unwind_setjmp(Buf)                         \
    ((void) ((Buf)->frame = _AddressOfReturnAddress()), \
    setjmp((Buf)->buf))
#else
# define cri_unwind_setjmp(Buf)    setjmp((Buf)->buf)
#endif

struct cri_unwind_jmp_buf {
    jmp_buf buf;
    void *frame;
};

CR_NORETURN void cri_unwind_longjmp(struct cri_unwind_jmp_buf *jmp, int val);

#endif /* !UNWIND_H_ */
