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
#ifndef CRITERION_INTERNAL_ASSERT_OP_HXX_
#define CRITERION_INTERNAL_ASSERT_OP_HXX_

#include <cwchar>

/* *INDENT-OFF* */
namespace criterion { namespace internal { namespace operators {
/* *INDENT-ON* */

template <typename T>
bool zero(const T &t) { return t == T{}; }

template <typename T>
bool zero(const T *t) { return !t; }

template <> inline bool zero<>(const char *t) { return !*t; }
template <> inline bool zero<>(const wchar_t *t) { return !*t; }

/* relops without const */
template <typename T, typename U>
inline bool operator!= (T& t, U& u) { return !(t == u); }

template <typename T, typename U>
inline bool operator<= (T& t, U& u) { return t < u || t == u; }

template <typename T, typename U>
inline bool operator> (T& t, U& u) { return !(t <= u); }

template <typename T, typename U>
inline bool operator>= (T& t, U& u) { return !(t < u); }

/* *INDENT-OFF* */
}}}
/* *INDENT-ON* */

#endif /* !CRITERION_INTERNAL_ASSERT_OP_HXX_ */
