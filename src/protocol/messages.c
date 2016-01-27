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
#include <stdlib.h>
#include <zmq.h>
#include "protocol/protocol.h"
#include "criterion/logging.h"
#include "io/event.h"
#include "io/asprintf.h"

int read_message(cr_socket sock, criterion_protocol_msg *message) {
    int res;
    zmq_msg_t msg;
    zmq_msg_init (&msg);

    int read = res = zmq_msg_recv(&msg, sock, 0);

    if (read <= 0)
        goto cleanup;

    pb_istream_t stream = pb_istream_from_buffer(zmq_msg_data(&msg), read);
    if (!pb_decode(&stream, criterion_protocol_msg_fields, message)) {
        res = -2;
        goto cleanup;
    }

    res = 1;
cleanup:
    zmq_msg_close(&msg);
    return res;
}

int write_message(cr_socket sock, const criterion_protocol_msg *message) {
    int res = -1;
    size_t size;
    unsigned char *buf = NULL;
    if (!pb_get_encoded_size(&size, criterion_protocol_msg_fields, message))
        goto cleanup;

    buf = malloc(size);
    pb_ostream_t stream = pb_ostream_from_buffer(buf, size);
    if (!pb_encode(&stream, criterion_protocol_msg_fields, message))
        goto cleanup;

    int written = zmq_send(sock, buf, size, 0);
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

void cr_send_to_runner(const criterion_protocol_msg *message) {
    if (write_message(g_client_socket, message) != 1) {
        criterion_perror("Could not write the \"%s\" message down the event pipe: %s.\n",
                message_names[message->data.which_value],
                zmq_strerror(errno));
        abort();
    }

    zmq_msg_t msg;
    zmq_msg_init (&msg);

    int read = zmq_msg_recv(&msg, g_client_socket, 0);

    if (read <= 0) {
        criterion_perror("Could not read ack: %s.\n", zmq_strerror(errno));
        abort();
    }

    criterion_protocol_ack ack;
    pb_istream_t stream = pb_istream_from_buffer(zmq_msg_data(&msg), read);
    if (!pb_decode(&stream, criterion_protocol_ack_fields, &ack)) {
        criterion_perror("Could not decode ack: %s.\n", PB_GET_ERROR(&stream));
        abort();
    }

    if (ack.status_code != criterion_protocol_ack_status_OK) {
        criterion_perror("Runner returned an error: %s.\n", ack.message ? ack.message : "Unknown error");
        abort();
    }
    pb_release(criterion_protocol_ack_fields, &ack);

    zmq_msg_close(&msg);
}

void send_ack(cr_socket sock, bool ok, const char *msg, ...) {
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

    int written = zmq_send(sock, buf, size, 0);
    if (written <= 0 || written != (int) size) {
        criterion_perror("Could not send ack: %s.\n", zmq_strerror(errno));
        abort();
    }

    free(buf);
}

void free_message(criterion_protocol_msg *msg) {
    pb_release(criterion_protocol_msg_fields, msg);
}
