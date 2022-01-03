/*
 * The MIT License (MIT)
 *
 * Copyright © 2018 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_INTERNAL_STREAM_H_
#define CRITERION_INTERNAL_STREAM_H_

#define CRI_ASSERT_TEST_TAG_stream       ,
#define CRI_ASSERT_TEST_TAGC_stream()    ,
#define CRI_ASSERT_TYPE_TAG_stream       struct cr_stream,
#define CRI_ASSERT_TYPE_TAG_ID_stream    stream,

#ifdef __cplusplus
# include <string>
# include <ostream>

inline bool operator==(criterion::stream &s1, criterion::stream &s2)
{
    return cr_user_stream_eq(&s1, &s2);
}

inline bool operator<(criterion::stream &s1, criterion::stream &s2)
{
    return cr_user_stream_lt(&s1, &s2);
}

inline std::ostream &operator<<(std::ostream &os, const criterion::stream &s)
{
    char *str = cr_user_stream_tostr(&s);

    os << std::string(str);
    free(str);
    return os;
}
#endif /* !__cplusplus */

#endif /* !CRITERION_INTERNAL_STREAM_H_ */
