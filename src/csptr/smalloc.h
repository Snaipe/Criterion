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

#ifndef CSPTR_SMALLOC_H_
# define CSPTR_SMALLOC_H_

# include <stdlib.h>
# include "config.h"
# include "common.h"

# ifdef CSPTR_NO_SENTINEL
#  ifndef __GNUC__
#   error Variadic structure sentinels can only be disabled on a compiler supporting GNU extensions
#  endif
#  define CSPTR_SENTINEL
#  define CSPTR_SENTINEL_DEC
# else
#  define CSPTR_SENTINEL        .sentinel_ = 0,
#  define CSPTR_SENTINEL_DEC int sentinel_;
# endif

enum pointer_kind {
    UNIQUE,
    SHARED,

    ARRAY = 1 << 8
};

typedef void (*f_destructor)(void *, void *);

typedef struct {
    void *(*alloc)(size_t);
    void (*dealloc)(void *);
} s_allocator;

extern s_allocator smalloc_allocator;

typedef struct {
    CSPTR_SENTINEL_DEC
    size_t size;
    size_t nmemb;
    enum pointer_kind kind;
    f_destructor dtor;
    struct {
        const void *data;
        size_t size;
    } meta;
} s_smalloc_args;

CSPTR_PURE void *get_smart_ptr_meta(void *ptr);
void *sref(void *ptr);
CSPTR_MALLOC_API void *smalloc(s_smalloc_args *args);
void sfree(void *ptr);
void *smove_size(void *ptr, size_t size);

#  define smalloc(...) \
    smalloc(&(s_smalloc_args) { CSPTR_SENTINEL __VA_ARGS__ })

#  define smove(Ptr) \
    smove_size((Ptr), sizeof (*(Ptr)))

#endif /* !CSPTR_SMALLOC_H_ */
