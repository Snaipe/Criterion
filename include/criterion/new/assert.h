/*
 * The MIT License (MIT)
 *
 * Copyright © 2017 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_NEW_ASSERT_H_
#define CRITERION_NEW_ASSERT_H_
#ifdef CRITERION_DOCS

/* Assertion API */

#define cr_assert(Specifier, Format, ...)

#define cr_expect(Specifier, Format, ...)

/* Specifiers */

#define not(Condition)
#define all(...)
#define any(...)
#define none(...)

#define eq(Tag, Actual, Expected)
#define ne(Tag, Actual, Unexpected)
#define lt(Tag, Actual, Reference)
#define le(Tag, Actual, Reference)
#define gt(Tag, Actual, Reference)
#define ge(Tag, Actual, Reference)

#define ieee_ulp_eq(Tag, Actual, Expected, Ulp)
#define ieee_ulp_ne(Tag, Actual, Expected, Ulp)

#define epsilon_eq(Tag, Actual, Expected, Epsilon)
#define epsilon_ne(Tag, Actual, Expected, Epsilon)

/* Tags */

#define i8
#define i16
#define i32
#define i64
#define u8
#define u16
#define u32
#define u64
#define sz
#define ptr
#define iptr
#define uptr
#define chr
#define int
#define uint
#define long
#define ulong
#define llong
#define ullong
#define flt
#define dbl
#define ldbl
#define cx_flt
#define cx_dbl
#define cx_ldbl
#define mem
#define str
#define wcs
#define tcs
#define type(UserType)

#endif /* !CRITERION_DOCS */

#include "memory.h"

/* Internals */

#include "../internal/new_asserts.h"

#endif /* !CRITERION_NEW_ASSERT_H_ */
