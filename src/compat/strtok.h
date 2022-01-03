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
#ifndef COMPAT_STRTOK_H_
#define COMPAT_STRTOK_H_

#include <wchar.h>
#include "config.h"

#ifdef VANILLA_WIN32
# if !defined (HAVE_STRTOK_R)
#  if defined (HAVE_STRTOK_S)
#   define strtok_r    strtok_s
#  else
static CR_INLINE char *strtok_r(char *str, const char *delim, CR_UNUSED char **saveptr)
{
    return strtok(str, delim);
}
#  endif
# endif

# ifdef _MSC_VER
#  define strdup    _strdup
# endif
#endif

char *cri_strtokc(char *str, const char delim);
wchar_t *cri_wcstokc(wchar_t *str, const wchar_t delim);

#endif /* !COMPAT_STRTOK_H_ */
