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
#include <pb_encode.h>
#include <pb_decode.h>
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "criterion/internal/common.h"
#include "protocol.h"

static bool write_fd_callback(pb_ostream_t *stream, const uint8_t *buf, size_t count) {
    int fd = (intptr_t) stream->state;
    ssize_t result = write(fd, buf, count);

    if (result < 0)
        return false;
    return (size_t) result == count;
}

static bool read_fd_callback(pb_istream_t *stream, uint8_t *buf, size_t count) {
    int fd = (intptr_t) stream->state;
    ssize_t result = read(fd, buf, count);

    if (result < 0)
        return false;

    if ((size_t) result < count)
        stream->bytes_left = result;
    return (size_t) result == count;
}

pb_ostream_t pb_ostream_from_fd(int fd) {
    pb_ostream_t stream = {&write_fd_callback, (void*)(intptr_t) fd, SIZE_MAX, 0, NULL};
    return stream;
}

pb_istream_t pb_istream_from_fd(int fd) {
    pb_istream_t stream = {&read_fd_callback, (void*)(intptr_t) fd, SIZE_MAX, NULL};
    return stream;
}

volatile bool is_extern_worker = false;
