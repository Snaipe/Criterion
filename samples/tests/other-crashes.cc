#include <criterion/criterion.h>

void crash(void)
{
    int *i = NULL;

    *i = 42;
}

Test(misc, setup_crash, .init = crash) {
    cr_assert(true);
}

Test(misc, teardown_crash, .fini = crash) {
    cr_assert(true);
}
