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
#ifndef CRITERION_INTERNAL_COMPLEX_H_
#define CRITERION_INTERNAL_COMPLEX_H_

#if defined (__cplusplus)
# include <complex>

# define CRI_ASSERT_TEST_TAG_cx_flt         ,
# define CRI_ASSERT_TEST_TAGC_cx_flt()      ,
# define CRI_ASSERT_TYPE_TAG_cx_flt         std::complex<float>
# define CRI_ASSERT_TEST_TAG_ID_cx_flt      cx_flt,

# define CRI_ASSERT_TEST_TAG_cx_dbl         ,
# define CRI_ASSERT_TEST_TAGC_cx_dbl()      ,
# define CRI_ASSERT_TYPE_TAG_cx_dbl         std::complex<double>
# define CRI_ASSERT_TEST_TAG_ID_cx_dbl      cx_dbl,

# define CRI_ASSERT_TEST_TAG_cx_ldbl        ,
# define CRI_ASSERT_TEST_TAGC_cx_ldbl()     ,
# define CRI_ASSERT_TYPE_TAG_cx_ldbl        std::complex<long double>
# define CRI_ASSERT_TEST_TAG_ID_cx_ldbl     cx_ldbl,

#elif defined (__STDC__)
# include <complex.h>

# define CRI_ASSERT_TEST_TAG_cx_flt         ,
# define CRI_ASSERT_TEST_TAGC_cx_flt()      ,
# define CRI_ASSERT_TYPE_TAG_cx_flt         _Complex float
# define CRI_ASSERT_TEST_TAG_ID_cx_flt      cx_flt,

# define CRI_ASSERT_TEST_TAG_cx_dbl         ,
# define CRI_ASSERT_TEST_TAGC_cx_dbl()      ,
# define CRI_ASSERT_TYPE_TAG_cx_dbl         _Complex double
# define CRI_ASSERT_TEST_TAG_ID_cx_dbl      cx_dbl,

# define CRI_ASSERT_TEST_TAG_cx_ldbl        ,
# define CRI_ASSERT_TEST_TAGC_cx_ldbl()     ,
# if defined (CRI_CAPS_LDBL_IS_DBL)
#  define CRI_ASSERT_TYPE_TAG_cx_ldbl       _Complex double
# else
#  define CRI_ASSERT_TYPE_TAG_cx_ldbl       _Complex long double
# endif
# define CRI_ASSERT_TEST_TAG_ID_cx_ldbl     cx_ldbl,

# define CRI_ASSERT_DECLARE_COMPLEX_FN(Tag, Fmt, S)                             \
    static inline int CRI_USER_TAG_ID(eq, Tag)(                                 \
        CRI_ASSERT_TYPE_TAG(Tag) *actual,                                       \
        CRI_ASSERT_TYPE_TAG(Tag) *expected)                                     \
    {                                                                           \
        return *actual == *expected;                                            \
    }                                                                           \
    static inline char *CRI_USER_TAG_ID(tostr, Tag)(                            \
        CRI_ASSERT_TYPE_TAG(Tag) *e)                                            \
    {                                                                           \
        char *str = NULL;                                                       \
        cr_asprintf(&str, "%" Fmt " + i%" Fmt, creal ## S(*e), cimag ## S(*e)); \
        return str;                                                             \
    }

CRI_ASSERT_DECLARE_COMPLEX_FN(cx_flt, "." CRI_FLT_DIG "g", f)
CRI_ASSERT_DECLARE_COMPLEX_FN(cx_dbl, "." CRI_DBL_DIG "g", )

# if defined (CRI_CAPS_LDBL_IS_DBL)
CRI_ASSERT_DECLARE_COMPLEX_FN(cx_ldbl, "." CRI_DBL_DIG "g", )
# else
CRI_ASSERT_DECLARE_COMPLEX_FN(cx_ldbl, "." CRI_LDBL_DIG "Lg", l)
# endif

#endif

#endif /* !CRITERION_INTERNAL_COMPLEX_H_ */
