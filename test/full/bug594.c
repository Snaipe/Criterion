#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(bug594, none) {
    cr_expect(none(eq(int, 1, 1), eq(int, 3, 3), eq(int, 3, 3)));
}

Test(bug594, not) {
    cr_expect(not(eq(int, 1, 1)));
}

Test(bug594, not_long) {
    char s[] = "a string well over the forty character display threshold";

    cr_expect(not(eq(str, s, s)));
}

Test(bug594, all) {
    cr_expect(all(eq(int, 1, 1), eq(int, 1, 2)));
}

Test(bug594, any) {
    cr_expect(any(eq(int, 1, 2), eq(int, 3, 4)));
}

Test(bug594, negated_combinator) {
    cr_expect(not(all(eq(int, 1, 1))));
}

Test(bug594, passing) {
    cr_expect(none(eq(int, 1, 2), eq(int, 3, 4)));
    cr_expect(not(eq(int, 1, 2)));
    cr_expect(all(eq(int, 1, 1), not(eq(int, 1, 2))));
    cr_expect(any(eq(int, 1, 2), not(not(eq(int, 3, 3)))));
}
