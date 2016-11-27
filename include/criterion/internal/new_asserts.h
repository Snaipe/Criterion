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
#ifndef CRITERION_INTERNAL_NEW_ASSERTS_H_
#define CRITERION_INTERNAL_NEW_ASSERTS_H_

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
# include <ostream>
#endif

#include "asprintf-compat.h"
#include "common.h"
#include "designated-initializer-compat.h"
#include "preprocess.h"
#include "capabilities.h"

#include "assert/types.h"
#include "assert/tag.h"
#include "assert/op.h"

#define CRI_SPECIFIER_INDIRECT()      CRI_ASSERT_SPECIFIER

#define CRI_ASSERT_SPECIFIER(Spec)                      \
    CRI_IF_DEFINED(CRI_ASSERT_TEST_SPECIFIER_ ## Spec,  \
        CR_CONCAT2, (CRI_ASSERT_SPECIFIER_, Spec),      \
        CRI_ASSERT_SPECIFIER_VALUE, (Spec)              \
    )

#define CRI_ASSERT_FAIL(File, Line, Fail, ...)                              \
    CR_EVAL(do {                                                            \
        struct cri_assert_node cri_root;                                    \
        cri_assert_node_init(&cri_root);                                    \
        cri_root.message = cri_assert_message("x" __VA_ARGS__);             \
        cri_assert_node_send(File, Line, &cri_root);                        \
        cri_assert_node_term(&cri_root);                                    \
        Fail();                                                             \
    } while (0))

#define CRI_ASSERT_CALL(File, Line, Fail, Condition, ...)                       \
    CR_EVAL(do {                                                                \
        struct cri_assert_node cri_tmpn, cri_root, *cri_node = &cri_root;       \
        (void) cri_tmpn;                                                        \
        (void) cri_node;                                                        \
        cri_assert_node_init(&cri_root);                                        \
        int cri_cond, cri_cond_un, *cri_pass = &cri_cond_un;                    \
        int cri_cond_def = 1;                                                   \
        (void) cri_cond_def;                                                    \
        (void) cri_pass;                                                        \
        cri_cond_un = CRI_ASSERT_SPECIFIER(Condition);                          \
        cri_cond = cri_cond_un;                                                 \
        if (!cri_cond) {                                                        \
            cri_root.message = cri_assert_message("x" CR_VA_TAIL(__VA_ARGS__)); \
            cri_assert_node_send(File, Line, &cri_root);                        \
        }                                                                       \
        cri_assert_node_term(&cri_root);                                        \
        if (!cri_cond)                                                          \
            Fail();                                                             \
    } while (0))

#ifdef __cplusplus

/* *INDENT-OFF* */
namespace criterion { namespace internal { namespace stream_char_conv {
/* *INDENT-ON* */

std::ostream &operator<<(std::ostream &s, int8_t i)
{
    s << signed (i);
    return s;
}

std::ostream &operator<<(std::ostream &s, uint8_t i)
{
    s << unsigned (i);
    return s;
}

std::ostream &operator<<(std::ostream &s, char c)
{
    s << c;
    return s;
}

/* *INDENT-OFF* */
}}}
/* *INDENT-ON* */

# include <type_traits>
# include <utility>
template <typename T> constexpr T && cri_val_escape(T && t) {
    return std::move(t);
}
template <typename T> constexpr T &cri_val_escape(T &t) { return t; }

template <class Char,
    typename = typename std::enable_if<std::is_same<Char, char>::value>::type>
std::string cri_val_escape(Char const *const &s) { return std::string(s); }

template <class Char,
    typename = typename std::enable_if<std::is_same<Char, wchar_t>::value>::type>
std::wstring cri_val_escape(Char const *const &s) { return std::wstring(s); }

template <size_t N>
std::string cri_val_escape(const char (&s)[N]) { return std::string(s); }
template <size_t N>
std::wstring cri_val_escape(const wchar_t (&s)[N]) { return std::wstring(s); }

