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
#ifndef PROTOCOL_H_
# define PROTOCOL_H_

# include <pb.h>
# include <pb_encode.h>
# include <pb_decode.h>
# include "criterion.pb.h"
# include "criterion/internal/preprocess.h"

enum protocol_version {
    PROTOCOL_V1 = 1,
};

bool pb_write_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
bool pb_read_string(pb_istream_t *stream, const pb_field_t *field, void **arg);

pb_ostream_t pb_ostream_from_fd(int fd);
pb_istream_t pb_istream_from_fd(int fd);

# define criterion_message(Kind, ...)                                       \
    (criterion_protocol_msg) {                                              \
        .version = 1,                                                       \
        .which_id = criterion_protocol_msg_pid_tag,                         \
        .id = {                                                             \
            .pid = get_process_id(),                                        \
        },                                                                  \
        .data = {                                                           \
            .which_value = criterion_protocol_submessage_ ## Kind ## _tag,  \
            .value = {                                                      \
                .Kind = { __VA_ARGS__ },                                    \
            }                                                               \
        }                                                                   \
    }

#endif /* !PROTOCOL_H_ */
