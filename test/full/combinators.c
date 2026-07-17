#include <criterion/criterion.h>
#include <criterion/new/assert.h>

Test(combinators, all_inside_any) {
    cr_expect(any(eq(i32, 1, 0), all(eq(i32, 1, 1), eq(i32, 2, 2))));
    cr_expect(any(eq(i32, 1, 0), none(eq(i32, 1, 0))));
    cr_expect(all(any(eq(i32, 1, 0), all(eq(i32, 1, 1), eq(i32, 2, 2))), eq(i32, 3, 3)));
}

Test(combinators, not_all) {
    cr_expect(not(all(eq(i32, 1, 0), eq(i32, 1, 1))));
    cr_expect(not(none(eq(i32, 1, 1))));
}

Test(combinators, siblings) {
    cr_expect(all(any(eq(i32, 1, 1), eq(i32, 1, 0)), any(eq(i32, 2, 2))));
    cr_expect(any(all(eq(i32, 1, 0)), all(eq(i32, 2, 2))));
    cr_expect(none(any(eq(i32, 1, 0)), any(eq(i32, 0, 1))));
}

Test(combinators, not_all_fails) {
    cr_expect(not(all(eq(i32, 1, 1), eq(i32, 2, 2))));
}

Test(combinators, nested_any_fails) {
    cr_expect(any(eq(i32, 1, 0), all(eq(i32, 1, 0), eq(i32, 1, 1))));
}