# define CRI_VALUE_ESCAPE(T, X) cri_val_escape<std::remove_reference<T>::type>(X)
# define CRI_USER_TOSTR(Tag, Var)                                     \
    ([&]() -> char * {                                                \
        using namespace criterion::internal::stream_char_conv;        \
        std::stringstream sstr;                                       \
        sstr << (Var);                                                \
                                                                      \
        const std::string str = sstr.str();                           \
        char *out = static_cast<char *>(std::malloc(str.size() + 1)); \
        std::copy(str.begin(), str.end(), out);                       \
        out[str.size()] = '\0';                                       \
        return out;                                                   \
    } ())
# define CRI_ASSERT_UNPRINTABLE(Var) CRI_USER_TOSTR(, Var)
# define CRI_ASSERT_NAMESPACES                      \
    using namespace std::rel_ops;                   \
    using namespace criterion::internal::operators
#else
# define CRI_VALUE_ESCAPE(T, X) X
# define CRI_USER_TOSTR(Tag, Var) CRI_USER_TAG_ID(tostr, Tag)(&(Var))
# define CRI_ASSERT_UNPRINTABLE(Var) "<unprintable>"
# define CRI_ASSERT_NAMESPACES do {} while (0)
#endif

#define CRI_ASSERT_SPECIFIER_VALUE(Val)           \
    1; do {                                       \
        cri_cond_un = (Val);                      \
        cri_assert_node_init(&cri_tmpn);          \
        cri_tmpn.repr = CR_STR(Val);              \
        cri_tmpn.pass = cri_cond_un;              \
        cri_assert_node_add(cri_node, &cri_tmpn); \
    } while (0)

/* *INDENT-OFF* */
#define CRI_SIZE_T_FMT %zu
/* *INDENT-ON* */

/* Comparison specifiers */

#define CRI_ASSERT_TEST_SPECIFIER_eq(...)   ,
#define CRI_ASSERT_TEST_SPECIFIER_ne(...)   ,
#define CRI_ASSERT_TEST_SPECIFIER_le(...)   ,
#define CRI_ASSERT_TEST_SPECIFIER_lt(...)   ,
#define CRI_ASSERT_TEST_SPECIFIER_ge(...)   ,
#define CRI_ASSERT_TEST_SPECIFIER_gt(...)   ,

#define CRI_ASSERT_SPECIFIER_eq(...)                                                        \
    CRI_ASSERT_SPECIFIER_OP(CRI_BINOP_T_EQ, CRI_BINOP_EQ, eq, (                             \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_expected, "expected", CRI_MKNODE_STR)     \
    ), __VA_ARGS__)

#define CRI_ASSERT_SPECIFIER_ne(...)                                                        \
    CRI_ASSERT_SPECIFIER_OP(CRI_BINOP_T_NE, CRI_BINOP_NE, ne, (                             \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_unexpected, "unexpected", CRI_MKNODE_STR) \
    ), __VA_ARGS__)

#define CRI_ASSERT_SPECIFIER_lt(...)                                                        \
    CRI_ASSERT_SPECIFIER_OP(CRI_BINOP_T_LT, CRI_BINOP_LT, lt, (                             \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_reference, "reference", CRI_MKNODE_STR)   \
    ), __VA_ARGS__)

#define CRI_ASSERT_SPECIFIER_le(...)                                                        \
    CRI_ASSERT_SPECIFIER_OP(CRI_BINOP_T_LE, CRI_BINOP_LE, le, (                             \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_reference, "reference", CRI_MKNODE_STR)   \
    ), __VA_ARGS__)

#define CRI_ASSERT_SPECIFIER_gt(...)                                                        \
    CRI_ASSERT_SPECIFIER_OP(CRI_BINOP_T_GT, CRI_BINOP_GT, gt, (                             \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_reference, "reference", CRI_MKNODE_STR)   \
    ), __VA_ARGS__)

#define CRI_ASSERT_SPECIFIER_ge(...)                                                        \
    CRI_ASSERT_SPECIFIER_OP(CRI_BINOP_T_GE, CRI_BINOP_GE, ge, (                             \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_reference, "reference", CRI_MKNODE_STR)   \
    ), __VA_ARGS__)

/* IEEE specifiers */

#ifdef __cplusplus

# include <cmath>

