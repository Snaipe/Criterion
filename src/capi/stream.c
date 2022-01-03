/*
 * The MIT License (MIT)
 *
 * Copyright © 2018 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#include <stdlib.h>
#include <string.h>

#include "criterion/new/stream.h"
#include "string/xxd.h"

static int streams_eval_cmp(struct cr_stream *m1, struct cr_stream *m2)
{
    /* It doesn't make sense to compare consumed streams, but it still
       happens in some case within an assertion. It's best to provide
       consistent results, so we simply cache the comparison results,
       and forbid comparing a previously compared stream with anything
       else. */
    if (m1->cri_data->dirty) {
        return m1->cri_data->cmp;
    }
    if (m2->cri_data->dirty) {
        return -m2->cri_data->cmp;
    }

    int cmp = 0;
    size_t r1 = 0, r2 = 0;

    do {
        m1->cri_data->cursor += r1;
        m2->cri_data->cursor += r2;

        r1 = CR_STREAM_BUFSIZE;
        r2 = CR_STREAM_BUFSIZE;

        cmp = -1;
        if (m1->read(m1->cookie, m1->cri_data->buffer, &r1) != 0) {
            break;
        }

        cmp = 1;
        if (m2->read(m2->cookie, m2->cri_data->buffer, &r2) != 0) {
            break;
        }

        m1->cri_data->remaining = r1;
        m2->cri_data->remaining = r2;

        cmp = r1 - r2;
        if (!cmp) {
            cmp = memcmp(m1->cri_data->buffer, m2->cri_data->buffer, r1);
        }
    } while (r1 && !cmp);

    m1->cri_data->dirty = 1;
    m1->cri_data->cmp = cmp;
    m2->cri_data->dirty = 1;
    m2->cri_data->cmp = -cmp;

    return cmp;
}

int cr_user_stream_eq(struct cr_stream *m1, struct cr_stream *m2)
{
    return !streams_eval_cmp(m1, m2);
}

int cr_user_stream_lt(struct cr_stream *m1, struct cr_stream *m2)
{
    return streams_eval_cmp(m1, m2) < 0;
}

char *cr_user_stream_tostr(const struct cr_stream *m)
{
    return cri_string_xxd(m->cri_data->buffer, m->cri_data->cursor, m->cri_data->remaining);
}

void cr_stream_init(struct cr_stream *s)
{
    s->cri_data = calloc(sizeof (*s->cri_data), 1);
    s->cri_data->ref = 1;
}

void cr_stream_close(struct cr_stream *s)
{
    s->cri_data->ref--;
    if (s->cri_data->ref > 0)
        return;
    if (s->close)
        s->close(s->cookie);
    free(s->cri_data);
}
