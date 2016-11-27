/*
 * The MIT License (MIT)
 *
 * Copyright © 2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#include <stddef.h>
#include <inttypes.h>
#include <ctype.h>
#include "common.h"

#define GROUP_BY          4
#define BYTES_PER_LINE    16

CR_API char *cri_string_xxd(const void *data, size_t size)
{
    const size_t maxsz = align2_up(size, BYTES_PER_LINE);
    const size_t nb_lines = maxsz / BYTES_PER_LINE;
    const size_t nb_spaces = nb_lines * (BYTES_PER_LINE / GROUP_BY + 2);
    const size_t len_addr = (sizeof (void *) * 2 + 1) * nb_lines;
    const size_t len_hex = maxsz * 2;

    char *out = malloc(maxsz + len_hex + nb_spaces + len_addr + nb_lines + 1);
    const char *c = data;
    size_t written = 0;

    for (size_t i = 0; i < size;) {
        written += sprintf(&out[written], "%016" PRIx64 ":", (uint64_t) i);

        size_t j = 0;
        for (; j < BYTES_PER_LINE && i + j < size; ++j) {
            if (j % GROUP_BY == 0)
                out[written++] = ' ';
            written += sprintf(&out[written], "%02hhx", c[i + j]);
        }
        size_t left = BYTES_PER_LINE - j;
        for (size_t k = (left + 1) * 2 + left / GROUP_BY; k > 0; --k)
            out[written++] = ' ';
        for (size_t k = 0; k < j; ++k)
            out[written++] = isprint(c[i + k]) ? c[i + k] : '.';
        for (size_t k = BYTES_PER_LINE - j; k > 0; --k)
            out[written++] = ' ';
        out[written++] = '\n';
        i += j;
    }
    out[written] = '\0';
    return out;
}