/* *INDENT-OFF* */
namespace criterion { namespace internal { namespace operators {
/* *INDENT-ON* */

template <typename Float, typename = typename std::enable_if<std::numeric_limits<Float>::is_iec559>::type>
bool ieee_eq(Float &a, Float &b, size_t ulp)
{
    if (ulp == 0)
        return false;

    /* Not the most efficient but the most portable */
    a = std::nextafter(a, b);
    return a == b ? true : ieee_eq(a, b, ulp - 1);
}

/* *INDENT-OFF* */
}}}
/* *INDENT-ON* */

# define CRI_IEEE_ULP_EQ(Lhs, Rhs, Ulp)           ::criterion::internal::operators::ieee_eq((Lhs), (Rhs), (Ulp))
# define CRI_IEEE_ULP_NE(Lhs, Rhs, Ulp)           !CRI_IEEE_ULP_EQ(Lhs, Rhs, Ulp)

# define CRI_IEEE_ULP_T_EQ(Tag, Lhs, Rhs, Ulp)    CRI_IEEE_ULP_EQ(Lhs, Rhs, Ulp)
# define CRI_IEEE_ULP_T_NE(Tag, Lhs, Rhs, Ulp)    CRI_IEEE_ULP_NE(Lhs, Rhs, Ulp)
#else
# define CRI_IEEE_ULP_EQ(Lhs, Rhs, Ulp) \
    CR_COMPILE_ERROR(ieee_ulp_eq without a tag parameter is unsupported on this compiler.)
# define CRI_IEEE_ULP_NE(Lhs, Rhs, Ulp) \
    CR_COMPILE_ERROR(ieee_ulp_ne without a tag parameter is unsupported on this compiler.)

# define CRI_IEEE_ULP_T_EQ(Tag, Lhs, Rhs, Ulp)    (CRI_USER_TAG_ID(ieee_ulp_eq, Tag)((Lhs), (Rhs), (Ulp)))
# define CRI_IEEE_ULP_T_NE(Tag, Lhs, Rhs, Ulp)    !(CRI_USER_TAG_ID(ieee_ulp_eq, Tag)((Lhs), (Rhs), (Ulp)))

# include <math.h>

# define CRI_DEFINE_IEEE_ULP_EQ(Tag, Suffix)                                  \
    static inline int CRI_USER_TAG_ID(ieee_ulp_eq, Tag)(                      \
        CRI_ASSERT_TYPE_TAG(Tag) a,                                           \
        CRI_ASSERT_TYPE_TAG(Tag) b, size_t ulp)                               \
    {                                                                         \
        if (ulp == 0)                                                         \
            return 0;                                                         \
        a = nextafter ## Suffix(a, b);                                        \
        return a == b ? 1 : CRI_USER_TAG_ID(ieee_ulp_eq, Tag)(a, b, ulp - 1); \
    }

CRI_DEFINE_IEEE_ULP_EQ(flt, f)
CRI_DEFINE_IEEE_ULP_EQ(dbl, )

# if defined (CRI_CAPS_LDBL)
CRI_DEFINE_IEEE_ULP_EQ(ldbl, l)
# endif
#endif

#define CRI_ASSERT_TEST_SPECIFIER_ieee_ulp_eq(...)    ,
#define CRI_ASSERT_TEST_SPECIFIER_ieee_ulp_ne(...)    ,

#define CRI_MKNODE_ULP(_, Var, Name) CRI_MKNODE_STR(sz, Var, Name)

#define CRI_ASSERT_SPECIFIER_ieee_ulp_eq(...)                                               \
    CRI_ASSERT_SPECIFIER_OP(CRI_IEEE_ULP_T_EQ, CRI_IEEE_ULP_EQ, ieee_ulp_eq, (              \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_expected, "expected", CRI_MKNODE_STR),    \
        (SINGLE, size_t, cri_ulp, "ulp", CRI_MKNODE_ULP)                                    \
    ), __VA_ARGS__)

#define CRI_ASSERT_SPECIFIER_ieee_ulp_ne(...)                                               \
    CRI_ASSERT_SPECIFIER_OP(CRI_IEEE_ULP_T_NE, CRI_IEEE_ULP_NE, ieee_ulp_ne, (              \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_expected, "unexpected", CRI_MKNODE_STR),  \
        (SINGLE, size_t, cri_ulp, "ulp", CRI_MKNODE_ULP)                                    \
    ), __VA_ARGS__)

