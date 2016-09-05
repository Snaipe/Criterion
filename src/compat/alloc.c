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
#include <string.h>

#include "alloc.h"
#include "err.h"

static bxf_arena inheritable_arena;

void cri_alloc_init(void)
{
    int rc = bxf_arena_init(0, BXF_ARENA_RESIZE | BXF_ARENA_IDENTITY
                    | BXF_ARENA_KEEPMAPPED, &inheritable_arena);

    if (rc < 0)
        cr_panic("Could not initialize inheritable arena: %s", strerror(-rc));
}

void cri_alloc_term(void)
{
    bxf_arena_term(&inheritable_arena);
}

bxf_arena cri_alloc_getarena(void)
{
    return inheritable_arena;
}

void *cr_malloc(size_t size)
{
    bxf_ptr ptr = bxf_arena_alloc(&inheritable_arena, size);

    if (ptr < 0) {
        errno = -ptr;
        return NULL;
    }
    return bxf_arena_ptr(inheritable_arena, ptr);
}

void *cr_calloc(size_t nmemb, size_t size)
{
    void *ptr = cr_malloc(size * nmemb);

    if (ptr)
        memset(ptr, 0, size * nmemb);
    return ptr;
}

void *cr_realloc(void *ptr, size_t size)
{
    bxf_ptr p = (intptr_t) ptr - (intptr_t) inheritable_arena;
    bxf_ptr newptr = bxf_arena_realloc(&inheritable_arena, p, size);

    if (newptr < 0) {
        errno = -newptr;
        return NULL;
    }
    return bxf_arena_ptr(inheritable_arena, newptr);
}

void cr_free(void *ptr)
{
    bxf_ptr p = (intptr_t) ptr - (intptr_t) inheritable_arena;

    (void) bxf_arena_free(&inheritable_arena, p);
}
