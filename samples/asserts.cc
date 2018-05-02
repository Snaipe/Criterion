#include <criterion/criterion.h>
#include <criterion/new/assert.h>
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
    cr_fail("You can fail an assertion with a message from anywhere");
    cr_fatal(); /* or without a message */
}

Test(asserts, string) {
    cr_assert(zero(str, ""));
    cr_assert(not(zero(str, "foo")));

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
    cr_assert(not(zero(wcs, L"foo")));

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
    cr_assert(not(eq(dbl, 0.1 * 0.1, 0.01)));
    cr_assert(ieee_ulp_eq(dbl, 0.1 * 0.1, 0.01, 4));
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
    auto mem_arr1 = criterion::memory { arr1 };
    auto mem_arr2 = criterion::memory { arr2 };

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

struct array_cursor {
    size_t off;
    size_t size;
    const void *buf;
};

static int read_array(void *cookie, void *buffer, size_t *size)
{
    array_cursor *arr = static_cast<array_cursor*>(cookie);
    size_t rem = *size;
    if (rem > arr->size - arr->off) {
        rem = arr->size - arr->off;
    }

    std::memcpy(buffer, (char *) arr->buf + arr->off, rem);
    arr->off += rem;
    *size = rem;

    return 0;
}

Test(asserts, stream) {
    struct array_cursor arr1 = {
        0,
        sizeof ("hello world"),
        "hello world",
    };

    struct array_cursor arr2 = {
        0,
        sizeof ("dlrow olleh"),
        "dlrow olleh",
    };

    /* we can compare binary data with the general purpose stream API, by
       a read function, and optionally a close function. */
    criterion::stream s1 = { &arr1, read_array };
    criterion::stream s2 = { &arr2, read_array };

    /* Note that this consumes both streams. Criterion will do the right thing
       if both streams are used in complex criteria by providing consistent
       comparison results between s1 and s2, but you can't compare either
       of them to any other stream without re-creating a fresh stream. */
    cr_assert(ne(stream, s1, s2));
}

Test(asserts, exception) {
    cr_expect(throw (std::runtime_error, {}));
    cr_assert(throw (std::invalid_argument, throw std::invalid_argument("some message")));
    cr_assert(throw (std::bad_alloc, throw std::invalid_argument("some other message")));
}