/* Epsilon specifiers */

#define CRI_EPSILON_EQ(Lhs, Rhs, Eps)                ((Rhs) - (Lhs) <= (Eps) && (Lhs) - (Rhs) <= (Eps))
#define CRI_EPSILON_NE(Lhs, Rhs, Eps)                !CRI_EPSILON_EQ(Lhs, Rhs, Eps)

#define CRI_EPSILON_T_EQ(Tag, Lhs, Rhs, Eps)         CRI_EPSILON_EQ(Lhs, Rhs, Eps)
#define CRI_EPSILON_T_NE(Tag, Lhs, Rhs, Eps)         CRI_EPSILON_NE(Lhs, Rhs, Eps)

#define CRI_ASSERT_TEST_SPECIFIER_epsilon_eq(...)    ,
#define CRI_ASSERT_TEST_SPECIFIER_epsilon_ne(...)    ,

#if defined (CRI_CAPS_LDBL)
# define CRI_MKNODE_EPS(_, Var, Name) CRI_MKNODE_STR(ldbl, Var, Name)
# define CRI_EPS_TYPE long double
#else
# define CRI_MKNODE_EPS(_, Var, Name) CRI_MKNODE_STR(dbl, Var, Name)
# define CRI_EPS_TYPE double
#endif

#define CRI_ASSERT_SPECIFIER_epsilon_eq(...)                                                \
    CRI_ASSERT_SPECIFIER_OP(CRI_EPSILON_T_EQ, CRI_EPSILON_EQ, epsilon_eq, (                 \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_expected, "expected", CRI_MKNODE_STR),    \
        (SINGLE, CRI_EPS_TYPE, cri_epsilon, "epsilon", CRI_MKNODE_EPS)                      \
    ), __VA_ARGS__)

#define CRI_ASSERT_SPECIFIER_epsilon_ne(...)                                                \
    CRI_ASSERT_SPECIFIER_OP(CRI_EPSILON_T_NE, CRI_EPSILON_NE, epsilon_ne, (                 \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_actual, "actual", CRI_MKNODE_STR),        \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_expected, "unexpected", CRI_MKNODE_STR),  \
        (SINGLE, CRI_EPS_TYPE, cri_epsilon, "epsilon", CRI_MKNODE_EPS)                      \
    ), __VA_ARGS__)

/* Other specifiers */

#define CRI_ASSERT_TEST_SPECIFIER_zero(...)    ,

#define CRI_ASSERT_SPECIFIER_zero(...)                                              \
    CRI_ASSERT_SPECIFIER_OP(CRI_UNOP_T_ZERO, CRI_UNOP_ZERO, zero, (                 \
        (TAGGED, CRI_AS_GETTYPE(__VA_ARGS__), cri_value, "value", CRI_MKNODE_STR)   \
    ), __VA_ARGS__)

/* Exception specifiers */

#ifdef __cplusplus

# define CRI_ASSERT_TEST_SPECIFIER_throw(...)    ,
# define CRI_ASSERT_SPECIFIER_throw(Ex, Stmt)         \
    cri_cond_def; do {                                \
        cri_assert_node_init(&cri_tmpn);              \
        cri_tmpn.repr = "throw(" #Ex ", " #Stmt ")";  \
        try {                                         \
            Stmt;                                     \
        } catch (Ex &cri_ex) {                        \
            cri_cond_un = 1;                          \
        } catch (std::exception &cri_ex) {            \
            cri_cond_un = 0;                          \
            /* TODO: Add std::exception message */    \
        } catch (...) {                               \
            cri_cond_un = 0;                          \
            /* TODO: Add generic exception message */ \
        }                                             \
        cri_assert_node_add(cri_node, &cri_tmpn);     \
    } while (0)

# define CRI_ASSERT_TEST_SPECIFIER_nothrow(...)    ,
# define CRI_ASSERT_SPECIFIER_nothrow(Stmt)           \
    cri_cond_def; do {                                \
        cri_assert_node_init(&cri_tmpn);              \
        cri_tmpn.repr = "throw(" #Stmt ")";           \
        try {                                         \
            Stmt;                                     \
            cri_cond_un = 1;                          \
        } catch (std::exception &cri_ex) {            \
            cri_cond_un = 0;                          \
            /* TODO: Add std::exception message */    \
        } catch (...) {                               \
            cri_cond_un = 0;                          \
            /* TODO: Add generic exception message */ \
        }                                             \
        cri_assert_node_add(cri_node, &cri_tmpn);     \
    } while (0)

