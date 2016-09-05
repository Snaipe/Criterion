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
#ifndef INTERNAL_H_
#define INTERNAL_H_

#if defined (_WIN32) && !defined (__CYGWIN__)
# include <windows.h>

# if defined (MINGW_DEFINE_OFF_T) && (defined (__MINGW32__) || defined (__MINGW64__))
#  include "off_t.h"

#  if !defined (__MINGW64__)
#   define off_t     cr_off32
#  else
#   define off_t     cr_off64
#  endif
#  define off64_t    cr_off64
# endif
# include <io.h>
# if defined (MINGW_DEFINE_OFF_T) && (defined (__MINGW32__) || defined (__MINGW64__))
#  undef off_t
#  undef off64_t
# endif
# include <fcntl.h>
# include <winnt.h>
# include <stdint.h>
# include <signal.h>
#else
# include <fcntl.h>
# include <signal.h>
# include <sys/param.h>
# include <sys/wait.h>
# include <unistd.h>
# ifdef BSD
#  include <sys/types.h>
typedef unsigned long u_long;
typedef unsigned int u_int;
typedef unsigned short u_short;
typedef unsigned char u_char;
#  include <sys/sysctl.h>
# endif
#endif

#include "posix.h"

#endif /* !INTERNAL_H_ */
