#include <criterion/criterion.h>
#include <stdlib.h>

#include "foo.h"

Test(strlen, empty)
{
    cr_assert_eq(foo_strlen(""), 0);
}

Test(strlen, simple)
{
    cr_assert_eq(foo_strlen("foo"), 3);
}

Test(strlen, longer)
{
    cr_assert_eq(foo_strlen("foo\0bar"), 3);
}

Test(strcpy, empty)
{
    char dst[] = "a";

    foo_strcpy(dst, "");
    cr_assert_str_empty(dst);
}

Test(strcpy, return)
{
    char dst[4];
    char *ret = foo_strcpy(dst, "foo");

    cr_assert_eq(ret, dst);
}

Test(strcpy, larger_dest)
{
    char dst[] = "foo baz";

    foo_strcpy(dst, "bar");

    cr_assert_str_eq(dst, "bar");
    cr_assert_str_eq(&dst[4], "baz");
}

Test(strdup, null)
{
    char *out = foo_strdup(NULL);

    /* NULL should be propagated */
    cr_assert_null(out);
}

Test(strdup, duplicate)
{
    char *out = foo_strdup("foo");

    /* out should contain the source string */
    cr_assert_str_eq(out, "foo");
    free(out);
}

/* These two tests expect a crash on failure */

Test(strdup, dynamic)
{
    char *out = foo_strdup("foo");

    /* out should be returned by malloc */
    free(out);
}

Test(strdup, writeable)
{
    char *out = foo_strdup("foo");

    /* out should be writeable memory */
    foo_strcpy(out, "bar");

    free(out);
}