# define CRI_ASSERT_TEST_SPECIFIER_anythrow(...)    ,
# define CRI_ASSERT_SPECIFIER_anythrow(Stmt)          \
    cri_cond_def; do {                                \
        cri_assert_node_init(&cri_tmpn);              \
        cri_tmpn.repr = "anythrow(" #Stmt ")";        \
        try {                                         \
            Stmt;                                     \
            cri_cond_un = 0;                          \
        } catch (...) {                               \
            cri_cond_un = 1;                          \
            /* TODO: Add generic exception message */ \
        }                                             \
        cri_assert_node_add(cri_node, &cri_tmpn);     \
    } while (0)

#endif /* __cplusplus */

/* Logical specifiers */

#define CRI_ASSERT_TEST_SPECIFIER_not(...)    ,
#define CRI_ASSERT_SPECIFIER_not(E)              \
    CRI_OBSTRUCT_N(CRI_SPECIFIER_INDIRECT)()(E); \
    cri_cond_un = !cri_cond_un;                  \
    cri_assert_node_negate(cri_node)

#define CRI_ASSERT_SPECIFIER_ALL_INDIRECT(Cond, E)    ; cri_cond_un = CRI_OBSTRUCT_N(CRI_SPECIFIER_INDIRECT)()(E); Cond = Cond && cri_cond_un
#define CRI_ASSERT_TEST_SPECIFIER_all(...)            ,
#define CRI_ASSERT_SPECIFIER_all(...)                                                                          \
    cri_cond_def; int *cri_pass_orig = cri_pass; cri_pass = &cri_cond_un; do {                                 \
        cri_assert_node_init(&cri_tmpn);                                                                       \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);                            \
        struct cri_assert_node *cri_node = cri_tmp;                                                            \
        int cri_cond_def = 1, cri_cond_un;                                                                     \
        int cri_cond = cri_cond_def CRITERION_APPLY(CRI_ASSERT_SPECIFIER_ALL_INDIRECT, cri_cond, __VA_ARGS__); \
        cri_node->pass = cri_cond;                                                                             \
        *cri_pass = *cri_pass && cri_cond;                                                                     \
    } while (0); cri_pass = cri_pass_orig

#define CRI_ASSERT_SPECIFIER_NONE_INDIRECT(Cond, E)    ; cri_cond_un = CRI_OBSTRUCT_N(CRI_SPECIFIER_INDIRECT)()(E); Cond = Cond && !(cri_cond_un)
#define CRI_ASSERT_TEST_SPECIFIER_none(...)            ,
#define CRI_ASSERT_SPECIFIER_none(...)                                                                          \
    cri_cond_def; int *cri_pass_orig = cri_pass; cri_pass = &cri_cond_un; do {                                  \
        cri_assert_node_init(&cri_tmpn);                                                                        \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);                             \
        struct cri_assert_node *cri_node = cri_tmp;                                                             \
        int cri_cond_def = 1, cri_cond_un;                                                                      \
        int cri_cond = cri_cond_def CRITERION_APPLY(CRI_ASSERT_SPECIFIER_NONE_INDIRECT, cri_cond, __VA_ARGS__); \
        cri_node->pass = cri_cond;                                                                              \
        *cri_pass = *cri_pass && cri_cond;                                                                      \
    } while (0); cri_pass = cri_pass_orig

#define CRI_ASSERT_SPECIFIER_ANY_INDIRECT(Cond, E)    ; Cond = Cond || CRI_OBSTRUCT_N(CRI_SPECIFIER_INDIRECT)()(E)
#define CRI_ASSERT_TEST_SPECIFIER_any(...)            ,
#define CRI_ASSERT_SPECIFIER_any(...)                                                                          \
    cri_cond_def; int *cri_pass_orig = cri_pass; cri_pass = &cri_cond_un; do {                                 \
        cri_assert_node_init(&cri_tmpn);                                                                       \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);                            \
        struct cri_assert_node *cri_node = cri_tmp;                                                            \
        int cri_cond_def = 0;                                                                                  \
        int cri_cond = cri_cond_def CRITERION_APPLY(CRI_ASSERT_SPECIFIER_ANY_INDIRECT, cri_cond, __VA_ARGS__); \
        cri_node->pass = cri_cond;                                                                             \
        *cri_pass = *cri_pass && cri_cond;                                                                     \
    } while (0); cri_pass = cri_pass_orig

