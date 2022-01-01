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
#ifndef CRITERION_INTERNAL_ASSERT_IEEE_H_
#define CRITERION_INTERNAL_ASSERT_IEEE_H_

/* IEEE specifiers */

#ifdef __cplusplus

# include <cmath>
# include <limits>

namespace criterion { namespace internal { namespace operators {

template <typename Float,
    typename = typename std::enable_if<std::numeric_limits<Float>::is_iec559>::type>
bool ieee_eq(Float &a, Float &b, size_t ulp)
{
    if (ulp == 0)
        return false;

    /* Not the most efficient but the most portable */
    a = std::nextafter(a, b);
    return a == b ? true : ieee_eq(a, b, ulp - 1);
}

}}} /* criterion::internal::operators */

# define CRI_IEEE_ULP_EQ(Lhs, Rhs, Ulp)                             \
    ::criterion::internal::operators::ieee_eq((Lhs), (Rhs), (Ulp))

# define CRI_IEEE_ULP_NE(Lhs, Rhs, Ulp) !CRI_IEEE_ULP_EQ(Lhs, Rhs, Ulp)

# define CRI_IEEE_ULP_T_EQ(Tag, Lhs, Rhs, Ulp)    CRI_IEEE_ULP_EQ(Lhs, Rhs, Ulp)
# define CRI_IEEE_ULP_T_NE(Tag, Lhs, Rhs, Ulp)    CRI_IEEE_ULP_NE(Lhs, Rhs, Ulp)
#else
# define CRI_IEEE_ULP_EQ(Lhs, Rhs, Ulp) \
    CR_COMPILE_ERROR(ieee_ulp_eq without a tag parameter is unsupported on this compiler.)
# define CRI_IEEE_ULP_NE(Lhs, Rhs, Ulp) \
    CR_COMPILE_ERROR(ieee_ulp_ne without a tag parameter is unsupported on this compiler.)

# define CRI_IEEE_ULP_T_EQ(Tag, Lhs, Rhs, Ulp) \
    (CRI_USER_TAG_ID(ieee_ulp_eq, Tag)((Lhs), (Rhs), (Ulp)))

# define CRI_IEEE_ULP_T_NE(Tag, Lhs, Rhs, Ulp) \
    !(CRI_USER_TAG_ID(ieee_ulp_eq, Tag)((Lhs), (Rhs), (Ulp)))

# include <math.h>

# define CRI_DEFINE_IEEE_ULP_EQ(Tag, Suffix)                                  \
    static inline int CRI_USER_TAG_ID(ieee_ulp_eq, Tag)(                      \
        CRI_ASSERT_TYPE_TAG(Tag) a,                                           \
        CRI_ASSERT_TYPE_TAG(Tag) b, size_t ulp)                               \
    {                                                                         \
        if (ulp == 0)                                                         \
            return a == b;                                                    \
        a = nextafter ## Suffix(a, b);                                        \
        return a == b ? 1 : CRI_USER_TAG_ID(ieee_ulp_eq, Tag)(a, b, ulp - 1); \
    }

CRI_DEFINE_IEEE_ULP_EQ(flt, f)
CRI_DEFINE_IEEE_ULP_EQ(dbl, )

# if defined (CRI_CAPS_LDBL_IS_DBL)
CRI_DEFINE_IEEE_ULP_EQ(ldbl, )
# else
CRI_DEFINE_IEEE_ULP_EQ(ldbl, l)
# endif
#endif

#define CRI_MKNODE_ULPS (CRI_MKNODE_ULP, CRI_MKNODE_ULP)
#define CRI_MKNODE_ULP(_, Var, Name) CRI_MKNODE_STR(sz, Var, Name)

#define CRI_ASSERT_TEST_SPECIFIER_ieee_ulp_eq(...)    ,
#define CRI_ASSERT_SPECIFIER_ieee_ulp_eq(...)                                               \
    CRI_ASSERT_SPECIFIER_OP(CRI_IEEE_ULP_T_EQ, CRI_IEEE_ULP_EQ, ieee_ulp_eq, (              \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_DEF),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_expected, "expected", CRI_MKNODE_DEF),    \
        (SINGLE, size_t, cri_ulp, "ulp", CRI_MKNODE_ULPS)                                   \
    ), __VA_ARGS__)

#define CRI_ASSERT_TEST_SPECIFIER_ieee_ulp_ne(...)    ,
#define CRI_ASSERT_SPECIFIER_ieee_ulp_ne(...)                                               \
    CRI_ASSERT_SPECIFIER_OP(CRI_IEEE_ULP_T_NE, CRI_IEEE_ULP_NE, ieee_ulp_ne, (              \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_DEF),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_expected, "unexpected", CRI_MKNODE_DEF),  \
        (SINGLE, size_t, cri_ulp, "ulp", CRI_MKNODE_ULPS)                                   \
    ), __VA_ARGS__)

/* Epsilon specifiers */

#define CRI_EPSILON_EQ(Lhs, Rhs, Eps) \
    (((Rhs) == (Lhs)) || ((Rhs) - (Lhs) <= (Eps) && (Lhs) - (Rhs) <= (Eps)))

#define CRI_EPSILON_NE(Lhs, Rhs, Eps) !CRI_EPSILON_EQ(Lhs, Rhs, Eps)

#define CRI_EPSILON_T_EQ(Tag, Lhs, Rhs, Eps) CRI_EPSILON_EQ(Lhs, Rhs, Eps)
#define CRI_EPSILON_T_NE(Tag, Lhs, Rhs, Eps) CRI_EPSILON_NE(Lhs, Rhs, Eps)

# define CRI_MKNODE_EPSS (CRI_MKNODE_EPS, CRI_MKNODE_EPS)

#if defined (CRI_CAPS_LDBL_IS_DBL)
# define CRI_MKNODE_EPS(_, Var, Name) CRI_MKNODE_STR(dbl, Var, Name)
# define CRI_EPS_TYPE double
#else
# define CRI_MKNODE_EPS(_, Var, Name) CRI_MKNODE_STR(ldbl, Var, Name)
# define CRI_EPS_TYPE long double
#endif

#define CRI_ASSERT_TEST_SPECIFIER_epsilon_eq(...) ,
#define CRI_ASSERT_SPECIFIER_epsilon_eq(...)                                                \
    CRI_ASSERT_SPECIFIER_OP(CRI_EPSILON_T_EQ, CRI_EPSILON_EQ, epsilon_eq, (                 \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_DEF),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_expected, "expected", CRI_MKNODE_DEF),    \
        (SINGLE, CRI_EPS_TYPE, cri_epsilon, "epsilon", CRI_MKNODE_EPSS)                     \
    ), __VA_ARGS__)

#define CRI_ASSERT_TEST_SPECIFIER_epsilon_ne(...) ,
#define CRI_ASSERT_SPECIFIER_epsilon_ne(...)                                                \
    CRI_ASSERT_SPECIFIER_OP(CRI_EPSILON_T_NE, CRI_EPSILON_NE, epsilon_ne, (                 \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_DEF),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_expected, "unexpected", CRI_MKNODE_DEF),  \
        (SINGLE, CRI_EPS_TYPE, cri_epsilon, "epsilon", CRI_MKNODE_EPSS)                     \
    ), __VA_ARGS__)

#endif /* !CRITERION_INTERNAL_ASSERT_IEEE_H_ */
