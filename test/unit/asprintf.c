#include "criterion/criterion.h"
#include "criterion/internal/asprintf-compat.h"
#include "criterion/new/assert.h"

#include <stdio.h>

#define assert_cr_asprintf_format_is_valid(Fmt, Val)                       \
    do {                                                                   \
        char *actual;                                                      \
        char expected[32];                                                 \
        int rc_expected = snprintf(expected, sizeof (expected), Fmt, Val); \
        int rc_actual = cr_asprintf(&actual, Fmt, Val);                    \
        cr_expect(eq(int, rc_actual, rc_expected));                        \
        cr_expect(eq(str, actual, expected));                              \
        free(actual);                                                      \
    } while (0)

Test(asprintf, valid) {
    assert_cr_asprintf_format_is_valid("%c", 'a');
    assert_cr_asprintf_format_is_valid("%hd", 42);
    assert_cr_asprintf_format_is_valid("%d", 42);
    assert_cr_asprintf_format_is_valid("%ld", 42L);
    assert_cr_asprintf_format_is_valid("%lld", 42LL);
    assert_cr_asprintf_format_is_valid("%hu", 42U);
    assert_cr_asprintf_format_is_valid("%u", 42U);
    assert_cr_asprintf_format_is_valid("%lu", 42UL);
    assert_cr_asprintf_format_is_valid("%llu", 42ULL);
    assert_cr_asprintf_format_is_valid("%f", 3.14);
    assert_cr_asprintf_format_is_valid("%s", "foo");
    assert_cr_asprintf_format_is_valid("%p", NULL);
}
