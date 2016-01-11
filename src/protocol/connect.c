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
#include <errno.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>

#define URL "ipc://criterion.sock"

#define errno_ignore(Stmt) do { int err = errno; Stmt; errno = err; } while (0)

int bind_server(void) {
    int fstrat = NN_FORK_RESET;
    nn_setopt(NN_FORK_STRATEGY, &fstrat, sizeof (fstrat));

    int sock = nn_socket(AF_SP, NN_REP);
    if (sock < 0)
        return -1;

    if (nn_bind(sock, URL) < 0)
        goto error;

    return sock;

error: {}
    errno_ignore(nn_close(sock));
    return -1;
}

int connect_client(void) {
    int sock = nn_socket(AF_SP, NN_REQ);
    if (sock < 0)
        return -1;

    if (nn_connect (sock, URL) < 0)
        goto error;

    return sock;

error: {}
    errno_ignore(nn_close(sock));
    return -1;
}

void close_socket(int sock) {
    nn_close(sock);
}
