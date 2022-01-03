/*
 * The MIT License (MIT)
 *
 * Copyright © 2018 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_NEW_STREAM_H_
#define CRITERION_NEW_STREAM_H_

#include <stddef.h>

#include "../internal/common.h"

#define CR_STREAM_BUFSIZE 4096

struct cri_stream_data {
    int dirty;
    int cmp;
    size_t ref;
    size_t cursor;
    size_t remaining;
    char buffer[CR_STREAM_BUFSIZE];
};

struct cr_stream {
    void *cookie;
    int (*read)(void *cookie, void *buf, size_t *size);
    void (*close)(void *cookie);

#ifdef __cplusplus
    template <typename T>
    cr_stream(T *cookie,
            int (*read)(void *cookie, void *buf, size_t *size),
            void (*close)(void *cookie) = nullptr);

    inline cr_stream(const cr_stream &other)
    {
        other.cri_data->ref++;
        cookie = other.cookie;
        read = other.read;
        close = other.close;
        cri_data = other.cri_data;
    }

    inline ~cr_stream();
#endif

    /* Internals. Do not poke at. */
#ifdef __cplusplus
private:
#endif
    struct cri_stream_data *cri_data;
};

CR_BEGIN_C_API

CR_API void cr_stream_init(struct cr_stream *s);
CR_API void cr_stream_close(struct cr_stream *s);

CR_API int cr_user_stream_eq(struct cr_stream *s1, struct cr_stream *s2);
CR_API int cr_user_stream_lt(struct cr_stream *s1, struct cr_stream *s2);
CR_API char *cr_user_stream_tostr(const struct cr_stream *s);

CR_END_C_API

#ifdef __cplusplus
namespace criterion {
    using stream = cr_stream;
}

inline bool operator==(criterion::stream &s1, criterion::stream &s2);
inline bool operator<(criterion::stream &s1, criterion::stream &s2);
inline std::ostream &operator<<(std::ostream &os, const criterion::stream &s);

template <typename T>
cr_stream::cr_stream(T *cookie,
        int (*read)(void *cookie, void *buf, size_t *size),
        void (*close)(void *cookie))
    : cookie(static_cast<void *>(cookie))
    , read(read)
    , close(close)
    , cri_data(nullptr)
{
    cr_stream_init(this);
}

inline cr_stream::~cr_stream() { cr_stream_close(this); }
#endif /* !__cplusplus */

#include "../internal/assert/stream.h"

#endif /* !CRITERION_NEW_STREAM_H_ */
