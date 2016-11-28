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
#ifndef SETJMP_H_
#define SETJMP_H_

#include <setjmp.h>
#include "common.h"

/* We need to replace the garbage implementation of setjmp in the Windows CRT
   that unwinds even though longjmp is not supposed to. */
#if defined (_WIN32)
# if defined (__GNUC__)
#  define cri_setjmp     __builtin_setjmp
#  define cri_longjmp    __builtin_longjmp
# else
int bxfi_setjmp(jmp_buf);
CR_NORETURN void bxfi_longjmp(jmp_buf, int);
/* Fallback on boxfort internal implementations on windows */
#  define cri_setjmp     bxfi_setjmp
#  define cri_longjmp    bxfi_longjmp
# endif
#else
# define cri_setjmp     setjmp
# define cri_longjmp    longjmp
#endif

#endif /* !SETJMP_H_ */
