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
#include "alloc.h"
#include "internal.h"
#include <stdlib.h>

#ifdef VANILLA_WIN32
struct block {
    void *ptr;
    size_t size;
    struct block *next;
};

struct block *g_blocks = NULL;

void init_inheritable_heap(void) {
}

int inherit_heap(HANDLE child_process) {
    for (struct block *b = g_blocks; b != NULL; b = b->next) {
        if (!VirtualAllocEx(child_process,
                b->ptr,
                b->size,
                MEM_COMMIT,
                PAGE_READWRITE))
            return -1;

        if (!WriteProcessMemory(child_process,
                b->ptr,
                b->ptr,
                b->size,
                NULL))
            return -1;
    }
    return 0;
}
#endif

void *cr_malloc(size_t size) {
#ifdef VANILLA_WIN32
    void *ptr = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
    if (ptr == NULL)
        return NULL;
    struct block *b = malloc(sizeof (struct block));
    b->next = g_blocks;
    b->ptr = ptr;
    b->size = size;
    g_blocks = b;
    return ptr;
#else
    return malloc(size);
#endif
}

void *cr_calloc(size_t nmemb, size_t size) {
#ifdef VANILLA_WIN32
    void *ptr = cr_malloc(nmemb * size);
    memset(ptr, 0, nmemb * size);
    return ptr;
#else
    return calloc(nmemb, size);
#endif
}

void *cr_realloc(void *ptr, size_t size) {
#ifdef VANILLA_WIN32
    if (size > 4096)
        return NULL;
    return ptr;
#else
    return realloc(ptr, size);
#endif
}

void cr_free(void *ptr) {
#ifdef VANILLA_WIN32
#else
    free(ptr);
#endif
}
