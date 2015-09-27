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
#include <string.h>
#include <csptr/smalloc.h>
#include "criterion/stats.h"
#include "criterion/common.h"
#include "criterion/hooks.h"
#include "core/worker.h"
#include "event.h"

s_pipe_file_handle *g_event_pipe = NULL;

void destroy_event(void *ptr, UNUSED void *meta) {
    struct event *ev = ptr;
    free(ev->data);
}

void destroy_assert_event(void *ptr, UNUSED void *meta) {
    struct event *ev = ptr;
    free((void*) ((struct criterion_assert_stats *) ev->data)->message);
    free(ev->data);
}

#ifdef __GNUC__
# define unlikely(x) __builtin_expect((x),0)
#else
# define unlikely(x) (x)
#endif

#define ASSERT(Cond)            \
    do {                        \
        if (unlikely(!(Cond)))  \
            abort();            \
    } while (0)

struct event *read_event(s_pipe_file_handle *f) {
    unsigned kind;
    ASSERT(pipe_read(&kind, sizeof (unsigned), f) == 1);

    unsigned long long pid;
    ASSERT(pipe_read(&pid, sizeof (unsigned long long), f) == 1);

    switch (kind) {
        case ASSERT: {
            const size_t assert_size = sizeof (struct criterion_assert_stats);
            struct criterion_assert_stats *buf = NULL;
            char *msg = NULL;

            buf = malloc(assert_size);
            ASSERT(pipe_read(buf, assert_size, f) == 1);

            size_t len = 0;
            ASSERT(pipe_read(&len, sizeof (size_t), f) == 1);

            msg = malloc(len);
            ASSERT(pipe_read(msg, len, f) == 1);

            buf->message = msg;

            struct event *ev = smalloc(
                    .size = sizeof (struct event),
                    .dtor = destroy_assert_event
                );
            *ev = (struct event) { .pid = pid, .kind = kind, .data = buf };
            return ev;
        }
        case THEORY_FAIL: {
            size_t len = 0;
            ASSERT(pipe_read(&len, sizeof (size_t), f) == 1);

            char *buf = malloc(len);
            ASSERT(pipe_read(buf, len, f) == 1);

            struct event *ev = smalloc(
                    .size = sizeof (struct event),
                    .dtor = destroy_event
                );
            *ev = (struct event) { .pid = pid, .kind = kind, .data = buf };
            return ev;
        }
        case POST_TEST: {
            double *elapsed_time = malloc(sizeof (double));
            ASSERT(pipe_read(elapsed_time, sizeof (double), f) == 1);

            struct event *ev = smalloc(
                    .size = sizeof (struct event),
                    .dtor = destroy_event
                );
            *ev = (struct event) { .pid = pid, .kind = kind, .data = elapsed_time };
            return ev;
        }
        case WORKER_TERMINATED: {
            struct worker_status *status = malloc(sizeof (struct worker_status));
            ASSERT(pipe_read(status, sizeof (struct worker_status), f) == 1);

            struct event *ev = smalloc(
                    .size = sizeof (struct event),
                    .dtor = destroy_event
                );
            *ev = (struct event) { .pid = pid, .kind = kind, .data = status };
            return ev;
        }
        default: {
            struct event *ev = smalloc(sizeof (struct event));
            *ev = (struct event) { .pid = pid, .kind = kind, .data = NULL };
            return ev;
        }
    }
}

void send_event(int kind, void *data, size_t size) {
    unsigned long long pid = get_process_id();

    unsigned char *buf = malloc(sizeof (int) + sizeof (pid) + size);
    memcpy(buf, &kind, sizeof (int));
    memcpy(buf + sizeof (int), &pid, sizeof (pid));
    memcpy(buf + sizeof (int) + sizeof (pid), data, size);
    ASSERT(pipe_write(buf, sizeof (int) + sizeof (pid) + size, g_event_pipe) == 1);

    free(buf);
}
