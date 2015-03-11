#include <criterion/criterion.h>

Test(asserts, base) {
    assert(true);
    expect(true);

    assert(true, "Assertions may take failure messages");

    expect(false, "assert is fatal, expect isn't");
    assert(false, "This assert runs");
    assert(false, "This does not");
}

Test(asserts, string) {
    assert_strings_equal("hello", "hello");
    assert_strings_not_equal("hello", "olleh");

    assert_strings_gt("hello", "hell");
    assert_strings_geq("hello", "hell");
    assert_strings_geq("hello", "hello");

    assert_strings_lt("hell", "hello");
    assert_strings_leq("hell", "hello");
    assert_strings_leq("hello", "hello");
}

Test(asserts, native) {
    assert_equal(1, 1);
    assert_not_equal(1, 2);

    assert_lt(1, 2);
    assert_leq(1, 2);
    assert_leq(2, 2);

    assert_gt(2, 1);
    assert_geq(2, 1);
    assert_geq(2, 2);
}

Test(asserts, float) {
    assert_not_equal(0.1 * 0.1, 0.01);
    assert_float_equal(0.1 * 0.1, 0.01, 0.001);
}
