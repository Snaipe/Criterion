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
#include "alloc.h"
#include "internal.h"
#include "log/logging.h"
#include <stdlib.h>

#ifdef VANILLA_WIN32
HANDLE g_heap;

struct garbage_heap {
    HANDLE handle;
    struct garbage_heap *next;
};

# define HEAP_MIN_BASE 0x08000000

void init_inheritable_heap(void) {
    struct garbage_heap *heaps = NULL;

    while ((void*)(g_heap = HeapCreate(0, 0, 0)) < (void*)HEAP_MIN_BASE) {
        if (g_heap == NULL)
            break;

        struct garbage_heap *heap = malloc(sizeof(struct garbage_heap));
        heap->handle = g_heap;
        heap->next = heaps;
        heaps = heap;
    }
    for (struct garbage_heap *h = heaps; h != NULL; h = h->next)
        HeapDestroy(h->handle);

    if (g_heap == (HANDLE) NULL) {
        criterion_perror("Could not create the private inheritable heap.\n");
        abort();
    }
}

int inherit_heap(HANDLE child_process) {
    PROCESS_HEAP_ENTRY entry = { .lpData = NULL };

    while (HeapWalk(g_heap, &entry)) {
        if (!(entry.wFlags & PROCESS_HEAP_REGION))
            continue;

        DWORD region_size = entry.Region.dwCommittedSize;

        if (!VirtualAllocEx(child_process,
                entry.lpData,
                region_size,
                MEM_RESERVE | MEM_COMMIT,
                PAGE_READWRITE))
            return -1;

        if (!WriteProcessMemory(child_process,
                entry.lpData,
                entry.lpData,
                region_size,
                NULL))
            return -1;
    }
    return 0;
}
#endif

void *cr_malloc(size_t size) {
#ifdef VANILLA_WIN32
    return HeapAlloc(g_heap, 0, size);
#else
    return malloc(size);
#endif
}

void *cr_calloc(size_t nmemb, size_t size) {
#ifdef VANILLA_WIN32
    return HeapAlloc(g_heap, HEAP_ZERO_MEMORY, nmemb * size);
#else
    return calloc(nmemb, size);
#endif
}

void *cr_realloc(void *ptr, size_t size) {
#ifdef VANILLA_WIN32
    return HeapReAlloc(g_heap, 0, ptr, size);
#else
    return realloc(ptr, size);
#endif
}

void cr_free(void *ptr) {
#ifdef VANILLA_WIN32
    HeapFree(g_heap, 0, ptr);
#else
    free(ptr);
#endif
}
