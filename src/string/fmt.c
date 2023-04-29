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
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include <string.h>

#include "fmt.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))

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

static inline const cri_escape_char *
find_escape_seq(char c, const cri_escape_char *escape, size_t escape_size)
{
    for (size_t i = 0; i < escape_size; ++i) {
        if (c == escape[i].unsafe_char)
            return &escape[i];
    }

    return NULL;
}

static inline bool astr_maybe_expand(char **buf, size_t *buf_size,
        size_t buf_pos, size_t by_len)
{
    if (buf_pos + by_len >= *buf_size) {
        size_t new_buf_size = buf_pos + by_len + 1;
        char *new_buf = realloc(*buf, new_buf_size);
        if (!new_buf)
            return false;

        *buf = new_buf;
        *buf_size = new_buf_size;
        return true;
    }

    return true;
}

static inline bool astr_append(char **buf, size_t *buf_size, size_t *buf_pos,
                const char *str)
{
    size_t str_len = strlen(str);
    if (!astr_maybe_expand(buf, buf_size, *buf_pos, str_len))
        return false;

    strcpy(*buf + *buf_pos, str);
    *buf_pos += str_len;
    (*buf)[*buf_pos] = 0;
    return true;
}

static inline bool is_control_char(char c)
{
    return c < 32;
}

static inline const char *escape_control_char(char c, char *buf, size_t buf_size)
{
    const char *esc;
    switch (c) {
        case '\b':
            esc = "\\b";
            break;
        case '\f':
            esc = "\\f";
            break;
        case '\n':
            esc = "\\n";
            break;
        case '\r':
            esc = "\\r";
            break;
        case '\t':
            esc = "\\t";
            break;
        case '\\':
            esc = "\\\\";
            break;
        default:
            snprintf(buf, buf_size, "\\u%04x", c);
            return buf;
    }

    snprintf(buf, buf_size, "%s", esc);
    return buf;
}

char *cri_escape_str(const char *in, size_t in_len,
        const cri_escape_char *escape, size_t escape_size)
{
    size_t escape_estimate = 0;

    if (escape_size != 0) {
        size_t escape_seq_len = strlen(escape[0].escape_seq);
        /* MAX(4, 10%) */
        escape_estimate = (size_t) MAX(4, in_len * 0.10 * escape_seq_len);
    }

    size_t out_size = in_len + 1 + (escape_estimate * sizeof(char));
    char *out = malloc(out_size);
    if (!out)
        return NULL;

    size_t out_pos = 0;
    out[out_pos] = 0;

    for (size_t in_pos = 0; in_pos < in_len; ++in_pos) {
        char c = in[in_pos];

        const cri_escape_char *esc;
        if ((esc = find_escape_seq(c, escape, escape_size))) {
            if(!astr_append(&out, &out_size, &out_pos, esc->escape_seq))
                goto error;
            continue;
        }

        if (is_control_char(c) || c == '\\') {
            char buf[16];
            if (!astr_append(&out, &out_size, &out_pos, escape_control_char(c, buf, sizeof(buf))))
                goto error;
            continue;
        }

        if (!astr_maybe_expand(&out, &out_size, out_pos, 1))
            goto error;

        out[out_pos] = c;
        out_pos++;
        out[out_pos] = 0;
    }

    return out;

error:
    free(out);
    return NULL;
}
