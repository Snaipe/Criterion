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
#include <nanomsg/nn.h>
#include <stdlib.h>
#include "protocol/protocol.h"

int read_message(int sock, criterion_protocol_msg *message) {
    int res;
    unsigned char *buf;
    int read = res = nn_recv(sock, &buf, NN_MSG, 0);

    if (read <= 0)
        goto cleanup;

    pb_istream_t stream = pb_istream_from_buffer(buf, read);
    if (!pb_decode(&stream, criterion_protocol_msg_fields, message)) {
        res = -2;
        goto cleanup;
    }

    res = 1;
cleanup:
    nn_freemsg(buf);
    return res;
}

int write_message(int sock, const criterion_protocol_msg *message) {
    int res = -1;
    size_t size;
    unsigned char *buf = NULL;
    if (!pb_get_encoded_size(&size, criterion_protocol_msg_fields, message))
        goto cleanup;

    buf = malloc(size);
    pb_ostream_t stream = pb_ostream_from_buffer(buf, size);
    if (!pb_encode(&stream, criterion_protocol_msg_fields, message))
        goto cleanup;

    int written = nn_send(sock, buf, size, 0);
    if (written <= 0 || written != (int) size)
        goto cleanup;

    res = 1;
cleanup:
    free(buf);
    return res;
}
