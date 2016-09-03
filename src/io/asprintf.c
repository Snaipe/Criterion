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

#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "criterion/internal/asprintf-compat.h"

int cr_asprintf(char **strp, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int res = cr_vasprintf(strp, fmt, ap);
    va_end(ap);
    return res;
}

int cr_vasprintf(char **strp, const char *fmt, va_list ap) {
    va_list vl;
    va_copy(vl, ap);

    int size = vsnprintf(0, 0, fmt, vl);
    int res = -1;

    if (size < 0 || size >= INT_MAX) {
        goto cleanup;
    }

    char *str = malloc(size + 1);
    if (str) {
        int res2 = vsnprintf(str, size + 1, fmt, ap);
        if (res2 < 0 || res2 > size) {
            free(str);
            goto cleanup;
        }
        *strp = str;
        res = res2;
    }

cleanup:
    va_end(vl);
    return res;
}

void cr_asprintf_free(char *buf)
{
    free(buf);
}
