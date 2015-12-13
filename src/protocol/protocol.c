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
