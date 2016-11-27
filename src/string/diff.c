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
#include <git2/diff.h>
#include <string.h>

#include "diff.h"
#include "fmt.h"

struct cri_diff_context {
    struct cri_diff_buffer *buf;
    size_t offset;
};

static int process_line(const git_diff_delta *delta, const git_diff_hunk *hunk,
        const git_diff_line *line, void *data)
{
    (void) delta;
    (void) hunk;
    (void) data;

    struct cri_diff_context *ctx = data;

    return cri_fmt_bprintf(&ctx->buf->ptr, &ctx->offset, &ctx->buf->size,
                   "%c%.*s", line->origin, (int) line->content_len,
                   line->content);
}

static int process_hunk(const git_diff_delta *delta, const git_diff_hunk *h,
        void *data)
{
    (void) delta;

    struct cri_diff_context *ctx = data;

    return cri_fmt_bprintf(&ctx->buf->ptr, &ctx->offset, &ctx->buf->size,
                   "@@ -%d,%d +%d,%d @@\n", h->old_start, h->old_lines,
                   h->new_start, h->new_lines);
}

int cri_diff_buffer_to_buffer(const struct cri_diff_buffer *b1,
        const struct cri_diff_buffer *b2, struct cri_diff_buffer *out)
{
    memset(out, 0, sizeof (*out));
    struct cri_diff_context ctx = {
        .buf    = out,
        .offset = 0,
    };
    return git_diff_buffers(b1->ptr, b1->size, "old",
                   b2->ptr, b2->size, "new",
                   NULL, NULL, NULL, process_hunk, process_line, &ctx);
}
