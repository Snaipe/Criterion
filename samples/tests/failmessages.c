#include <criterion/criterion.h>

/* Define NULL consistenly for the regression tests */
#undef NULL
#define NULL    ((void *) 0)

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
    cr_expect(eq(i8, 0, 1));
    cr_expect(eq(i16, 0, 1));
    cr_expect(eq(i32, 0, 1));
    cr_expect(eq(i64, 0, 1));
    cr_expect(eq(u8, 0, 1));
    cr_expect(eq(u16, 0, 1));
    cr_expect(eq(u32, 0, 1));
    cr_expect(eq(u64, 0, 1));
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

    cr_expect(eq(str, "", "foo"));
    cr_expect(eq(str,
            "reallyreallylongstringindeedmygoodsirormadam",
            "yetanotherreallyreallylongstring"));
    cr_expect(eq(str, "foo\nbar", "foo\nbaz"));

    int a = 0;
    int b = 1;

    struct cr_mem ma = { &a, sizeof (a) };
    struct cr_mem mb = { &b, sizeof (b) };

    cr_expect(eq(mem, ma, mb));
    cr_expect(eq(int[1], &a, &b));

    int arra[16];
    int arrb[16];

    for (size_t i = 0; i < 16; ++i) {
        arra[i] = i;
        arrb[15 - i] = i;
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

Test(messages, default) {
    cr_expect(0);
    cr_expect(0, "foo %s", "bar");
}
