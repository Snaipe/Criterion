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
#include <errno.h>
#include <zmq.h>
#include "connect.h"

#define URL "ipc://criterion.sock"

#define errno_ignore(Stmt) do { int err = errno; Stmt; errno = err; } while (0)

static void *zmq_ctx;

void cr_transport_init (void) {
    zmq_ctx = zmq_ctx_new ();
}

void cr_transport_term (void) {
    zmq_ctx_destroy (zmq_ctx);
}

cr_socket bind_server(void) {
    cr_socket sock = zmq_socket(zmq_ctx, ZMQ_REP);
    if (sock == NULL)
        goto error;

    if (zmq_bind(sock, URL) < 0)
        goto error;

    return sock;
error: {}
    if (sock)
        errno_ignore(zmq_close(sock));
    return NULL;
}

cr_socket connect_client(void) {
    cr_socket sock = zmq_socket(zmq_ctx, ZMQ_REQ);
    if (sock == NULL)
        goto error;

    if (zmq_connect (sock, URL) < 0)
        goto error;

    return sock;
error: {}
    if (sock)
        errno_ignore(zmq_close(sock));
    return NULL;
}

void close_socket(cr_socket sock) {
    zmq_close(sock);
}
