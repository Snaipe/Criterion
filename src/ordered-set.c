#include <criterion/common.h>
#include <criterion/ordered-set.h>
#include <csptr/smart_ptr.h>

static void destroy_ordered_set(void *ptr, UNUSED void *meta) {
    sfree(((struct criterion_ordered_set *) ptr)->first);
}

__attribute__ ((always_inline))
static inline void nothing() {}

static void destroy_ordered_set_node(void *ptr, void *meta) {
    struct criterion_ordered_set *set = *(void **) meta;
    struct criterion_ordered_set_node *n = ptr;
    (set->dtor ?: nothing)(n->data, NULL);
    sfree(((struct criterion_ordered_set_node *) ptr)->next);
}

struct criterion_ordered_set *new_ordered_set(int (*cmp)(void *, void *), f_destructor dtor) {
    return unique_ptr(struct criterion_ordered_set,
            { .cmp = cmp, .dtor = dtor }, destroy_ordered_set);
}

void *insert_ordered_set(struct criterion_ordered_set *l, void *ptr, size_t size) {
    int cmp;
    struct criterion_ordered_set_node *n, *prev = NULL;
    for (n = l->first; n && (cmp = l->cmp(ptr, n->data)) > 0; n = n->next)
        prev = n;

    if (n && !cmp) // element already exists
        return n->data;

    struct criterion_ordered_set_node *new = smalloc(
            .size = sizeof(struct criterion_ordered_set_node) + size,
            .dtor = destroy_ordered_set_node
        );
    if (!new)
        return NULL;

    memcpy(new->data, ptr, size);
    new->next = n;
    if (prev) {
        prev->next = new;
    } else {
        l->first = new;
    }

    ++l->size;
    return new->data;
}
