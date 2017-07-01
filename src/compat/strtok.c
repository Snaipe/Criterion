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
#include <string.h>

#include "criterion/internal/asprintf-compat.h"
#include "config.h"
#include "mutex.h"
#include "strtok.h"

#define DEFINE_STRTOK(Kind, Type)                             \
    Type * cri_ ## Kind ## tokc(Type * str, const Type delim) \
    {                                                         \
        static tls Type *state = NULL;                        \
        if (str)                                              \
            state = str;                                      \
        if (!state)                                           \
            return NULL;                                      \
                                                              \
        Type *tok = state;                                    \
        Type *found = Kind ## chr(state, delim);              \
        if (found) {                                          \
            *found = 0;                                       \
            state = found + 1;                                \
        } else {                                              \
            state = NULL;                                     \
        }                                                     \
        return tok;                                           \
    }

DEFINE_STRTOK(str, char)
DEFINE_STRTOK(wcs, wchar_t)

#if defined (HAVE_STRTOK_R)
# define strtok_r    strtok_r
#elif defined (HAVE_STRTOK_S)
# define strtok_r    strtok_s
#else
# error No strtok_r implementation
#endif

char *cri_strtok_r(char *str, const char *delim, char **saveptr) {
    return strtok_r(str, delim, saveptr);
}
