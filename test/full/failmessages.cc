#include <criterion/criterion.h>

#include <criterion/new/assert.h>

#include <map>
#include <vector>

struct dummy_struct {
    char a;
    size_t b;

    explicit dummy_struct(char a = '\0', size_t b = 0) : a(a), b(b) {}
};

/* We need to provide basic functions for our dummy struct */
int operator==(const dummy_struct &a, const dummy_struct &b)
{
    return a.a == b.a && a.b == b.b;
}

std::ostream &operator<<(std::ostream &s, const struct dummy_struct &val)
{
    s << "(struct dummy_struct) {\n\t.a = " << unsigned(val.a) << ",\n\t.b = " << val.b << "\n}";
    return s;
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

Test(messages, zero) {
    /* Primitive */
    cr_expect(not(zero(i8, 0)));
    cr_expect(not(zero(i16, 0)));
    cr_expect(not(zero(i32, 0)));
    cr_expect(not(zero(i64, 0)));
    cr_expect(not(zero(u8, 0)));
    cr_expect(not(zero(u16, 0)));
    cr_expect(not(zero(u32, 0)));
    cr_expect(not(zero(u64, 0)));
    cr_expect(not(zero(iptr, 0)));
    cr_expect(not(zero(uptr, 0)));
    cr_expect(not(zero(flt, 0)));
    cr_expect(not(zero(dbl, 0)));
    cr_expect(not(zero(ldbl, 0)));

    /* Strings & pointers */
    cr_expect(not(zero(ptr, 0)));
    cr_expect(not(zero(str, "")));
    cr_expect(not(zero(wcs, L"")));

    cr_expect(not(zero(type(dummy_struct), dummy_struct{})));

    /* STL containers */
    using int_vect = std::vector<int>;
    cr_expect(not(zero(type(int_vect), int_vect{})));

    using string_int_map = std::map<std::string, int>;
    cr_expect(not(zero(type(string_int_map), string_int_map{})));
}

Test(messages, eq) {
    /* Primitive */
    cr_expect(eq(i8, 0, 1));
    cr_expect(eq(i16, 0, 1));
    cr_expect(eq(i32, 0, 1));
    cr_expect(eq(i64, 0, 1));
    cr_expect(eq(u8, 0, 1));
    cr_expect(eq(u16, 0, 1));
    cr_expect(eq(u32, 0, 1));
    cr_expect(eq(u64, 0, 1));
    cr_expect(eq(iptr, 0, 1));
    cr_expect(eq(uptr, 0, 1));
    cr_expect(eq(flt, 0, 1 / 3.f));
    cr_expect(eq(dbl, 0, 1 / 3.));
    cr_expect(eq(ldbl, 0, 1 / 3.l));

    /* Strings & pointers */
    cr_expect(eq(ptr, (void *) 1, (void *) 0));

    cr_expect(eq(str, "", "foo"));
    cr_expect(eq(str, "reallyreallylongstringindeedmygoodsirormadam", "yetanotherreallyreallylongstring"));
    cr_expect(eq(str, "foo\nbar", "foo\nbaz"));

    cr_expect(eq(wcs, L"", L"foo"));
    cr_expect(eq(wcs, L"reallyreallylongstringindeedmygoodsirormadam", L"yetanotherreallyreallylongstring"));
    cr_expect(eq(wcs, L"foo\nbar", L"foo\nbaz"));

    int a = 0;
    int b = 1;

    auto ma = criterion::memory { &a, 1 };
    auto mb = criterion::memory { &b, 1 };

    cr_expect(eq(mem, ma, mb));
    cr_expect(eq(int[1], &a, &b));

    int arra[5];
    int arrb[5];

    for (size_t i = 0; i < 5; ++i) {
        arra[i] = i;
        arrb[4 - i] = i;
    }

    auto marra = criterion::memory { arra };
    auto marrb = criterion::memory { arrb };

    cr_expect(eq(mem, marra, marrb));
    cr_expect(eq(int[sizeof (arra) / sizeof (int)], arra, arrb));

    dummy_struct dummy1 { 42, 24 };
    dummy_struct dummy2 { 42, 42 };

    cr_expect(eq(type(struct dummy_struct), dummy1, dummy2));
    cr_expect(eq(type(struct dummy_struct)[1], &dummy1, &dummy2));

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

    criterion::stream s1 = { &arr1, read_array };
    criterion::stream s2 = { &arr2, read_array };

    cr_expect(eq(stream, s1, s2));

    /* STL containers */
    using int_vect = std::vector<int>;
    int_vect vec1 = {1, 2, 3};
    int_vect vec2 = {3, 2, 1};
    cr_expect(eq(type(int_vect), vec1, vec2));

    using string_int_map = std::map<std::string, int>;
    string_int_map m1 = {{"hello", 1}, {"world", 2}};
    string_int_map m2 = {{"hello", 2}, {"all", 1}};
    cr_expect(eq(type(string_int_map), m1, m2));
}

#define cmptest(Tag, Lo, Hi) \
    all(                     \
        lt(Tag, Hi, Lo),     \
        le(Tag, Hi, Lo),     \
        gt(Tag, Lo, Hi),     \
        ge(Tag, Lo, Hi)      \
        )

Test(messages, cmp) {
    /* Primitive */
    cr_expect(cmptest(i8, 0, 1));
    cr_expect(cmptest(i16, 0, 1));
    cr_expect(cmptest(i32, 0, 1));
    cr_expect(cmptest(i64, 0, 1));
    cr_expect(cmptest(u8, 0, 1));
    cr_expect(cmptest(u16, 0, 1));
    cr_expect(cmptest(u32, 0, 1));
    cr_expect(cmptest(u64, 0, 1));
    cr_expect(cmptest(iptr, 0, 1));
    cr_expect(cmptest(uptr, 0, 1));
    cr_expect(cmptest(flt, 0, 1 / 3.f));
    cr_expect(cmptest(dbl, 0, 1 / 3.));
    cr_expect(cmptest(ldbl, 0, 1 / 3.l));

    /* Strings & pointers */
    cr_expect(cmptest(ptr, (void *) 0, (void *) 1));

    cr_expect(cmptest(str, "abc", "cba"));
    cr_expect(cmptest(str, "abc\nabc", "cba\ncba"));

    cr_expect(cmptest(wcs, L"abc", L"cba"));
    cr_expect(cmptest(wcs, L"abc\nabc", L"cba\ncba"));

    struct array_cursor ahi = {
        0,
        sizeof ("hello world"),
        "hello world",
    };

    struct array_cursor alo = {
        0,
        sizeof ("dlrow olleh"),
        "dlrow olleh",
    };

    criterion::stream shi = { &ahi, read_array };
    criterion::stream slo = { &alo, read_array };

    cr_expect(cmptest(stream, slo, shi));
}

Test(message, compo) {
    cr_expect(all(eq(i32, 1, 1), eq(i32, 1, 1), eq(i32, 1, 0)));
    cr_expect(any(eq(i32, 1, 0), eq(i32, 1, 0), eq(i32, 1, 0)));
    cr_expect(none(eq(i32, 1, 0), eq(i32, 1, 0), eq(i32, 1, 1)));
}

Test(message, exception) {
    cr_expect(throw(std::bad_alloc, throw std::invalid_argument("exception message")));
    cr_expect(not(throw(std::invalid_argument, throw std::invalid_argument("exception message"))));
    cr_expect(nothrow(throw std::invalid_argument("exception message")));
    cr_expect(anythrow({}));
}

Test(messages, default) {
    cr_expect(0);
    cr_expect(0, "foo %s", "bar");
}

Test(messages, report_escape) {
    cr_fail();
    cr_fail("");
    cr_fail("\"dquote\" \\and\\ 'squote'\t\r\n<script>\x01.");
}
