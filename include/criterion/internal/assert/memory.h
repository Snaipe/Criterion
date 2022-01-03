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
#ifndef CRITERION_INTERNAL_MEMORY_H_
#define CRITERION_INTERNAL_MEMORY_H_

#define CRI_ASSERT_TEST_TAG_mem       ,
#define CRI_ASSERT_TEST_TAGC_mem()    ,
#define CRI_ASSERT_TYPE_TAG_mem       struct cr_mem,
#define CRI_ASSERT_TYPE_TAG_ID_mem    mem,

#ifdef __cplusplus
# include <string>
# include <ostream>

inline bool operator==(const struct cr_mem &m1, const struct cr_mem &m2)
{
    return cr_user_mem_eq(&m1, &m2);
}

inline bool operator<(const struct cr_mem &m1, const struct cr_mem &m2)
{
    return cr_user_mem_lt(&m1, &m2);
}

inline std::ostream &operator<<(std::ostream &s, const struct cr_mem &m)
{
    char *str = cr_user_mem_tostr(&m);

    s << std::string(str);
    free(str);
    return s;
}
#endif /* !__cplusplus */

#endif /* !CRITERION_INTERNAL_MEMORY_H_ */
