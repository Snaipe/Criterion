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
