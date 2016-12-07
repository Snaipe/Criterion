#include <criterion/criterion.h>
#include <stdio.h>

void setup(void)
{
    puts("Runs before the test");
}

void teardown(void)
{
    puts("Runs after the test");
}

Test(simple, fixtures, .init = setup, .fini = teardown) {
    cr_assert(1);
}
