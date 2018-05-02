/*
 * The MIT License (MIT)
 *
 * Copyright © 2017 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_NEW_MEMORY_H_
#define CRITERION_NEW_MEMORY_H_

#include "../internal/common.h"

struct cr_mem {
    const void *data;
    size_t size;

#ifdef __cplusplus
    template <typename T, size_t N>
    constexpr cr_mem(const T (&arr)[N])
        : data(static_cast<const void *>(&arr)), size(N)
    {}

    template <typename T>
    constexpr cr_mem(const T *arr, size_t n)
        : data(static_cast<const void *>(arr)), size(n)
    {}
#endif /* !__cplusplus */
};

#ifdef __cplusplus
namespace criterion {
    using memory = cr_mem;
}

inline bool operator==(const criterion::memory &m1, const criterion::memory &m2);
inline bool operator<(const criterion::memory &m1, const criterion::memory &m2);
inline std::ostream &operator<<(std::ostream &s, const criterion::memory &m);
#endif /* !__cplusplus */

CR_BEGIN_C_API

CR_API int cr_user_mem_eq(const struct cr_mem *m1, const struct cr_mem *m2);
CR_API int cr_user_mem_lt(const struct cr_mem *m1, const struct cr_mem *m2);
CR_API char *cr_user_mem_tostr(const struct cr_mem *m);

CR_END_C_API

#include "../internal/assert/memory.h"

#endif /* !CRITERION_NEW_MEMORY_H_ */
