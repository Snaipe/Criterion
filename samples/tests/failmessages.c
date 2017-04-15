#include <criterion/criterion.h>

struct dummy_struct {
    char a;
    size_t b;
};

/* We need to provide basic functions for our dummy struct */
int cr_user_eq_dummy_struct(struct dummy_struct *a, struct dummy_struct *b)
{
    return a->a == b->a && a->b == b->b;
}

char *cr_user_tostr_dummy_struct(struct dummy_struct *d)
{
    char *out;

    cr_asprintf(&out, "(struct dummy_struct) {\n\t.a = %u,\n\t.b = %llu\n}",
            d->a, (unsigned long long) d->b);
    return out;
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
#if defined (CRI_CAPS_LDBL)
    cr_expect(eq(ldbl, 0, 1 / 3.l));
#endif

#if defined (CRI_CAPS_COMPLEX)
    cr_expect(eq(cx_flt, 0, 1));
    cr_expect(eq(cx_dbl, 0, 1));
# if defined (CRI_CAPS_LDBL)
    cr_expect(eq(cx_ldbl, 0, 1));
# endif
#endif

    /* Strings & pointers */
    cr_expect(eq(ptr, (void *) 1, NULL));

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

    struct cr_mem ma = { &a, sizeof (a) };
    struct cr_mem mb = { &b, sizeof (b) };

    cr_expect(eq(mem, ma, mb));
    cr_expect(eq(int[1], &a, &b));

    int arra[5];
    int arrb[5];

    for (size_t i = 0; i < 5; ++i) {
        arra[i] = i;
        arrb[4 - i] = i;
    }

    struct cr_mem marra = { &arra, sizeof (arra) };
    struct cr_mem marrb = { &arrb, sizeof (arrb) };

    cr_expect(eq(mem, marra, marrb));
    cr_expect(eq(int[sizeof (arra) / sizeof (int)], arra, arrb));

    struct dummy_struct dummy1 = { .a = 42, .b = 24 };
    struct dummy_struct dummy2 = { .a = 42, .b = 42 };

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
#if defined (CRI_CAPS_LDBL)
    cr_expect(cmptest(ldbl, 0, 1 / 3.l));
#endif

    /* Strings & pointers */
    cr_expect(cmptest(ptr, NULL, (void *) 1));

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

Test(messages, default) {
    cr_expect(0);
    cr_expect(0, "foo %s", "bar");
}
