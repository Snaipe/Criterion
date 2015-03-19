#include <criterion/common.h>
#include <criterion/sorted-list.h>
#include <csptr/smart_ptr.h>

static void destroy_sorted_list(void *ptr, UNUSED void *meta) {
    sfree(((struct criterion_sorted_list *) ptr)->first);
}

struct criterion_sorted_list *new_sorted_list(int (*cmp)(void *, void *)) {
    return unique_ptr(struct criterion_sorted_list,
            { .cmp = cmp }, destroy_sorted_list);
}

void insert_sorted_list(struct criterion_sorted_list *l, void *ptr, size_t size) {
    struct criterion_sorted_list_node *n, *prev = NULL;
    for (n = l->first; l->cmp(ptr, n->data) > 0; ++n)
        prev = n;

    struct criterion_sorted_list_node *new =
        smalloc(sizeof(struct criterion_sorted_list_node) + size, 0, UNIQUE);

    memcpy(new->data, ptr, size);
    if (prev) {
        new->next = prev->next;
        prev->next = new;
    } else {
        new->next = NULL;
        l->first = new;
    }
}
