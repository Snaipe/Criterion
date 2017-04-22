/*
 * The MIT License (MIT)
 *
 * Copyright © 2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_INTERNAL_MEMORY_H_
#define CRITERION_INTERNAL_MEMORY_H_

CR_BEGIN_C_API

CR_API char *cri_string_xxd(const void *data, size_t size);

CR_END_C_API

struct cr_mem {
    const void *data;
    size_t size;

#ifdef __cplusplus
    template <typename T, size_t N>
    constexpr cr_mem(const T (&arr)[N])
        : data(static_cast<const void *>(&arr)),
        size(N)
    {}

    template <typename T>
    constexpr cr_mem(const T *arr, size_t n)
        : data(static_cast<const void *>(arr)),
        size(n)
    {}
#endif
};

#define CRI_ASSERT_TEST_TAG_mem       ,
#define CRI_ASSERT_TEST_TAGC_mem()    ,
#define CRI_ASSERT_TYPE_TAG_mem       struct cr_mem,
#define CRI_ASSERT_TYPE_TAG_ID_mem    mem,

#ifdef __cplusplus
bool operator==(const struct cr_mem &m1, const struct cr_mem &m2)
{
    return m1.size == m2.size ? !memcmp(m1.data, m2.data, m1.size) : 0;
}

bool operator<(const struct cr_mem &m1, const struct cr_mem &m2)
{
    return m1.size == m2.size ? memcmp(m1.data, m2.data, m1.size) < 0 : (m1.size > m2.size ? 1 : 0);
}

std::ostream &operator<<(std::ostream &s, const struct cr_mem &m)
{
    char *str = cri_string_xxd(m.data, m.size);

    s << std::string(str);
    free(str);
    return s;
}
#else
# include <string.h>

static inline int cr_user_mem_eq(struct cr_mem *m1, struct cr_mem *m2)
{
    return m1->size == m2->size ? !memcmp(m1->data, m2->data, m1->size) : 0;
}

static inline int cr_user_mem_cmp(struct cr_mem *m1, struct cr_mem *m2)
{
    return m1->size == m2->size ? memcmp(m1->data, m2->data, m1->size) : (m1->size > m2->size ? 1 : -1);
}

static inline char *cr_user_mem_tostr(struct cr_mem *m)
{
    return cri_string_xxd(m->data, m->size);
}
#endif

#endif /* !CRITERION_INTERNAL_MEMORY_H_ */
