#include <csptr/smalloc.h>

#include "criterion/criterion.h"
#include "criterion/internal/ordered-set.h"

int compare_gt(void *a, void *b)
{
    int *ia = a, *ib = b;

    return *ia == *ib ? 0 : (*ia > *ib ? -1 : 1);
}

int compare_lt(void *a, void *b)
{
    int *ia = a, *ib = b;

    return *ia == *ib ? 0 : (*ia < *ib ? -1 : 1);
}

Test(ordered_set, contract_lt) {
    struct criterion_ordered_set *set = new_ordered_set(compare_lt, NULL);

    insert_ordered_set(set, &(int[1]) { 1 }, sizeof (int));
    insert_ordered_set(set, &(int[1]) { 8 }, sizeof (int));
    insert_ordered_set(set, &(int[1]) { 3 }, sizeof (int));

    int *prev = NULL;
    FOREACH_SET(int *e, set) {
        if (prev)
            cr_assert_lt(*prev, *e);
        prev = e;
    }

    sfree(set);
}

Test(ordered_set, contract_gt) {
    struct criterion_ordered_set *set = new_ordered_set(compare_gt, NULL);

    insert_ordered_set(set, &(int[1]) { 1 }, sizeof (int));
    insert_ordered_set(set, &(int[1]) { 8 }, sizeof (int));
    insert_ordered_set(set, &(int[1]) { 3 }, sizeof (int));

    int *prev = NULL;
    FOREACH_SET(int *e, set) {
        if (prev)
            cr_assert_gt(*prev, *e);
        prev = e;
    }

    sfree(set);
}
