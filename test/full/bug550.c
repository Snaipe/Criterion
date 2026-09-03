#include <criterion/criterion.h>
#include <criterion/new/assert.h>

static int one(void)
{
    return 1;
}

static int *addr(void)
{
    static int x;
    return &x;
}

Test(bug550, leading_paren) {
    cr_expect((one()));
    cr_expect((!!one()));
    cr_expect((long) one() == 1);
    cr_expect(((long) one()) == 1);
    cr_expect(not((one() == 0)));
}

Test(bug550, truthy_value) {
    int flags = 4;

    cr_expect(flags & 4);
    cr_expect(2);
    cr_expect(addr());
    cr_expect(all(flags & 4, eq(int, 1, 1)));
    cr_expect(any(flags & 8, flags & 4));
    cr_expect(not(flags & 8));
}

Test(bug550, leading_paren_fails) {
    cr_expect((one() == 0));
}
