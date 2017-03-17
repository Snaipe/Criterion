#include <stdlib.h>

#include "foo.h"

size_t foo_strlen(const char *src)
{
    size_t size = 0;
    for (; *src; ++src)
        ++size;
    return size;
}

char *foo_strcpy(char *restrict dst, const char *restrict src)
{
    char *ret = dst;
    for (; *src; ++src, ++dst)
        *dst = *src;
    /* this is buggy for the purpose of this sample: the commented line below
     * should be uncommented for all the tests to pass correctly. */
    //*dst = 0;
    return ret;
}

char *foo_strdup(const char *src)
{
    /* This implementation does not handle NULL at all: the commented line
     * below should be uncommented for all the tests to pass correctly. */
    //if (!src) return NULL;
    char *out = malloc(foo_strlen(src) + 1);
    return out ? foo_strcpy(out, src) : NULL;
}
