#include <criterion/criterion.h>

Test(misc, failing) {
    cr_assert(0);
}

Test(misc, passing) {
    cr_assert(1);
}
