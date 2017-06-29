#include <criterion/criterion.h>

#include <criterion/new/assert.h>

struct dummy_struct {
    char a;
    size_t b;
};

/* We need to provide basic functions for our dummy struct */
int operator==(const dummy_struct &a, const dummy_struct &b)
{
    return a.a == b.a && a.b == b.b;
}

std::ostream &operator<<(std::ostream &s, const struct dummy_struct &val)
{
    s << "(struct dummy_struct) {\n\t.a = " << val.a << ",\n\t.b = " << val.b << "\n}";
    return s;
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
    cr_expect(eq(str,
            "reallyreallylongstringindeedmygoodsirormadam",
            "yetanotherreallyreallylongstring"));
    cr_expect(eq(str, "foo\nbar", "foo\nbaz"));

    cr_expect(eq(wcs, L"", L"foo"));
    cr_expect(eq(wcs,
            L"reallyreallylongstringindeedmygoodsirormadam",
            L"yetanotherreallyreallylongstring"));
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
