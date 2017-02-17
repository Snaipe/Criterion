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
#define _GNU_SOURCE    1
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "config.h"
#include "internal.h"
#include "criterion/redirect.h"

#if defined (HAVE_FUNOPEN) || defined (HAVE_FOPENCOOKIE)

# ifdef HAVE_FUNOPEN
typedef int cr_count;
typedef int cr_retcount;
typedef fpos_t cr_off;
# else
typedef size_t cr_count;
typedef ssize_t cr_retcount;
typedef off64_t cr_off;
# endif

struct memfile {
    size_t size;
    size_t region_size;
    size_t cur;
    size_t max_size;
    char *mem;
};

static inline size_t size_safe_add(size_t size, size_t cur, cr_off off)
{
    cur = cur < SIZE_MAX - off ? cur + off : SIZE_MAX;
    return cur < size ? cur : size;
}

static inline size_t off_safe_add(size_t size, size_t cur, cr_off off)
{
    if (off >= 0)
        cur = cur < SIZE_MAX - off ? cur + off : SIZE_MAX;
    else
        cur = cur > (size_t) -off ? cur + off : 0;
    return cur < size ? cur : size;
}

# define errno_return(Errno, Val) \
    do {                          \
        errno = (Errno);          \
        return (Val);             \
    } while (0)

static cr_retcount mock_file_read(void *cookie, char *buf, cr_count count)
{
    struct memfile *mf = cookie;

# ifdef HAVE_FUNOPEN
    if (count < 0)
        errno_return(EINVAL, (cr_retcount) - 1);
# endif
    if (mf->cur >= mf->size || count == 0)
        return 0;

    size_t end = size_safe_add(mf->size, mf->cur, count);
    count = end - mf->cur;
    memcpy(buf, mf->mem + mf->cur, count);
    mf->cur = end;
    return count;
}

static cr_retcount mock_file_write(void *cookie, const char *buf, cr_count count)
{
    struct memfile *mf = cookie;

# ifdef HAVE_FUNOPEN
    if (count < 0)
        errno_return(EINVAL, (cr_retcount) - 1);
# endif
    if (count == 0)
        return 0;

    if (mf->cur >= mf->max_size)
        errno_return(EIO, (cr_retcount) - 1);

    size_t end = size_safe_add(mf->max_size, mf->cur, count);
    if (mf->size < end)
        mf->size = end;
    count = end - mf->cur;

    if (mf->size > mf->region_size) {
        while (mf->size > mf->region_size)
            mf->region_size = mf->region_size * 3 / 2;
        char *newptr = realloc(mf->mem, mf->region_size);
        if (!newptr)
            errno_return(EIO, (cr_retcount) - 1);
        mf->mem = newptr;
    }
    memcpy(mf->mem + mf->cur, buf, count);
    mf->cur = end;
    return count;
}

# ifdef HAVE_FUNOPEN
static cr_off mock_file_seek(void *cookie, cr_off off, int whence)
{
    struct memfile *mf = cookie;

    switch (whence) {
        case SEEK_SET: return mf->cur = off;
        case SEEK_CUR: return mf->cur = off_safe_add(mf->size, mf->cur, off);
        case SEEK_END: return mf->cur = off_safe_add(mf->size, mf->size, off);
        default: break;
    }
    errno = EINVAL;
    return (off_t) -1;
}
# else
static int mock_file_seek(void *cookie, cr_off *off, int whence)
{
    struct memfile *mf = cookie;

    switch (whence) {
        case SEEK_SET: mf->cur = *off; break;
        case SEEK_CUR: *off = (mf->cur = off_safe_add(mf->size, mf->cur, *off)); break;
        case SEEK_END: *off = (mf->cur = off_safe_add(mf->size, mf->size, *off)); break;
        default: errno = EINVAL; return -1;
    }
    return 0;
}
# endif

static int mock_file_close(void *cookie)
{
    struct memfile *mf = cookie;

    free(mf->mem);
    free(cookie);
    return 0;
}
#endif /* defined(HAVE_FUNOPEN) || defined(HAVE_FOPENCOOKIE) */

FILE *cr_mock_file_size(size_t max_size)
{
#if defined (HAVE_FUNOPEN) || defined (HAVE_FOPENCOOKIE)
    struct memfile *cookie = malloc(sizeof (struct memfile));
    *cookie = (struct memfile) {
        .max_size = max_size,
        .region_size = 4096,
        .mem = malloc(4096),
    };

    FILE *f;
# if defined (HAVE_FOPENCOOKIE)
    f = fopencookie(cookie, "w+", (cookie_io_functions_t) {
        .read  = mock_file_read,
        .write = mock_file_write,
        .seek  = mock_file_seek,
        .close = mock_file_close,
    });
# else
    f = funopen(cookie,
                    mock_file_read,
                    mock_file_write,
                    mock_file_seek,
                    mock_file_close);
# endif
    return f;
#elif defined (HAVE_OPEN_MEMSTREAM)
    (void) max_size;
    char *mem;
    size_t sz;

    /* We're actually leaking memory here. Can't do much, really -- we don't
     * really know when the user decides to close the file, and iterating a
     * list of opened memory streams to free them at exit wouldn't be worth
     * the effort. */
    return open_memstream(&mem, &sz);
#else
    (void) max_size;

    /* fallback to tmpfile() */
    return tmpfile();
#endif
}