#define CRI_ASSERT_DECLARE_NATIVE_CMP_FN(Tag)     \
    static inline int CRI_USER_TAG_ID(le, Tag)(   \
        CRI_ASSERT_TYPE_TAG(Tag) * actual,        \
        CRI_ASSERT_TYPE_TAG(Tag) * expected)      \
    {                                             \
        return *actual < *expected;               \
    }                                             \
    static inline int CRI_USER_TAG_ID(eq, Tag)(   \
        CRI_ASSERT_TYPE_TAG(Tag) * actual,        \
        CRI_ASSERT_TYPE_TAG(Tag) * expected)      \
    {                                             \
        return *actual == *expected;              \
    }                                             \
    static inline int CRI_USER_TAG_ID(zero, Tag)( \
        CRI_ASSERT_TYPE_TAG(Tag) * val)           \
    {                                             \
        return !*val;                             \
    }

#define CRI_ASSERT_DECLARE_NATIVE_FN(Tag, Fmt)       \
    CRI_ASSERT_DECLARE_NATIVE_CMP_FN(Tag)            \
    static inline char *CRI_USER_TAG_ID(tostr, Tag)( \
        CRI_ASSERT_TYPE_TAG(Tag) * e)                \
    {                                                \
        char *str = NULL;                            \
        cr_asprintf(&str, "%" Fmt, *e);              \
        return str;                                  \
    }

#define CRI_ASSERT_DECLARE_STR_FN(Tag, Fmt)          \
    static inline int CRI_USER_TAG_ID(le, Tag)(      \
        CRI_ASSERT_TYPE_TAG(Tag) * actual,           \
        CRI_ASSERT_TYPE_TAG(Tag) * expected)         \
    {                                                \
        return Tag ## cmp(*actual, *expected) < 0;   \
    }                                                \
    static inline int CRI_USER_TAG_ID(eq, Tag)(      \
        CRI_ASSERT_TYPE_TAG(Tag) * actual,           \
        CRI_ASSERT_TYPE_TAG(Tag) * expected)         \
    {                                                \
        return !Tag ## cmp(*actual, *expected);      \
    }                                                \
    static inline int CRI_USER_TAG_ID(zero, Tag)(    \
        CRI_ASSERT_TYPE_TAG(Tag) * val)              \
    {                                                \
        return !Tag ## len(*val);                    \
    }                                                \
    static inline char *CRI_USER_TAG_ID(tostr, Tag)( \
        CRI_ASSERT_TYPE_TAG(Tag) * e)                \
    {                                                \
        char *str = NULL;                            \
        cr_asprintf(&str, "%" Fmt, *e);              \
        return str;                                  \
    }

#ifndef __cplusplus
# include <string.h>
# include <wchar.h>
# include <float.h>

CRI_ASSERT_DECLARE_NATIVE_FN(i8, PRId8)
CRI_ASSERT_DECLARE_NATIVE_FN(i16, PRId16)
CRI_ASSERT_DECLARE_NATIVE_FN(i32, PRId32)
CRI_ASSERT_DECLARE_NATIVE_FN(i64, PRId64)
CRI_ASSERT_DECLARE_NATIVE_FN(u8, PRIu8)
CRI_ASSERT_DECLARE_NATIVE_FN(u16, PRIu16)
CRI_ASSERT_DECLARE_NATIVE_FN(u32, PRIu32)
CRI_ASSERT_DECLARE_NATIVE_FN(u64, PRIu64)
CRI_ASSERT_DECLARE_NATIVE_FN(int, "d")
CRI_ASSERT_DECLARE_NATIVE_FN(uint, "u")
CRI_ASSERT_DECLARE_NATIVE_FN(long, "ld")
CRI_ASSERT_DECLARE_NATIVE_FN(ulong, "lu")
CRI_ASSERT_DECLARE_NATIVE_FN(llong, "lld")
CRI_ASSERT_DECLARE_NATIVE_FN(ullong, "llu")
CRI_ASSERT_DECLARE_NATIVE_FN(ptr, "p")
CRI_ASSERT_DECLARE_STR_FN(str, "s")
CRI_ASSERT_DECLARE_STR_FN(wcs, "ls")

