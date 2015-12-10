#include <pb_encode.h>
#include <pb_decode.h>
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t count) {
    int sock = (intptr_t) stream->state;
    int result = nn_send(sock, buf, count, 0);

    if (result < 0)
        return false;
    return (size_t) result == count;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t count) {
    int sock = (intptr_t)stream->state;
    int result;

    result = nn_recv(sock, buf, count, 0);

    if (result == 0)
        stream->bytes_left = 0; /* EOF */

    if (result < 0)
        return false;

    return (size_t) result == count;
}

pb_ostream_t pb_ostream_from_nn_socket(int sock) {
    pb_ostream_t stream = {&write_callback, (void*)(intptr_t) sock, SIZE_MAX, 0, ""};
    return stream;
}

pb_istream_t pb_istream_from_nn_socket(int sock) {
    pb_istream_t stream = {&read_callback, (void*)(intptr_t) sock, SIZE_MAX, ""};
    return stream;
}
