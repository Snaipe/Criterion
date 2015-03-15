/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
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

#include <stdio.h>
#include <csptr/smart_ptr.h>
#include "criterion/stats.h"
#include "criterion/common.h"
#include "criterion/hooks.h"
#include "event.h"

FILE *g_event_pipe = NULL;

void destroy_event(void *ptr, UNUSED void *meta) {
    struct event *ev = ptr;
    free(ev->data);
}

struct event *read_event(FILE *f) {
    unsigned kind;
    if (fread(&kind, sizeof (unsigned), 1, f) == 0)
        return NULL;

    switch (kind) {
        case ASSERT: {
            const size_t assert_size = sizeof (struct criterion_assert_stats);
            unsigned char *buf = malloc(assert_size);
            if (fread(buf, assert_size, 1, f) == 0)
                return NULL;

            return unique_ptr(struct event, { .kind = kind, .data = buf }, destroy_event);
        }
        case POST_TEST: {
            double *elapsed_time = malloc(sizeof (double));
            if (fread(elapsed_time, sizeof (double), 1, f) == 0)
                return NULL;

            return unique_ptr(struct event, { .kind = kind, .data = elapsed_time }, destroy_event);
        }
        default:
            return unique_ptr(struct event, { .kind = kind, .data = NULL });
    }
}

void send_event(int kind, void *data, size_t size) {
    unsigned char buf[sizeof (int) + size];
    memcpy(buf, &kind, sizeof (int));
    memcpy(buf + sizeof (int), data, size);
    if (fwrite(buf, sizeof (int) + size, 1, g_event_pipe) == 0)
        abort();
}
