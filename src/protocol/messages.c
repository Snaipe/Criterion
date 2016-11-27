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
#include <nanomsg/nn.h>
#include <stdlib.h>
#include "protocol/protocol.h"
#include "log/logging.h"
#include "io/event.h"
#include "string/fmt.h"
#include "mutex.h"

int read_message(int sock, criterion_protocol_msg *message)
{
    int read;
    unsigned char *buf = NULL;

    do {
        read = nn_recv(sock, &buf, NN_MSG, 0);
    } while (read < 0 && errno == EINTR);

    if (read <= 0)
        goto cleanup;

    pb_istream_t stream = pb_istream_from_buffer(buf, read);
    if (!pb_decode(&stream, criterion_protocol_msg_fields, message)) {
        read = -2;
        goto cleanup;
    }

    read = 1;
cleanup:
    if (buf)
        nn_freemsg(buf);
    return read;
}

int write_message(int sock, const criterion_protocol_msg *message)
{
    int res = -1;
    size_t size;
    unsigned char *buf = NULL;

    if (!pb_get_encoded_size(&size, criterion_protocol_msg_fields, message))
        goto cleanup;

    buf = malloc(size);
    pb_ostream_t stream = pb_ostream_from_buffer(buf, size);
    if (!pb_encode(&stream, criterion_protocol_msg_fields, message))
        goto cleanup;

    int written;

    do {
        written = nn_send(sock, buf, size, 0);
    } while (written < 0 && errno == EINTR);

    if (written <= 0 || written != (int) size)
        goto cleanup;

    res = 1;
cleanup:
    free(buf);
    return res;
}

const char *message_names[] = {
    [criterion_protocol_submessage_birth_tag]   = "birth",
    [criterion_protocol_submessage_phase_tag]   = "phase",
    [criterion_protocol_submessage_death_tag]   = "death",
    [criterion_protocol_submessage_message_tag] = "message",
    [criterion_protocol_submessage_assert_tag]  = "assert",
};

void cr_send_to_runner(const criterion_protocol_msg *message)
{
    static struct cri_mutex sync;
    int err = cri_mutex_init_once(&sync);

    if (err < 0) {
        criterion_perror("Could not initialize the global message mutex: %s.\n",
                strerror(-err));
        abort();
    }

    err = cri_mutex_lock(&sync);
    if (err < 0) {
        criterion_perror("Could not lock the global message mutex: %s.\n",
                strerror(-err));
        abort();
    }

    if (write_message(g_client_socket, message) != 1) {
        criterion_perror("Could not write the \"%s\" message down the event pipe: %s.\n",
                message_names[message->data.which_value],
                nn_strerror(errno));
        abort();
    }

    unsigned char *buf = NULL;
    int read;

    do {
        read = nn_recv(g_client_socket, &buf, NN_MSG, 0);
    } while (read < 0 && errno == EINTR);

    err = cri_mutex_unlock(&sync);
    if (err < 0) {
        criterion_perror("Could not unlock the global message mutex: %s.\n",
                strerror(-err));
        abort();
    }

    if (read <= 0) {
        criterion_perror("Could not read ack: %s.\n", nn_strerror(errno));
        abort();
    }

    criterion_protocol_ack ack;
    pb_istream_t stream = pb_istream_from_buffer(buf, read);
    if (!pb_decode(&stream, criterion_protocol_ack_fields, &ack)) {
        criterion_perror("Could not decode ack: %s.\n", PB_GET_ERROR(&stream));
        abort();
    }

    if (ack.status_code != criterion_protocol_ack_status_OK) {
        criterion_perror("Runner returned an error: %s.\n", ack.message ? ack.message : "Unknown error");
        abort();
    }
    pb_release(criterion_protocol_ack_fields, &ack);

    if (buf)
        nn_freemsg(buf);
}

void send_ack(int sock, bool ok, const char *msg, ...)
{
    criterion_protocol_ack ack;

    ack.status_code = ok ? criterion_protocol_ack_status_OK : criterion_protocol_ack_status_ERROR;
    ack.message = NULL;

    if (!ok) {
        va_list ap;
        va_start(ap, msg);
        if (cr_vasprintf(&ack.message, msg, ap) < 0)
            ack.message = NULL;
        va_end(ap);
    }

    size_t size;
    unsigned char *buf = NULL;
    if (!pb_get_encoded_size(&size, criterion_protocol_ack_fields, &ack)) {
        criterion_perror("Could not calculate the size of an ack.\n");
        abort();
    }

    buf = malloc(size);
    pb_ostream_t stream = pb_ostream_from_buffer(buf, size);
    if (!pb_encode(&stream, criterion_protocol_ack_fields, &ack)) {
        criterion_perror("Could not encode ack: %s.\n", PB_GET_ERROR(&stream));
        abort();
    }

    int written;

    do {
        written = nn_send(sock, buf, size, 0);
    } while (written < 0 && errno == EINTR);

    if (written <= 0 || written != (int) size) {
        criterion_perror("Could not send ack: %s.\n", nn_strerror(errno));
        abort();
    }

    free(buf);
}

void free_message(criterion_protocol_msg *msg)
{
    pb_release(criterion_protocol_msg_fields, msg);
}
