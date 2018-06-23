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
#ifndef POSIX_COMPAT_H_
#define POSIX_COMPAT_H_

#if defined (_WIN32) && !defined (__CYGWIN__)
# define VANILLA_WIN32
#endif

#if defined (BSD)            \
    || defined (__FreeBSD__) \
    || defined (__NetBSD__)  \
    || defined (__OpenBSD__) \
    || defined (__DragonFly__)
# define OS_BSD    1
#endif

#if !defined (_POSIX_SOURCE)
# define _POSIX_SOURCE    1
# define TMP_POSIX
#endif
#if defined (MINGW_DEFINE_OFF_T) && (defined (__MINGW32__) || defined (__MINGW64__))
# include "off_t.h"

# if !defined (__MINGW64__)
#  define off_t     cr_off32
# else
#  define off_t     cr_off64
# endif
# define off64_t    cr_off64
#endif
#include <stdio.h>
#include <string.h>
#if defined (MINGW_DEFINE_OFF_T) && defined (__MINGW32__) || defined (__MINGW64__)
# undef off_t
# undef off64_t
#endif
#ifdef TMP_POSIX
# undef _POSIX_SOURCE
# undef TMP_POSIX
#endif

#ifdef VANILLA_WIN32
# define WEXITSTATUS(Status)    (((Status) & 0xFF00) >> 8)
# define WTERMSIG(Status)       ((Status) & 0x7F)
# define WIFEXITED(Status)      (WTERMSIG(Status) == 0)
# define WIFSIGNALED(Status)    (((signed char) (WTERMSIG(Status) + 1) >> 1) > 0)

# define SIGPROF    27
# define SIGSYS     31

/*
 * We define a custom exception code following the ntstatus specification:
 *
 *   1110 1111 1111 1111 1111 1111 1111 1111
 *   -||| ------------------| -------------|
 *    |||                   |              `- Code: Unspecified     (-1)
 *    |||                   `---------------- Facility: Unspecified (-1)
 *    ||`------------------------------------ Reserved              (0b0)
 *    |`------------------------------------- Customer code: Yes    (0b1)
 *    `-------------------------------------- Severity: Error       (0b11)
 */
# define CR_EXCEPTION_TIMEOUT    0xEFFFFFFF

#else
# include <sys/wait.h>
#endif

#include "compat/pipe.h"
#include "compat/section.h"
#include "compat/process.h"
#include "compat/path.h"

#endif /* !POSIX_COMPAT_H_ */
