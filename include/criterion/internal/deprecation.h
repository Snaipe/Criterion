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
#ifndef CRITERION_INTERNAL_DEPRECATION_H_
#define CRITERION_INTERNAL_DEPRECATION_H_

#include "preprocess.h"

#ifdef _MSC_VER
# define CR_PRAGMA(Msg)    __pragma(Msg)
#else
# define CR_PRAGMA(Msg)    _Pragma(#Msg)
#endif

#define CR_COMPILE_ERROR(Msg)    CR_COMPILE_ERROR_(Msg)

#if defined (_MSC_VER)
# define CR_COMPILE_ERROR_(Msg)    CR_PRAGMA(message(CR_STR(error: Msg)))
#elif defined (__GNUC__)
# define CR_COMPILE_ERROR_(Msg)    CR_PRAGMA(GCC error #Msg)
#elif __STDC_VERSION__ >= 201112L
# define CR_COMPILE_ERROR_(Msg)    _Static_assert(0, #Msg);
#elif __cplusplus >= 201103L
# define CR_COMPILE_ERROR_(Msg)    static_assert(0, #Msg);
#else
# define CR_COMPILE_ERROR_(Msg)    ((void) sizeof (char[-1]))
#endif

#define CRI_COMPILE_ERROR(...)    ,
#define CR_CHECKERROR__(M, N)     M ## N
#define CR_CHECKERROR_(M, N)      CR_CHECKERROR__(M, N)
#define CR_CHECKERROR(M)          CR_CHECKERROR_(CR_CHECKERROR_, CR_VA_TAIL_SELECT(CR_CHECKERROR_(CRI_, M)))(M)
#define CR_CHECKERROR_1(M)        M
#define CR_CHECKERROR_2(M)        CR_CHECKERROR_(CR_, M)

#define CR_DEPRECATED(Msg)        CR_PRAGMA(message(Msg))

#ifdef __GNUC__
# define CR_DEPRECATED_MEMBER(Member)    Member __attribute__((deprecated))
#else
# define CR_DEPRECATED_MEMBER(Member)    Member
#endif

#endif /* !CRITERION_INTERNAL_DEPRECATION_H_ */
