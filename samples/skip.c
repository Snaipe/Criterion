#include <criterion/criterion.h>

Test(misc, skipping) {
    cr_skip_test();
    cr_assert_fail("We want to skip before failing");
}

Test(misc, message) {
    cr_skip_test("Skips may take %s", "printf-like messages");
}
