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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>

#include "fmt.h"

int cri_fmt_bprintf(char **buf, size_t *offset, size_t *sz,
        const char *fmt, ...)
{
    va_list vl;

    va_start(vl, fmt);
    int rc = cri_fmt_vbprintf(buf, offset, sz, fmt, vl);
    va_end(vl);
    return rc;
}

int cri_fmt_vbprintf(char **buf, size_t *offset, size_t *sz,
        const char *fmt, va_list ap)
{
    va_list vl;
    size_t offset_start = 0;

    if (!offset)
        offset = &offset_start;

    va_copy(vl, ap);
#ifdef _WIN32
    int size = _vscprintf(fmt, vl);
#else
    int size = vsnprintf(0, 0, fmt, vl);
#endif
    va_end(vl);

    if (size < 0) {
        return -errno;
    }

    size_t nsz = *sz + size;

    char *nb = realloc(*buf, nsz + 1);
    if (!nb) {
        return -errno;
    }
    *buf = nb;

    vsnprintf(&nb[*offset], size + 1, fmt, ap);

    *offset += size;
    *sz = nsz;
    return 0;
}

int cr_asprintf(char **strp, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    int res = cr_vasprintf(strp, fmt, ap);
    va_end(ap);
    return res;
}

int cr_vasprintf(char **strp, const char *fmt, va_list ap)
{
    size_t size = 0;
    char *out = NULL;
    int rc = cri_fmt_vbprintf(&out, NULL, &size, fmt, ap);
    if (rc < 0)
        return rc;
    if (size > (size_t) INT_MAX)
        return -EOVERFLOW;
    *strp = out;
    return size;
}

void cr_asprintf_free(char *buf)
{
    free(buf);
}
