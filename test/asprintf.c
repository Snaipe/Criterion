#include "criterion/criterion.h"
#include "criterion/theories.h"
#include "criterion/internal/asprintf-compat.h"
#include "criterion/new/assert.h"

#include <stdio.h>

union anyval {
    char c;
    short hd;
    int d;
    long ld;
    long long lld;
    unsigned short hu;
    unsigned int u;
    unsigned long lu;
    unsigned long long llu;
    double f;
    const char *s;
    void *p;
};

struct format_test {
    const char *format;
    union anyval *val;
};

#define VALUE(Fmt, Val)    & (struct format_test) { .format = "%" #Fmt, .val = &(union anyval) { .Fmt = Val } }

TheoryDataPoints(asprintf, valid) = {
    DataPoints(struct format_test *,
            VALUE(c,                'a'),
            VALUE(hd,               42),
            VALUE(d,                42),
            VALUE(ld,               42),
            VALUE(lld,              42),
            VALUE(hu,               42),
            VALUE(u,                42),
            VALUE(lu,               42),
            VALUE(llu,              42),
            VALUE(f,                3.14),
            VALUE(s,                "foo"),
            VALUE(p,                NULL),
            ),
};

Theory((struct format_test *fmt), asprintf, valid) {
    char *actual;
    char expected[32];

    int rc_expected = snprintf(expected, sizeof (expected), fmt->format, *fmt->val);
    int rc_actual = cr_asprintf(&actual, fmt->format, *fmt->val);

    cr_expect(eq(int, rc_actual, rc_expected));
    cr_expect(eq(str, actual, expected));

    free(actual);
}
