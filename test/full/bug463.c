#include <criterion/criterion.h>
#include <criterion/new/assert.h>

struct T1 {};

int cr_user_T1_eq(struct T1 *a, struct T1 *b) { return 0; }

char *cr_user_T1_tostr(struct T1 *d)
{
    char *out;
    cr_asprintf(&out, "hello\n");
    return out;
}

Test(bug463, nl) {
    cr_assert(eq(type(struct T1), (struct T1){}, (struct T1){}));
}

struct T2 {};

int cr_user_T2_eq(struct T2 *a, struct T2 *b) { return 0; }

char *cr_user_T2_tostr(struct T2 *d)
{
    char *out;
    cr_asprintf(&out, "hello");
    return out;
}

Test(bug463, nonl) {
    cr_assert(eq(type(struct T2), (struct T2){}, (struct T2){}));
}
