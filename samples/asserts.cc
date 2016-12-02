#include <criterion/criterion.h>
#include <exception>
#include <new>
#include <array>

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
    /* cr_assert(zero(str, ""));
       cr_assert(not(zero(str, "foo"))); */

    cr_assert(eq(str, "hello", "hello"));
    cr_assert(ne(str, "hello", "olleh"));

    cr_assert(gt(str, "hello", "hell"));
    cr_assert(ge(str, "hello", "hell"));
    cr_assert(ge(str, "hello", "hello"));

    cr_assert(lt(str, "hell", "hello"));
    cr_assert(le(str, "hell", "hello"));
    cr_assert(le(str, "hello", "hello"));
}

Test(asserts, wstring) {
    cr_assert(zero(wcs, L""));
    cr_assert(not (zero(wcs, L"foo")));

    cr_assert(eq(wcs, L"hello", L"hello"));
    cr_assert(ne(wcs, L"hello", L"olleh"));

    cr_assert(gt(wcs, L"hello", L"hell"));
    cr_assert(ge(wcs, L"hello", L"hell"));
    cr_assert(ge(wcs, L"hello", L"hello"));

    cr_assert(lt(wcs, L"hell", L"hello"));
    cr_assert(le(wcs, L"hell", L"hello"));
    cr_assert(le(wcs, L"hello", L"hello"));
}

Test(asserts, native) {
    cr_assert(eq(1, 1));
    cr_assert(ne(1, 2));

    cr_assert(lt(1, 2));
    cr_assert(le(1, 2));
    cr_assert(le(2, 2));

    cr_assert(gt(2, 1));
    cr_assert(ge(2, 1));
    cr_assert(ge(2, 2));
}

Test(asserts, float) {
    cr_assert_neq(0.1 * 0.1, 0.01);
    cr_assert_float_eq(0.1 * 0.1, 0.01, 0.001);
}

struct dummy_struct {
    char a;
    size_t b;
};

/* We need to provide basic functions for our dummy struct */
bool operator==(const struct dummy_struct &lhs, const struct dummy_struct &rhs)
{
    return lhs.a == rhs.a && lhs.b == rhs.b;
}

std::ostream &operator<<(std::ostream &s, const struct dummy_struct &val)
{
    s << "(struct dummy_struct) { .a = " << val.a << ", .b = " << val.b << "}";
    return s;
}

Test(asserts, array) {
    int arr1[] = { 1, 2, 3, 4 };
    int arr2[] = { 4, 3, 2, 1 };

    /* For primitive types we can compare their byte-to-byte representation */
    struct cr_mem mem_arr1 { arr1 };
    struct cr_mem mem_arr2 { arr2 };

    cr_assert(eq(mem, mem_arr1, mem_arr1));
    cr_assert(ne(mem, mem_arr1, mem_arr2));

    /* Or we can use the tag[] notation */
    cr_assert(eq(int[4], arr1, arr1));
    cr_assert(ne(int[4], arr1, arr2));

    /* The tag[] notation is mandatory to correctly compare padded types */
    struct dummy_struct s1[] = { { 4, 2 }, { 2, 4 } };
    struct dummy_struct s2[2];
    memset(s2, 0xFF, sizeof (s2));
    s2[0].a = 4;
    s2[0].b = 2;
    s2[1].a = 2;
    s2[1].b = 4;

    /* Here cr_assert(eq(mem, mem_s1, mem_s2)) would not have worked */
    cr_assert(eq(type(struct dummy_struct)[2], s1, s2));
}

/* Test(asserts, exception) {
      cr_assert_throw(throw std::exception(), std::exception);
      cr_assert_throw(throw std::exception(), std::bad_alloc);
   } */
