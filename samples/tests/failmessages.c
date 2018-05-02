#include <criterion/criterion.h>

#include <criterion/new/assert.h>

struct dummy_struct {
    char a;
    size_t b;
};

/* We need to provide basic functions for our dummy struct */
int cr_user_dummy_struct_eq(struct dummy_struct *a, struct dummy_struct *b)
{
    return a->a == b->a && a->b == b->b;
}

char *cr_user_dummy_struct_tostr(struct dummy_struct *d)
{
    char *out;

    cr_asprintf(&out, "(struct dummy_struct) {\n\t.a = %u,\n\t.b = %llu\n}",
            d->a, (unsigned long long) d->b);
    return out;
}

struct array_cursor {
    size_t off;
    size_t size;
    const void *buf;
};

static int read_array(void *cookie, void *buffer, size_t *size)
{
    struct array_cursor *arr = cookie;
    size_t rem = *size;
    if (rem > arr->size - arr->off) {
        rem = arr->size - arr->off;
    }

    memcpy(buffer, (char *) arr->buf + arr->off, rem);
    arr->off += rem;
    *size = rem;

    return 0;
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

    struct cr_stream s1 = { .cookie = &arr1, .read = read_array };
    cr_stream_init(&s1);

    struct cr_stream s2 = { .cookie = &arr2, .read = read_array };
    cr_stream_init(&s2);

    cr_expect(eq(stream, s1, s2));
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

    struct cr_stream shi = { .cookie = &ahi, .read = read_array };
    cr_stream_init(&shi);

    struct cr_stream slo = { .cookie = &alo, .read = read_array };
    cr_stream_init(&slo);

    cr_expect(cmptest(stream, slo, shi));

    cr_stream_close(&shi);
    cr_stream_close(&slo);
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