#if defined (FLT_DECIMAL_DIG)
# define CRI_FLT_DIG CR_STR(FLT_DECIMAL_DIG)
#elif defined (__FLT_DECIMAL_DIG__)
# define CRI_FLT_DIG CR_STR(__FLT_DECIMAL_DIG__)
#elif defined (DECIMAL_DIG)
# define CRI_FLT_DIG CR_STR(DECIMAL_DIG)
#else
# define CRI_FLT_DIG "9"
#endif

#if defined (DBL_DECIMAL_DIG)
# define CRI_DBL_DIG CR_STR(DBL_DECIMAL_DIG)
#elif defined (__DBL_DECIMAL_DIG__)
# define CRI_DBL_DIG CR_STR(__DBL_DECIMAL_DIG__)
#elif defined (DECIMAL_DIG)
# define CRI_DBL_DIG CR_STR(DECIMAL_DIG)
#else
# define CRI_DBL_DIG "17"
#endif

#if defined (LDBL_DECIMAL_DIG)
# define CRI_LDBL_DIG CR_STR(LDBL_DECIMAL_DIG)
#elif defined (__LDBL_DECIMAL_DIG__)
# define CRI_LDBL_DIG CR_STR(__LDBL_DECIMAL_DIG__)
#elif defined (DECIMAL_DIG)
# define CRI_LDBL_DIG CR_STR(DECIMAL_DIG)
#else
# define CRI_LDBL_DIG "21"
#endif

CRI_ASSERT_DECLARE_NATIVE_FN(flt, "." CRI_FLT_DIG "g")
CRI_ASSERT_DECLARE_NATIVE_FN(dbl, "." CRI_DBL_DIG "g")

#if defined (CRI_CAPS_LDBL)
CRI_ASSERT_DECLARE_NATIVE_FN(ldbl, "." CRI_LDBL_DIG "Lg")
#endif

# ifdef _WIN32
CRI_ASSERT_DECLARE_NATIVE_FN(sz, "Iu")
# else
CRI_ASSERT_DECLARE_NATIVE_FN(sz, "zu")
# endif
#endif

#if defined (_WIN32)
# include <tchar.h>
# if defined (_UNICODE)
CRI_ASSERT_DECLARE_NATIVE_FN(tcs, "ls")
# else
CRI_ASSERT_DECLARE_NATIVE_FN(tcs, "s")
# endif
#endif

#include "assert/memory.h"

#ifdef CRI_CAPS_COMPLEX
# include "assert/complex.h"
#endif

#undef cr_assert_user
#define cr_assert_user(File, Line, Fail, ...) \
    CRI_ASSERT_CALL(File, Line, Fail, CR_VA_HEAD(__VA_ARGS__), , CR_VA_TAIL(__VA_ARGS__))

#undef cr_assert_fail_user
#define cr_assert_fail_user(File, Line, Fail, ...)    CRI_ASSERT_FAIL(File, Line, Fail, __VA_ARGS__)

#undef cr_assert
#define cr_assert(...)    cr_assert_user(__FILE__, __LINE__, criterion_abort_test, __VA_ARGS__)

#undef cr_expect
#define cr_expect(...)    cr_assert_user(__FILE__, __LINE__, criterion_continue_test, __VA_ARGS__)

#undef cr_assert_fail
#define cr_assert_fail(...)    cr_assert_fail_user(__FILE__, __LINE__, criterion_abort_test, __VA_ARGS__)

#undef cr_expect_fail
#define cr_expect_fail(...)    cr_assert_fail_user(__FILE__, __LINE__, criterion_continue_test, __VA_ARGS__)

#undef cr_skip
#define cr_skip(...)    criterion_skip_test("" __VA_ARGS__)

#endif /* !CRITERION_INTERNAL_NEW_ASSERTS_H_ */
