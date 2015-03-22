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
#ifndef CRITERION_ORDERED_SET_H_
# define CRITERION_ORDERED_SET_H_

# include <stddef.h>
# include "criterion.h"

struct criterion_ordered_set {
    struct criterion_ordered_set_node *first;
    size_t size;
    int (*const cmp)(void *, void *);
    void (*const dtor)(void *, void *);
};

struct criterion_ordered_set_node {
    struct criterion_ordered_set_node *next;
    char data[0];
};

struct criterion_suite_set {
    struct criterion_suite suite;
    struct criterion_ordered_set *tests;
};

struct criterion_test_set {
    struct criterion_ordered_set *suites;
    size_t tests;
};

struct criterion_ordered_set *new_ordered_set(int (*cmp)(void *, void *), void (*dtor)(void *, void *));
void *insert_ordered_set(struct criterion_ordered_set *l, void *ptr, size_t size);

# define FOREACH_SET(Elt, Set)                                                 \
    for (struct criterion_ordered_set_node *n = Set->first; n; n = n->next)    \
        for (int cond = 1; cond;)                                              \
            for (Elt = (void*) n->data; cond; cond = 0)

#endif /* !CRITERION_ORDERED_SET_H_ */
