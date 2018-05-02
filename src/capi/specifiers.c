/*
 * The MIT License (MIT)
 *
 * Copyright © 2017-2018 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "criterion/internal/new_asserts.h"
#include "criterion/new/memory.h"
#include "compat/strtok.h"
#include "string/xxd.h"

/* *INDENT-OFF* */
#define CRI_ASSERT_DEFINE_STR_FN(Tag, Prefix, Fmt)                              \
    int CRI_USER_TAG_ID(lt, Tag)(                                               \
        CRI_ASSERT_TYPE_TAG(Tag) *actual,                                       \
        CRI_ASSERT_TYPE_TAG(Tag) *expected)                                     \
    {                                                                           \
        return Tag ## cmp(*actual, *expected) < 0;                              \
    }                                                                           \
    int CRI_USER_TAG_ID(eq, Tag)(                                               \
        CRI_ASSERT_TYPE_TAG(Tag) *actual,                                       \
        CRI_ASSERT_TYPE_TAG(Tag) *expected)                                     \
    {                                                                           \
        return !Tag ## cmp(*actual, *expected);                                 \
    }                                                                           \
    int CRI_USER_TAG_ID(zero, Tag)(CRI_ASSERT_TYPE_TAG(Tag) *val)               \
    {                                                                           \
        return !Tag ## len(*val);                                               \
    }                                                                           \
    char *CRI_USER_TAG_ID(tostr, Tag)(CRI_ASSERT_TYPE_TAG(Tag) *e)              \
    {                                                                           \
        CRI_ASSERT_TYPE_TAG(Tag) cri_dup = Tag ## dup(*e);                      \
        CRI_ASSERT_TYPE_TAG(Tag) cri_line = cri_ ## Tag ## tokc(cri_dup, '\n'); \
                                                                                \
        char *cri_str = NULL;                                                   \
        size_t cri_off = 0;                                                     \
        size_t cri_sz = 0;                                                      \
                                                                                \
        if (cri_line) {                                                         \
            cri_fmt_bprintf(&cri_str, &cri_off, &cri_sz,                        \
                    #Prefix "\"%" Fmt, cri_line);                               \
                                                                                \
            while ((cri_line = cri_ ## Tag ## tokc(NULL, '\n'))) {              \
                cri_fmt_bprintf(&cri_str, &cri_off, &cri_sz,                    \
                        "\\n\"\n" #Prefix "\"%" Fmt, cri_line);                 \
            }                                                                   \
                                                                                \
            cri_fmt_bprintf(&cri_str, &cri_off, &cri_sz, "\"");                 \
        }                                                                       \
        free(cri_dup);                                                          \
        return cri_str;                                                         \
    }

CRI_ASSERT_DEFINE_STR_FN(str, , "s")
CRI_ASSERT_DEFINE_STR_FN(wcs, L, "ls")
/* *INDENT-ON* */

int cr_user_mem_eq(const struct cr_mem *m1, const struct cr_mem *m2)
{
    if (m1->size != m2->size)
        return 0;
    return !memcmp(m1->data, m2->data, m1->size);
}

int cr_user_mem_lt(const struct cr_mem *m1, const struct cr_mem *m2)
{
    if (m1->size != m2->size)
        return m1->size < m2->size;
    return memcmp(m1->data, m2->data, m1->size) < 0;
}

char *cr_user_mem_tostr(const struct cr_mem *m)
{
    return cri_string_xxd(m->data, 0, m->size);
}
