#include <criterion/criterion.h>

Test(asserts, base) {
    assert(true);
    expect(true);

    assert(true, "Assertions may take failure messages");

    assert(true, .msg = "You can use explicit named arguments");

    expect(false, "assert is fatal, expect isn't");
    assert(false, "This assert runs");
    assert(false, "This does not");
}

Test(asserts, old_school) {
    if (false)
        abort_test("You can abort the test with a message from anywhere");

    abort_test(NULL); // or without a message
}

Test(asserts, string) {
    assert_strings_eq("hello", "hello");
    assert_strings_neq("hello", "olleh");

    assert_strings_gt("hello", "hell");
    assert_strings_geq("hello", "hell");
    assert_strings_geq("hello", "hello");

    assert_strings_lt("hell", "hello");
    assert_strings_leq("hell", "hello");
    assert_strings_leq("hello", "hello");
}

Test(asserts, native) {
    assert_eq(1, 1);
    assert_neq(1, 2);

    assert_lt(1, 2);
    assert_leq(1, 2);
    assert_leq(2, 2);

    assert_gt(2, 1);
    assert_geq(2, 1);
    assert_geq(2, 2);
}

Test(asserts, float) {
    assert_neq(0.1 * 0.1, 0.01);
    assert_float_eq(0.1 * 0.1, 0.01, 0.001);
}

struct dummy_struct {
    char a;
    size_t b;
};

int eq_dummy(struct dummy_struct *a, struct dummy_struct *b) {
    return a->a != b->a || a->b != b->b;
}

Test(asserts, array) {
    int arr1[] = {1, 2, 3, 4};
    int arr2[] = {4, 3, 2, 1};

    struct dummy_struct s1[] = {{4, 2}, {2, 4}};
    struct dummy_struct s2[2];
    memset(s2, 0xFF, sizeof(s2));
    s2[0].a = 4;
    s2[0].b = 2;
    s2[1].a = 2;
    s2[1].b = 4;

    assert_arrays_eq(arr1, arr1, 4);
    assert_arrays_neq(arr1, arr2, 4);

    assert_arrays_neq(s1, s2, 2);
    assert_arrays_eq_cmp(s1, s2, 2, eq_dummy);
}
