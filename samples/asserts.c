#include <criterion/criterion.h>

Test(asserts, base) {
    cr_assert(true);
    cr_expect(true);

    cr_assert(true, "Assertions may take failure messages");

    cr_assert(true, "Or even %d format string %s", 1, "with parameters");

    cr_expect(false, "assert is fatal, expect isn't");
    cr_assert(false, "This assert runs");
    cr_assert(false, "This does not");
}

Test(asserts, old_school) {
    cr_expect_fail("You can fail an assertion with a message from anywhere");
    cr_assert_fail(); /* or without a message */
}

Test(asserts, string) {
    cr_assert_str_empty("");
    cr_assert_str_not_empty("foo");

    cr_assert_str_eq("hello", "hello");
    cr_assert_str_neq("hello", "olleh");

    cr_assert_str_gt("hello", "hell");
    cr_assert_str_geq("hello", "hell");
    cr_assert_str_geq("hello", "hello");

    cr_assert_str_lt("hell", "hello");
    cr_assert_str_leq("hell", "hello");
    cr_assert_str_leq("hello", "hello");
}

Test(asserts, wstring) {
    cr_assert_wcs_empty(L"");
    cr_assert_wcs_not_empty(L"foo");

    cr_assert_wcs_eq(L"hello", L"hello");
    cr_assert_wcs_neq(L"hello", L"olleh");

    cr_assert_wcs_gt(L"hello", L"hell");
    cr_assert_wcs_geq(L"hello", L"hell");
    cr_assert_wcs_geq(L"hello", L"hello");

    cr_assert_wcs_lt(L"hell", L"hello");
    cr_assert_wcs_leq(L"hell", L"hello");
    cr_assert_wcs_leq(L"hello", L"hello");
}

Test(asserts, native) {
    cr_assert_eq(1, 1);
    cr_assert_neq(1, 2);

    cr_assert_lt(1, 2);
    cr_assert_leq(1, 2);
    cr_assert_leq(2, 2);

    cr_assert_gt(2, 1);
    cr_assert_geq(2, 1);
    cr_assert_geq(2, 2);
}

Test(asserts, float) {
    cr_assert_neq(0.1 * 0.1, 0.01);
    cr_assert_float_eq(0.1 * 0.1, 0.01, 0.001);
}

struct dummy_struct {
    char a;
    size_t b;
};

int eq_dummy(struct dummy_struct *a, struct dummy_struct *b)
{
    return a->a != b->a || a->b != b->b;
}

Test(asserts, array) {
    int arr1[] = { 1, 2, 3, 4 };
    int arr2[] = { 4, 3, 2, 1 };

    cr_assert_arr_eq(arr1, arr1, 4 * sizeof (int));
    cr_assert_arr_neq(arr1, arr2, 4 * sizeof (int));

#ifdef __GNUC__
    struct dummy_struct s1[] = { { 4, 2 }, { 2, 4 } };
    struct dummy_struct s2[2];
    memset(s2, 0xFF, sizeof (s2));
    s2[0].a = 4;
    s2[0].b = 2;
    s2[1].a = 2;
    s2[1].b = 4;

    /* cr_assert_arr_eq(s1, s2, 2); not guaranteed to work on structs. */
    cr_assert_arr_eq_cmp(s1, s2, 2, eq_dummy);
#else
    cr_assert(1); /* This is here for the test suite */
#endif
}
