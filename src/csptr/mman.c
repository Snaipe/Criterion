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

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "mman.h"
#include "array.h"
#undef smalloc

s_allocator smalloc_allocator = {malloc, free};

#ifdef _MSC_VER
# include <windows.h>
# include <malloc.h>
#endif

#ifndef _MSC_VER
static CSPTR_INLINE size_t atomic_add(volatile size_t *count, const size_t limit, const size_t val) {
    size_t old_count, new_count;
    do {
      old_count = *count;
      if (old_count == limit)
          abort();
      new_count = old_count + val;
    } while (!__sync_bool_compare_and_swap(count, old_count, new_count));
    return new_count;
}
#endif

static CSPTR_INLINE size_t atomic_increment(volatile size_t *count) {
#ifdef _MSC_VER
    return InterlockedIncrement64(count);
#else
    return atomic_add(count, SIZE_MAX, 1);
#endif
}

static CSPTR_INLINE size_t atomic_decrement(volatile size_t *count) {
#ifdef _MSC_VER
    return InterlockedDecrement64(count);
#else
    return atomic_add(count, 0, -1);
#endif
}

CSPTR_INLINE void *get_smart_ptr_meta(void *ptr) {
    assert((size_t) ptr == align((size_t) ptr));

    s_meta *meta = get_meta(ptr);
    assert(meta->ptr == ptr);

    size_t head_size = meta->kind & SHARED ? sizeof (s_meta_shared) : sizeof (s_meta);
    size_t *metasize = (size_t *) ptr - 1;
    if (*metasize == head_size)
        return NULL;

    return (char *) meta + head_size;
}

void *sref(void *ptr) {
    s_meta *meta = get_meta(ptr);
    assert(meta->ptr == ptr);
    assert(meta->kind & SHARED);
    atomic_increment(&((s_meta_shared *) meta)->ref_count);
    return ptr;
}

void *smove_size(void *ptr, size_t size) {
    s_meta *meta = get_meta(ptr);
    assert(meta->kind & UNIQUE);

    s_smalloc_args args;

    size_t *metasize = (size_t *) ptr - 1;
    if (meta->kind & ARRAY) {
        s_meta_array *arr_meta = get_smart_ptr_meta(ptr);
        args = (s_smalloc_args) {
            .size = arr_meta->size * arr_meta->nmemb,
            .kind = (enum pointer_kind) (SHARED | ARRAY),
            .dtor = meta->dtor,
            .meta = { arr_meta, *metasize },
        };
    } else {
        void *user_meta = get_smart_ptr_meta(ptr);
        args = (s_smalloc_args) {
            .size = size,
            .kind = SHARED,
            .dtor = meta->dtor,
            .meta = { user_meta, *metasize },
        };
    }

    void *newptr = smalloc(&args);
    memcpy(newptr, ptr, size);
    return newptr;
}

CSPTR_MALLOC_API
CSPTR_INLINE static void *alloc_entry(size_t head, size_t size, size_t metasize) {
    const size_t totalsize = head + size + metasize + sizeof (size_t);
#ifdef SMALLOC_FIXED_ALLOCATOR
    return malloc(totalsize);
#else /* !SMALLOC_FIXED_ALLOCATOR */
    return smalloc_allocator.alloc(totalsize);
#endif /* !SMALLOC_FIXED_ALLOCATOR */
}

CSPTR_INLINE static void dealloc_entry(s_meta *meta, void *ptr) {
    if (meta->dtor) {
        void *user_meta = get_smart_ptr_meta(ptr);
        if (meta->kind & ARRAY) {
            s_meta_array *arr_meta = (void *) (meta + 1);
            for (size_t i = 0; i < arr_meta->nmemb; ++i)
                meta->dtor((char *) ptr + arr_meta->size * i, user_meta);
        } else
            meta->dtor(ptr, user_meta);
    }

#ifdef SMALLOC_FIXED_ALLOCATOR
    free(meta);
#else /* !SMALLOC_FIXED_ALLOCATOR */
    smalloc_allocator.dealloc(meta);
#endif /* !SMALLOC_FIXED_ALLOCATOR */
}

CSPTR_MALLOC_API
static void *smalloc_impl(s_smalloc_args *args) {
    if (!args->size)
        return NULL;

    // align the sizes to the size of a word
    size_t aligned_metasize = align(args->meta.size);
    size_t size = align(args->size);

    size_t head_size = args->kind & SHARED ? sizeof (s_meta_shared) : sizeof (s_meta);
    s_meta_shared *ptr = alloc_entry(head_size, size, aligned_metasize);
    if (ptr == NULL)
        return NULL;

    char *shifted = (char *) ptr + head_size;
    if (args->meta.size && args->meta.data)
        memcpy(shifted, args->meta.data, args->meta.size);

    size_t *sz = (size_t *) (shifted + aligned_metasize);
    *sz = head_size + aligned_metasize;

    *(s_meta*) ptr = (s_meta) {
        .kind = args->kind,
        .dtor = args->dtor,
#ifndef NDEBUG
        .ptr = sz + 1
#endif
    };

    if (args->kind & SHARED)
        ptr->ref_count = 1;

    return sz + 1;
}

CSPTR_MALLOC_API
CSPTR_INLINE static void *smalloc_array(s_smalloc_args *args) {
    const size_t size = align(args->meta.size + sizeof(s_meta_array));
#ifdef _MSC_VER
    char *new_meta = _alloca(size);
#else
    char new_meta[size];
#endif
    s_meta_array *arr_meta = (void *) new_meta;
    *arr_meta = (s_meta_array) {
        .size = args->size,
        .nmemb = args->nmemb,
    };
    memcpy(arr_meta + 1, args->meta.data, args->meta.size);
    return smalloc_impl(&(s_smalloc_args) {
            .size = args->nmemb * args->size,
            .kind = (enum pointer_kind) (args->kind | ARRAY),
            .dtor = args->dtor,
            .meta = { &new_meta, size },
        });
}

CSPTR_MALLOC_API
void *smalloc(s_smalloc_args *args) {
    return (args->nmemb == 0 ? smalloc_impl : smalloc_array)(args);
}

void sfree(void *ptr) {
    if (!ptr) return;

    assert((size_t) ptr == align((size_t) ptr));
    s_meta *meta = get_meta(ptr);
    assert(meta->ptr == ptr);

    if (meta->kind & SHARED && atomic_decrement(&((s_meta_shared *) meta)->ref_count))
        return;

    dealloc_entry(meta, ptr);
}
