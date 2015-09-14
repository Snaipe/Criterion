#include <criterion/criterion.h>
#include <new>

Test(messages, default) {
    cr_expect(0);
    cr_expect_eq(0, 1);
    cr_expect_neq(1, 1);
    cr_expect_lt(2, 1);
    cr_expect_leq(2, 1);
    cr_expect_gt(1, 2);
    cr_expect_geq(1, 2);
    cr_expect_null("");
    cr_expect_not_null(NULL);

    cr_expect_float_eq(1, 2, 0.1);
    cr_expect_float_neq(2, 2, 0.1);

    cr_expect_str_empty("foo");
    cr_expect_str_not_empty("");
    cr_expect_str_eq("abc", "abd");
    cr_expect_str_neq("abc", "abc");
    cr_expect_str_lt("abc", "aba");
    cr_expect_str_leq("abc", "aba");
    cr_expect_str_gt("abc", "abd");
    cr_expect_str_geq("abc", "abd");

    cr_expect_throw(throw std::exception(), std::bad_alloc);
    cr_expect_no_throw(throw std::exception(), std::exception);
    cr_expect_any_throw({});
    cr_expect_none_throw(throw std::exception());
}

Test(messages, user) {
    cr_expect(0, "foo %s", "bar");
    cr_expect_eq(0, 1, "foo %s", "bar");
    cr_expect_neq(1, 1, "foo %s", "bar");
    cr_expect_lt(2, 1, "foo %s", "bar");
    cr_expect_leq(2, 1, "foo %s", "bar");
    cr_expect_gt(1, 2, "foo %s", "bar");
    cr_expect_geq(1, 2, "foo %s", "bar");
    cr_expect_null("", "foo %s", "bar");
    cr_expect_not_null(NULL, "foo %s", "bar");

    cr_expect_float_eq(1, 2, 0.1, "foo %s", "bar");
    cr_expect_float_neq(2, 2, 0.1, "foo %s", "bar");

    cr_expect_str_empty("foo", "foo %s", "bar");
    cr_expect_str_not_empty("", "foo %s", "bar");
    cr_expect_str_eq("abc", "abd", "foo %s", "bar");
    cr_expect_str_neq("abc", "abc", "foo %s", "bar");
    cr_expect_str_lt("abc", "aba", "foo %s", "bar");
    cr_expect_str_leq("abc", "aba", "foo %s", "bar");
    cr_expect_str_gt("abc", "abd", "foo %s", "bar");
    cr_expect_str_geq("abc", "abd", "foo %s", "bar");

    cr_expect_throw(throw std::exception(), std::bad_alloc, "foo %s", "bar");
    cr_expect_no_throw(throw std::exception(), std::exception, "foo %s", "bar");
    cr_expect_any_throw({}, "foo %s", "bar");
    cr_expect_none_throw(throw std::exception(), "foo %s", "bar");
}
