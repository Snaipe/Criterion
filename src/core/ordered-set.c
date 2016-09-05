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
#include <string.h>
#include <criterion/internal/common.h>
#include <criterion/internal/ordered-set.h>
#include <csptr/smalloc.h>
#include "common.h"

static void destroy_ordered_set(void *ptr, CR_UNUSED void *meta)
{
    sfree(((struct criterion_ordered_set *) ptr)->first);
}

static CR_INLINE void nothing(CR_UNUSED void *ptr, CR_UNUSED void *meta) {}

static void destroy_ordered_set_node(void *ptr, void *meta)
{
    struct criterion_ordered_set *set = *(void **) meta;
    struct criterion_ordered_set_node *n = ptr;

    DEF(set->dtor, nothing)(n + 1, NULL);
    sfree(((struct criterion_ordered_set_node *) ptr)->next);
}

struct criterion_ordered_set *new_ordered_set(f_criterion_cmp cmp,
        f_destructor dtor)
{
    struct criterion_ordered_set *newset = smalloc(
        .size = sizeof (struct criterion_ordered_set),
        .dtor = destroy_ordered_set
        );

    struct criterion_ordered_set data = { .cmp = cmp, .dtor = dtor };

    memcpy(newset, &data, sizeof (struct criterion_ordered_set));
    return newset;
}

void *insert_ordered_set(struct criterion_ordered_set *l,
        void *ptr,
        size_t size)
{
    int cmp;
    struct criterion_ordered_set_node *n, *prev = NULL;

    for (n = l->first; n && (cmp = l->cmp(ptr, n + 1)) > 0; n = n->next)
        prev = n;

    if (n && !cmp) /* element already exists */
        return n + 1;

    struct criterion_ordered_set_node *new = smalloc(
        .size = sizeof (struct criterion_ordered_set_node) + size,
        .dtor = destroy_ordered_set_node,
        .meta = { &l, sizeof (void *) },
        );
    if (!new)
        return NULL;

    memcpy(new + 1, ptr, size);
    new->next = n;
    if (prev)
        prev->next = new;
    else
        l->first = new;

    ++l->size;
    return new + 1;
}
