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
#include <assert.h>
#include <errno.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <string.h>

#include "compat/process.h"
#include "core/worker.h"

#define URL "ipc://%scriterion_%llu.sock"

#define errno_ignore(Stmt) do { int err = errno; Stmt; errno = err; } while (0)

int bind_server(void) {

    int sock = nn_socket(AF_SP, NN_REP);
    if (sock < 0)
        return -1;

#ifdef VANILLA_WIN32
    char *prefix = "";
#else
    char *prefix = "/tmp/";
#endif

    char url[256];
    int rc = snprintf(url, sizeof (url), URL, prefix, get_process_id());
    if (rc == 256)
        cr_panic("IPC url too long");

    if (nn_bind(sock, url) < 0)
        goto error;

    return sock;

error: {}
    errno_ignore(nn_close(sock));
    return -1;
}

int connect_client(void) {
    if (is_single_mode())
        return 0;

    int sock = nn_socket(AF_SP, NN_REQ);
    if (sock < 0)
        return -1;

#ifdef VANILLA_WIN32
    char *prefix = "";
#else
    char *prefix = "/tmp/";
#endif

    char url[256];
    int rc = snprintf(url, sizeof (url), URL, prefix, get_runner_process_id());
    if (rc == 256)
        cr_panic("IPC url too long");

    if (nn_connect (sock, url) < 0)
        goto error;

    return sock;

error: {}
    errno_ignore(nn_close(sock));
    return -1;
}

void close_socket(int sock) {
    if (is_single_mode())
        return;

    nn_close(sock);
}
