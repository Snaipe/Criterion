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
#ifndef CRITERION_INTERNAL_DETECT_H_
#define CRITERION_INTERNAL_DETECT_H_

/* Standard version checks */
#if !defined (CR_NO_COMPILER_CHECK)
# if defined (__cplusplus)
/* Is the compiler a >C++11 compiler ?*/
#  if defined (_MSC_VER)
#   if _MSC_VER < 1800
#    error                                                                  \
     Your version of MSVC is too old, please compile your tests using       \
     a c++11 compiler, like MinGW or MSVC 14.0+ (Included in visual studio  \
     2015)
#   endif
#  elif __cplusplus < 201103L
#   error The Criterion C++ API needs a C++11 compiler or later.
#  endif
# elif defined (__STDC__) || defined (_MSC_VER)
/* Is the compiler a supported c99 compiler? */
#  if defined (_MSC_VER)
#   if _MSC_VER < 1900
#    error                                                                  \
     Your version of MSVC is too old, please compile your tests using       \
     a c99 compiler, like MinGW or MSVC 14.0+ (Included in visual studio    \
     2015)
#   endif
#  elif __STDC_VERSION__ < 199901L
#   error The Criterion C API needs a C99 compiler or later.
#  endif
# else
#  error The Criterion C/C++ API needs a C or C++ compiler.
# endif
#endif /* !CR_NO_COMPILER_CHECK */

/* Does the compiler support section attributes? */
#if !defined (__clang__) && !defined (__GNUC__) && !defined (_MSC_VER)
# error Criterion needs a compiler that supports section attributes.
#endif

/* Does the compiler support complex numbers? */
#if defined (__cplusplus) || (__STDC_VERSION__ >= 199901L && !defined (__STDC_NO_COMPLEX__))
# define CRI_CAPS_COMPLEX
#endif

/* Does the compiler support auto types? */
#if defined (__cplusplus)
# define CRI_AUTOTYPE    auto
# define CRI_CAPS_AUTOTYPE
#elif defined (__GNUC__)
# define CRI_AUTOTYPE    __extension__ __auto_type
# define CRI_CAPS_AUTOTYPE
#else
# define CRI_AUTOTYPE
#endif

/* Does the compiler represent long doubles as regular doubles? */
#if defined (_MSC_VER) || (defined (__MINGW32__) && !defined (__USE_MINGW_ANSI_STDIO))
# define CRI_CAPS_LDBL_IS_DBL
#endif

#endif /* !CRITERION_INTERNAL_DETECT_H_ */
