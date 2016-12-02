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

#include "asprintf-compat.h"
#include "common.h"
#include "designated-initializer-compat.h"
#include "preprocess.h"

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

enum cri_assert_result_type {
    CRI_ASSERT_RT_NONE = 0,
    CRI_ASSERT_RT_DATA,
    CRI_ASSERT_RT_FILE,

    CRI_ASSERT_RT_STRING = 1 << 8 | CRI_ASSERT_RT_DATA,
};

struct cri_assert_node {
    const char *repr;
    void *expected;
    void *actual;
    int pass;
    enum cri_assert_result_type rtype;
    size_t nchild;
    size_t maxchild;
    struct cri_assert_node *children;
};

CR_BEGIN_C_API

CR_API void cri_assert_node_init(struct cri_assert_node *node);
CR_API struct cri_assert_node *cri_assert_node_add(struct cri_assert_node *tree,
        struct cri_assert_node *node);
CR_API void cri_assert_node_negate(struct cri_assert_node *tree);
CR_API void cri_assert_node_term(struct cri_assert_node *tree);
CR_API void cri_assert_node_send(const char *file, size_t line, struct cri_assert_node *tree);
CR_API char *cri_assert_message(const char *fmt, ...);
CR_API char *cri_string_xxd(const void *data, size_t size);

CR_END_C_API

#define CRI_SPECIFIER_INDIRECT()      CRI_ASSERT_SPECIFIER

#define CRI_ASSERT_SPECIFIER(Spec)    CRI_IF_DEFINED(CRI_ASSERT_TEST_SPECIFIER_ ## Spec, CR_CONCAT2, (CRI_ASSERT_SPECIFIER_, Spec), CRI_ASSERT_SPECIFIER_VALUE, (Spec))

#define CRI_ASSERT_FAIL(File, Line, Fail, ...)                           \
    CR_EVAL(do {                                                         \
        struct cri_assert_node cri_root;                                 \
        cri_assert_node_init(&cri_root);                                 \
        cri_root.repr = cri_assert_message("x" CR_VA_TAIL(__VA_ARGS__)); \
        cri_assert_node_send(File, Line, &cri_root);                     \
        Fail();                                                          \
    } while (0))

#define CRI_ASSERT_CALL(File, Line, Fail, Condition, ...)                    \
    CR_EVAL(do {                                                             \
        struct cri_assert_node cri_tmpn, cri_root, *cri_node = &cri_root;    \
        (void) cri_tmpn;                                                     \
        (void) cri_node;                                                     \
        cri_assert_node_init(&cri_root);                                     \
        int cri_cond, cri_cond_un, *cri_pass = &cri_cond_un;                 \
        int cri_cond_def = 1;                                                \
        (void) cri_cond_def;                                                 \
        (void) cri_pass;                                                     \
        cri_cond_un = CRI_ASSERT_SPECIFIER(Condition);                       \
        cri_cond = cri_cond_un;                                              \
        if (!cri_cond) {                                                     \
            cri_root.repr = cri_assert_message("x" CR_VA_TAIL(__VA_ARGS__)); \
            cri_assert_node_send(File, Line, &cri_root);                     \
        }                                                                    \
        if (!cri_cond)                                                       \
            Fail();                                                          \
    } while (0))

#define CRI_ASSERT_TYPE_TAG(Tag)           CR_EXPAND(CRI_ASSERT_TYPE_TAG_(Tag))
#define CRI_ASSERT_TYPE_TAG_(Tag)          CRI_IF_DEFINED(CRI_ASSERT_TEST_TAG_ ## Tag, CR_VA_HEAD(CRI_ASSERT_TYPE_TAG_ ## Tag), , CRI_ASSERT_TYPE_TAG_UNKNOWN, (Tag))

#define CRI_USER_TAG_ID(Id, Tag)           CR_CONCAT(cr_user_ ## Id ## _, CRI_ASSERT_TYPE_TAG_ID(Tag))

#define CRI_ASSERT_TYPE_TAG_ID(Tag)        CR_EXPAND(CRI_ASSERT_TYPE_TAG_ID_(Tag))
#define CRI_ASSERT_TYPE_TAG_ID_(Tag)       CRI_IF_DEFINED_NODEFER(CRI_ASSERT_TEST_TAG_ ## Tag, CR_VA_HEAD(CRI_ASSERT_TYPE_TAG_ID_ ## Tag), , CRI_ASSERT_SWALLOW_KEYWORD, (Tag))

#define CRI_ASSERT_TYPE_TAG_ARRLEN(Tag)    CR_EXPAND(CRI_ASSERT_TYPE_TAG_ARRLEN_(Tag))
#define CRI_ASSERT_TYPE_TAG_ARRLEN_(Tag)                                                                   \
    CRI_IF_DEFINED(CRI_ASSERT_TEST_TAG_ ## Tag,                                                            \
            ((intptr_t) &((*(char (*)[])((char volatile *) 0))CR_VA_TAIL(CRI_ASSERT_TYPE_TAG_ ## Tag))), , \
            CRI_ASSERT_TYPE_TAG_UNKNOWN, (Tag))

#define CRI_ASSERT_TYPE_TAG_UNKNOWN(Tag)    CR_COMPILE_ERROR(Unknown tag #Tag.)

#define CRI_ASSERT_SWALLOW_KEYWORD(Tag)     CRI_IF_DEFINED(CRI_ASSERT_TEST_KW_ ## Tag, CRI_ASSERT_SWALLOW_KW_ ## Tag, , Tag, )
#define CRI_ASSERT_TEST_KW_struct    ,
#define CRI_ASSERT_SWALLOW_KW_struct
#define CRI_ASSERT_TEST_KW_class     ,
#define CRI_ASSERT_SWALLOW_KW_class
#define CRI_ASSERT_TEST_KW_enum      ,
#define CRI_ASSERT_SWALLOW_KW_enum
#define CRI_ASSERT_TEST_KW_union     ,
#define CRI_ASSERT_SWALLOW_KW_union

#define CRI_ASSERT_TEST_TAG_i8           ,
#define CRI_ASSERT_TEST_TAGC_i8()    ,
#define CRI_ASSERT_TYPE_TAG_i8           int8_t,
#define CRI_ASSERT_TYPE_TAG_ID_i8        i8,

#define CRI_ASSERT_TEST_TAG_i16          ,
#define CRI_ASSERT_TEST_TAGC_i16()    ,
#define CRI_ASSERT_TYPE_TAG_i16          int16_t,
#define CRI_ASSERT_TYPE_TAG_ID_i16       i16,

#define CRI_ASSERT_TEST_TAG_i32          ,
#define CRI_ASSERT_TEST_TAGC_i32()    ,
#define CRI_ASSERT_TYPE_TAG_i32          int32_t,
#define CRI_ASSERT_TYPE_TAG_ID_i32       i32,

#define CRI_ASSERT_TEST_TAG_i64          ,
#define CRI_ASSERT_TEST_TAGC_i64()    ,
#define CRI_ASSERT_TYPE_TAG_i64          int64_t,
#define CRI_ASSERT_TYPE_TAG_ID_i64       i64,

#define CRI_ASSERT_TEST_TAG_u8           ,
#define CRI_ASSERT_TEST_TAGC_u8()    ,
#define CRI_ASSERT_TYPE_TAG_u8           uint8_t,
#define CRI_ASSERT_TYPE_TAG_ID_u8        u8,

#define CRI_ASSERT_TEST_TAG_u16          ,
#define CRI_ASSERT_TEST_TAGC_u16()    ,
#define CRI_ASSERT_TYPE_TAG_u16          uint16_t,
#define CRI_ASSERT_TYPE_TAG_ID_u16       u16,

#define CRI_ASSERT_TEST_TAG_u32          ,
#define CRI_ASSERT_TEST_TAGC_u32()    ,
#define CRI_ASSERT_TYPE_TAG_u32          uint32_t,
#define CRI_ASSERT_TYPE_TAG_ID_u32       u32,

#define CRI_ASSERT_TEST_TAG_u64          ,
#define CRI_ASSERT_TEST_TAGC_u64()    ,
#define CRI_ASSERT_TYPE_TAG_u64          uint64_t,
#define CRI_ASSERT_TYPE_TAG_ID_u64       u64,

#define CRI_ASSERT_TEST_TAG_int          ,
#define CRI_ASSERT_TEST_TAGC_int()    ,
#define CRI_ASSERT_TYPE_TAG_int          int,
#define CRI_ASSERT_TYPE_TAG_ID_int       int,

#define CRI_ASSERT_TEST_TAG_uint         ,
#define CRI_ASSERT_TEST_TAGC_uint()    ,
#define CRI_ASSERT_TYPE_TAG_uint         unsigned int,
#define CRI_ASSERT_TYPE_TAG_ID_uint      uint,

#define CRI_ASSERT_TEST_TAG_long         ,
#define CRI_ASSERT_TEST_TAGC_long()    ,
#define CRI_ASSERT_TYPE_TAG_long         long,
#define CRI_ASSERT_TYPE_TAG_ID_long      long,

#define CRI_ASSERT_TEST_TAG_ulong        ,
#define CRI_ASSERT_TEST_TAGC_ulong()    ,
#define CRI_ASSERT_TYPE_TAG_ulong        unsigned long,
#define CRI_ASSERT_TYPE_TAG_ID_ulong     ulong,

#define CRI_ASSERT_TEST_TAG_llong        ,
#define CRI_ASSERT_TEST_TAGC_llong()    ,
#define CRI_ASSERT_TYPE_TAG_llong        long long,
#define CRI_ASSERT_TYPE_TAG_ID_llong     llong,

#define CRI_ASSERT_TEST_TAG_ullong       ,
#define CRI_ASSERT_TEST_TAGC_ullong()    ,
#define CRI_ASSERT_TYPE_TAG_ullong       unsigned long long,
#define CRI_ASSERT_TYPE_TAG_ID_ullong    ullong,

#define CRI_ASSERT_TEST_TAG_ptr          ,
#define CRI_ASSERT_TEST_TAGC_ptr()    ,
#define CRI_ASSERT_TYPE_TAG_ptr          void *,
#define CRI_ASSERT_TYPE_TAG_ID_ptr       ptr,

#define CRI_ASSERT_TEST_TAG_str          ,
#define CRI_ASSERT_TEST_TAGC_str()    ,
#ifdef __cplusplus
# define CRI_ASSERT_TYPE_TAG_str      std::string,
#else
# define CRI_ASSERT_TYPE_TAG_str      char *,
#endif
#define CRI_ASSERT_TYPE_TAG_ID_str    str,

#define CRI_ASSERT_TEST_TAG_wcs       ,
#define CRI_ASSERT_TEST_TAGC_wcs()    ,
#ifdef __cplusplus
# define CRI_ASSERT_TYPE_TAG_wcs       std::wstring,
#else
# define CRI_ASSERT_TYPE_TAG_wcs       wchar_t *,
#endif
#define CRI_ASSERT_TYPE_TAG_ID_wcs     wcs,

#define CRI_ASSERT_TEST_TAG_tcs        ,
#define CRI_ASSERT_TEST_TAGC_tcs()    ,
#define CRI_ASSERT_TYPE_TAG_tcs        _TCHAR *,
#define CRI_ASSERT_TYPE_TAG_ID_tcs     tcs,

#define CRI_ASSERT_TEST_TAG_flt        ,
#define CRI_ASSERT_TEST_TAGC_flt()    ,
#define CRI_ASSERT_TYPE_TAG_flt        float,
#define CRI_ASSERT_TYPE_TAG_ID_flt     flt,

#define CRI_ASSERT_TEST_TAG_dbl        ,
#define CRI_ASSERT_TEST_TAGC_dbl()    ,
#define CRI_ASSERT_TYPE_TAG_dbl        double,
#define CRI_ASSERT_TYPE_TAG_ID_dbl     dbl,

#define CRI_ASSERT_TEST_TAG_ldbl       ,
#define CRI_ASSERT_TEST_TAGC_ldbl()    ,
#define CRI_ASSERT_TYPE_TAG_ldbl       long double,
#define CRI_ASSERT_TYPE_TAG_ID_ldbl    ldbl,

#define CRI_ASSERT_TEST_TAG_mem        ,
#define CRI_ASSERT_TEST_TAGC_mem()    ,
#define CRI_ASSERT_TYPE_TAG_mem        struct cr_mem,
#define CRI_ASSERT_TYPE_TAG_ID_mem     mem,

#define CRI_ASSERT_TEST_TAG_type       ,
#define CRI_ASSERT_TEST_TAGC_type(x)      CRI_ASSERT_TEST_TAGC_type_
#define CRI_ASSERT_TEST_TAGC_type_()      ,
#define CRI_ASSERT_TYPE_TAG_type(T)       T,
#define CRI_ASSERT_TYPE_TAG_ID_type(T)    CRI_ASSERT_SWALLOW_KEYWORD(T),

#ifdef __cplusplus
# include <complex>

# define CRI_ASSERT_TEST_TAG_cx_flt        ,
# define CRI_ASSERT_TEST_TAGC_cx_flt()    ,
# define CRI_ASSERT_TYPE_TAG_cx_flt        std::complex<float>
# define CRI_ASSERT_TEST_TAG_ID_cx_flt     cx_flt,

# define CRI_ASSERT_TEST_TAG_cx_dbl        ,
# define CRI_ASSERT_TEST_TAGC_cx_dbl()    ,
# define CRI_ASSERT_TYPE_TAG_cx_dbl        std::complex<double>
# define CRI_ASSERT_TEST_TAG_ID_cx_dbl     cx_dbl,

# define CRI_ASSERT_TEST_TAG_cx_ldbl       ,
# define CRI_ASSERT_TEST_TAGC_cx_ldbl()    ,
# define CRI_ASSERT_TYPE_TAG_cx_ldbl       std::complex<long double>
# define CRI_ASSERT_TEST_TAG_ID_cx_ldbl    cx_ldbl,

#elif __STDC_VERSION__ == 199901L && !defined (__STDC_NO_COMPLEX__)
# include <complex.h>

# define CRI_ASSERT_TEST_TAG_cx_flt        ,
# define CRI_ASSERT_TEST_TAGC_cx_flt()    ,
# define CRI_ASSERT_TYPE_TAG_cx_flt        _Complex float
# define CRI_ASSERT_TEST_TAG_ID_cx_flt     cx_flt,

# define CRI_ASSERT_TEST_TAG_cx_dbl        ,
# define CRI_ASSERT_TEST_TAGC_cx_dbl()    ,
# define CRI_ASSERT_TYPE_TAG_cx_dbl        _Complex double
# define CRI_ASSERT_TEST_TAG_ID_cx_dbl     cx_dbl,

# define CRI_ASSERT_TEST_TAG_cx_ldbl       ,
# define CRI_ASSERT_TEST_TAGC_cx_ldbl()    ,
# define CRI_ASSERT_TYPE_TAG_cx_ldbl       _Complex long double
# define CRI_ASSERT_TEST_TAG_ID_cx_ldbl    cx_ldbl,

#endif

#define CRI_ASSERT_SPEC_OP_LEN(...)            \
    CR_EXPAND(CR_VA_TAIL_SELECT64(__VA_ARGS__, \
            3, 3, 3, 3,                        \
            3, 3, 3, 3, 3, 3, 3, 3, 3, 3,      \
            3, 3, 3, 3, 3, 3, 3, 3, 3, 3,      \
            3, 3, 3, 3, 3, 3, 3, 3, 3, 3,      \
            3, 3, 3, 3, 3, 3, 3, 3, 3, 3,      \
            3, 3, 3, 3, 3, 3, 3, 3, 3, 3,      \
            3, 3, 3, 3, 3, 3, 3, 2, 2, 2))

#define CRI_ASSERT_SPEC_UNOP_LEN(...)          \
    CR_EXPAND(CR_VA_TAIL_SELECT64(__VA_ARGS__, \
            2, 2, 2, 2,                        \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 1, 1))

#define CRI_ASSERT_SPEC_TRIOP_LEN(...)         \
    CR_EXPAND(CR_VA_TAIL_SELECT64(__VA_ARGS__, \
            4, 4, 4, 4,                        \
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      \
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      \
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      \
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      \
            4, 4, 4, 4, 4, 4, 4, 4, 4, 4,      \
            4, 4, 4, 4, 4, 4, 3, 3, 3, 3))

#define CRI_BINOP(Op, Actual, Ref)    Actual Op Ref
#define CRI_BINOP_EQ(Actual, Ref)     CRI_BINOP(==, Actual, Ref)
#define CRI_BINOP_NE(Actual, Ref)     CRI_BINOP(!=, Actual, Ref)
#define CRI_BINOP_LE(Actual, Ref)     CRI_BINOP(<=, Actual, Ref)
#define CRI_BINOP_LT(Actual, Ref)     CRI_BINOP(<, Actual, Ref)
#define CRI_BINOP_GE(Actual, Ref)     CRI_BINOP(>=, Actual, Ref)
#define CRI_BINOP_GT(Actual, Ref)     CRI_BINOP(>, Actual, Ref)

#ifdef __cplusplus
# define CRI_BINOP_T_EQ(Tag, Actual, Ref)    CRI_BINOP_EQ(Actual, Ref)
# define CRI_BINOP_T_NE(Tag, Actual, Ref)    CRI_BINOP_NE(Actual, Ref)
# define CRI_BINOP_T_LE(Tag, Actual, Ref)    CRI_BINOP_LE(Actual, Ref)
# define CRI_BINOP_T_LT(Tag, Actual, Ref)    CRI_BINOP_LT(Actual, Ref)
# define CRI_BINOP_T_GE(Tag, Actual, Ref)    CRI_BINOP_GE(Actual, Ref)
# define CRI_BINOP_T_GT(Tag, Actual, Ref)    CRI_BINOP_GT(Actual, Ref)

# define CRI_UNOP_ZERO(X)                    ::criterion::internal::operators::zero(X)
# define CRI_UNOP_T_ZERO(Tag, X)             ::criterion::internal::operators::zero(X)

# include <locale>
# include <codecvt>

/* *INDENT-OFF* */
namespace criterion { namespace internal { namespace operators {
/* *INDENT-ON* */

# define CRI_DEFINE_MEMBER_DETECTOR(Member)                                                 \
    template <class T>                                                                      \
    class HasMember_ ## Member {                                                            \
        private:                                                                            \
            using yes = char[2];                                                            \
            using no  = char[1];                                                            \
            struct fallback { int Member; };                                                \
            struct derived : T, fallback {};                                                \
            template <class U> static no &test(decltype (U::Member) *);                     \
            template <typename U> static yes &test(U *);                                    \
        public:                                                                             \
            static constexpr bool result = sizeof (test<derived>(nullptr)) == sizeof (yes); \
    };                                                                                      \
    template <class T>                                                                      \
    struct has_member_ ## Member : public std::integral_constant<bool, HasMember_ ## Member<T>::result> {}

CRI_DEFINE_MEMBER_DETECTOR(empty);

template <typename T>
bool zero(const T *t) { return !t; }

template <typename T>
bool zero(const typename std::enable_if<!std::is_pointer<T>::value, T>::type &t) { return !t; }

template <typename T, typename = typename has_member_empty<T>::type>
bool zero(const T &t) { return t.empty(); }

template <> inline bool zero<>(const char *t) { return !*t; }
template <> inline bool zero<>(const wchar_t *t) { return !*t; }

std::ostream &operator<<(std::ostream &s, std::wstring &ws)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> conv;
    s << conv.to_bytes(ws);
    return s;
}

/* *INDENT-OFF* */
}}}
/* *INDENT-ON* */
#else
# define CRI_BINOP_EQ_TAG(Tag, Op, Actual, Ref)    (Op (CRI_USER_TAG_ID(eq, Tag)(&(Actual), &(Ref))))
# define CRI_BINOP_LE_TAG(Tag, Op, Actual, Ref)    (Op (CRI_USER_TAG_ID(le, Tag)(&(Actual), &(Ref))))

# define CRI_BINOP_T_EQ(Tag, Actual, Ref)          (CRI_BINOP_EQ_TAG(Tag, , Actual, Ref))
# define CRI_BINOP_T_NE(Tag, Actual, Ref)          (CRI_BINOP_EQ_TAG(Tag, !, Actual, Ref))
# define CRI_BINOP_T_LE(Tag, Actual, Ref)          (CRI_BINOP_LE_TAG(Tag, , Actual, Ref) || CRI_BINOP_EQ_TAG(Tag, , Actual, Ref))
# define CRI_BINOP_T_LT(Tag, Actual, Ref)          (CRI_BINOP_LE_TAG(Tag, , Actual, Ref))
# define CRI_BINOP_T_GE(Tag, Actual, Ref)          (CRI_BINOP_LE_TAG(Tag, !, Actual, Ref) || CRI_BINOP_EQ_TAG(Tag, , Actual, Ref))
# define CRI_BINOP_T_GT(Tag, Actual, Ref)          (CRI_BINOP_LE_TAG(Tag, !, Actual, Ref))

# define CRI_UNOP_ZERO(X)                          !(X)
# define CRI_UNOP_T_ZERO(Tag, X)                   CRI_USER_TAG_ID(zero, Tag)(&(X))
#endif

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

template <class Char, typename = typename std::enable_if<std::is_same<Char, char>::value>::type>
std::string cri_val_escape(Char const *const &s) { return std::string(s); }

template <class Char, typename = typename std::enable_if<std::is_same<Char, wchar_t>::value>::type>
std::wstring cri_val_escape(Char const *const &s) { return std::wstring(s); }

template <size_t N>
std::string cri_val_escape(const char (&s)[N]) { return std::string(s); }
template <size_t N>
std::wstring cri_val_escape(const wchar_t (&s)[N]) { return std::wstring(s); }

# define CRI_VALUE_ESCAPE(T, X)              cri_val_escape<std::remove_reference<T>::type>(X)
# define CRI_USER_TOSTR(Tag, Var)                                     \
    ([&]() -> char * {                                                \
        using namespace criterion::internal::stream_char_conv;        \
        std::stringstream sstr;                                       \
        sstr << (Var);                                                \
        const std::string str = sstr.str();                           \
        char *out = static_cast<char *>(std::malloc(str.size() + 1)); \
        std::copy(str.begin(), str.end(), out);                       \
        out[str.size()] = '\0';                                       \
        return out;                                                   \
    } ())
# define CRI_ASSERT_UNPRINTABLE(Tag, Var)    (void *) CRI_USER_TOSTR(Tag, Var)
# define CRI_ASSERT_NAMESPACES    \
    using namespace std::rel_ops; \
    using namespace criterion::internal::operators
# define CRI_AUTOTYPE    auto
#else
# define CRI_VALUE_ESCAPE(T, X)              X
# define CRI_USER_TOSTR(Tag, Var)            CRI_USER_TAG_ID(tostr, Tag)(&(Var))
# define CRI_ASSERT_UNPRINTABLE(Tag, Var)    (void *) "<unprintable>"
# define CRI_ASSERT_NAMESPACES    do {} while (0)
# ifdef __GNUC__
#  define CRI_AUTOTYPE            __extension__ __auto_type
# endif
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

#define CRI_ASSERT_SPECIFIER_OP_HELPER(Op, N, ...)    CR_DEFER(CR_CONCAT)(CRI_ASSERT_SPECIFIER_ ## Op, N)(__VA_ARGS__)

/* Binary specifier helpers */

#ifdef CRI_AUTOTYPE
# define CRI_ASSERT_SPECIFIER_OP2(Op, Name, Lhs, Rhs)                 \
    1; do {                                                           \
        CRI_ASSERT_NAMESPACES;                                        \
        CRI_AUTOTYPE cri_lhs = CRI_VALUE_ESCAPE(decltype (Lhs), Lhs); \
        CRI_AUTOTYPE cri_rhs = CRI_VALUE_ESCAPE(decltype (Rhs), Rhs); \
        cri_cond_un = Op (cri_lhs, cri_rhs);                          \
        cri_assert_node_init(&cri_tmpn);                              \
        cri_tmpn.rtype = CRI_ASSERT_RT_STRING;                        \
        cri_tmpn.repr = CR_STR(Name(Lhs, Rhs));                       \
        cri_tmpn.actual = CRI_ASSERT_UNPRINTABLE(Tag, cri_lhs);       \
        cri_tmpn.expected = CRI_ASSERT_UNPRINTABLE(Tag, cri_rhs);     \
        cri_tmpn.pass = cri_cond_un;                                  \
        cri_assert_node_add(cri_node, &cri_tmpn);                     \
    } while (0)
#else
# define CRI_ASSERT_SPECIFIER_OP2(Op, Name, Lhs, Rhs) \
    1; CR_COMPILE_ERROR(Name without a tag parameter is unsupported on this compiler.)
#endif

#define CRI_ASSERT_SPECIFIER_OP3(Op, Name, Tag, Lhs, Rhs) \
    CRI_IF_DEFINED_NODEFER(CRI_ASSERT_TEST_TAGC_ ## Tag(), CRI_ASSERT_SPECIFIER_OP3_SINGLE, , CRI_ASSERT_SPECIFIER_OP3_ARRAY, )(Op, Name, Tag, Lhs, Rhs)

#define CRI_ASSERT_SPECIFIER_OP3_SINGLE(Op, Name, Tag, Lhs, Rhs)                      \
    1; do {                                                                           \
        CRI_ASSERT_NAMESPACES;                                                        \
        CRI_ASSERT_TYPE_TAG(Tag) cri_lhs = CRI_VALUE_ESCAPE(decltype (cri_lhs), Lhs); \
        CRI_ASSERT_TYPE_TAG(Tag) cri_rhs = CRI_VALUE_ESCAPE(decltype (cri_rhs), Rhs); \
        cri_cond_un = Op (Tag, cri_lhs, cri_rhs);                                     \
        cri_assert_node_init(&cri_tmpn);                                              \
        cri_tmpn.rtype = CRI_ASSERT_RT_STRING;                                        \
        cri_tmpn.repr = CR_STR(Name(Tag, Lhs, Rhs));                                  \
        cri_tmpn.actual = (void *) CRI_USER_TOSTR(Tag, cri_lhs);                      \
        cri_tmpn.expected = (void *) CRI_USER_TOSTR(Tag, cri_rhs);                    \
        cri_tmpn.pass = cri_cond_un;                                                  \
        cri_assert_node_add(cri_node, &cri_tmpn);                                     \
    } while (0)

#define CRI_ASSERT_SPECIFIER_OP3_ARRAY(Op, Name, Tag, Lhs, Rhs)                                          \
    1; do {                                                                                              \
        CRI_ASSERT_NAMESPACES;                                                                           \
        CRI_ASSERT_TYPE_TAG(Tag) * cri_lhs = CRI_VALUE_ESCAPE(decltype (cri_lhs), Lhs);                  \
        CRI_ASSERT_TYPE_TAG(Tag) * cri_rhs = CRI_VALUE_ESCAPE(decltype (cri_rhs), Rhs);                  \
        size_t cri_size = CRI_ASSERT_TYPE_TAG_ARRLEN(Tag);                                               \
        cri_assert_node_init(&cri_tmpn);                                                                 \
        cri_tmpn.repr = CR_STR(Name(Tag, Lhs, Rhs));                                                     \
        cri_tmpn.pass = 1;                                                                               \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);                      \
        struct cri_assert_node *cri_node = cri_tmp;                                                      \
        for (size_t cri_i = 0; cri_node->pass && cri_i < cri_size; ++cri_i) {                            \
            cri_assert_node_init(&cri_tmpn);                                                             \
            cri_tmpn.rtype = CRI_ASSERT_RT_STRING;                                                       \
            cr_asprintf((char **) &cri_tmpn.repr,                                                        \
                    CR_STR(Name(CRI_ASSERT_TYPE_TAG_ID(Tag), Lhs[CRI_SIZE_T_FMT], Rhs[CRI_SIZE_T_FMT])), \
                    cri_i, cri_i);                                                                       \
            cri_tmpn.actual = (void *) CRI_USER_TOSTR(Tag, cri_lhs[cri_i]);                              \
            cri_tmpn.expected = (void *) CRI_USER_TOSTR(Tag, cri_rhs[cri_i]);                            \
            cri_tmpn.pass = Op (Tag, cri_lhs[cri_i], cri_rhs[cri_i]);                                    \
            cri_assert_node_add(cri_node, &cri_tmpn);                                                    \
            cri_node->pass = cri_node->pass && cri_tmpn.pass;                                            \
        }                                                                                                \
        cri_cond_un = cri_node->pass;                                                                    \
    } while (0)

/* Unary specifier helpers */

#ifdef CRI_AUTOTYPE
# define CRI_ASSERT_SPECIFIER_UNOP1(Op, Name, Val)                    \
    1; do {                                                           \
        CRI_ASSERT_NAMESPACES;                                        \
        CRI_AUTOTYPE cri_val = CRI_VALUE_ESCAPE(decltype (Val), Val); \
        cri_cond_un = Op (cri_val);                                   \
        cri_assert_node_init(&cri_tmpn);                              \
        cri_tmpn.rtype = CRI_ASSERT_RT_STRING;                        \
        cri_tmpn.repr = CR_STR(Name(Val));                            \
        cri_tmpn.actual = CRI_ASSERT_UNPRINTABLE(Tag, cri_val);       \
        cri_tmpn.pass = cri_cond_un;                                  \
        cri_assert_node_add(cri_node, &cri_tmpn);                     \
    } while (0)
#else
# define CRI_ASSERT_SPECIFIER_UNOP1(Op, Name, Val) \
    1; CR_COMPILE_ERROR(Name without a tag parameter is unsupported on this compiler.)
#endif

#define CRI_ASSERT_SPECIFIER_UNOP2(Op, Name, Tag, Val) \
    CRI_IF_DEFINED_NODEFER(CRI_ASSERT_TEST_TAGC_ ## Tag(), CRI_ASSERT_SPECIFIER_UNOP2_SINGLE, , CRI_ASSERT_SPECIFIER_UNOP2_ARRAY, )(Op, Name, Tag, Val)

#define CRI_ASSERT_SPECIFIER_UNOP2_SINGLE(Op, Name, Tag, Val)                         \
    1; do {                                                                           \
        CRI_ASSERT_NAMESPACES;                                                        \
        CRI_ASSERT_TYPE_TAG(Tag) cri_val = CRI_VALUE_ESCAPE(decltype (cri_val), Val); \
        cri_cond_un = Op (Tag, cri_val);                                              \
        cri_assert_node_init(&cri_tmpn);                                              \
        cri_tmpn.rtype = CRI_ASSERT_RT_STRING;                                        \
        cri_tmpn.repr = CR_STR(Name(Tag, Val));                                       \
        cri_tmpn.actual = (void *) CRI_USER_TOSTR(Tag, cri_val);                      \
        cri_tmpn.expected = (void *) "<zero-ish>";                                    \
        cri_tmpn.pass = cri_cond_un;                                                  \
        cri_assert_node_add(cri_node, &cri_tmpn);                                     \
    } while (0)

#define CRI_ASSERT_SPECIFIER_UNOP2_ARRAY(Op, Name, Tag, Val)                                                              \
    1; do {                                                                                                               \
        CRI_ASSERT_NAMESPACES;                                                                                            \
        CRI_ASSERT_TYPE_TAG(Tag) * cri_val = CRI_VALUE_ESCAPE(decltype (cri_val), Val);                                   \
        size_t cri_size = CRI_ASSERT_TYPE_TAG_ARRLEN(Tag);                                                                \
        cri_assert_node_init(&cri_tmpn);                                                                                  \
        cri_tmpn.repr = CR_STR(Name(Tag, Val));                                                                           \
        cri_tmpn.pass = 1;                                                                                                \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);                                       \
        struct cri_assert_node *cri_node = cri_tmp;                                                                       \
        for (size_t cri_i = 0; cri_node->pass && cri_i < cri_size; ++cri_i) {                                             \
            cri_assert_node_init(&cri_tmpn);                                                                              \
            cri_tmpn.rtype = CRI_ASSERT_RT_STRING;                                                                        \
            cr_asprintf((char **) &cri_tmpn.repr, CR_STR(Name(CRI_ASSERT_TYPE_TAG_ID(Tag), val[CRI_SIZE_T_FMT])), cri_i); \
            cri_tmpn.actual = (void *) CRI_USER_TOSTR(Tag, cri_val[cri_i]);                                               \
            cri_tmpn.expected = (void *) "<zero-ish>";                                                                    \
            cri_tmpn.pass = Op (Tag, cri_val[cri_i]);                                                                     \
            cri_assert_node_add(cri_node, &cri_tmpn);                                                                     \
            cri_node->pass = cri_node->pass && cri_tmpn.pass;                                                             \
        }                                                                                                                 \
        cri_cond_un = cri_node->pass;                                                                                     \
    } while (0)

/* Ternary specifier helpers */

#ifdef CRI_AUTOTYPE
# define CRI_ASSERT_SPECIFIER_TERNOP3(Op, Name, Lhs, Rhs, Val, ValType) \
    1; do {                                                             \
        CRI_ASSERT_NAMESPACES;                                          \
        CRI_AUTOTYPE cri_lhs = CRI_VALUE_ESCAPE(decltype (Lhs), Lhs);   \
        CRI_AUTOTYPE cri_rhs = CRI_VALUE_ESCAPE(decltype (Rhs), Rhs);   \
        ValType cri_val = CRI_VALUE_ESCAPE(ValType, Val);               \
        cri_cond_un = Op (cri_lhs, cri_rhs, cri_val);                   \
        cri_assert_node_init(&cri_tmpn);                                \
        cri_tmpn.rtype = CRI_ASSERT_RT_STRING;                          \
        cri_tmpn.repr = CR_STR(Name(Lhs, Rhs, Val));                    \
        cri_tmpn.actual = CRI_ASSERT_UNPRINTABLE(Tag, cri_lhs);         \
        cri_tmpn.expected = CRI_ASSERT_UNPRINTABLE(Tag, cri_rhs);       \
        cri_tmpn.pass = cri_cond_un;                                    \
        cri_assert_node_add(cri_node, &cri_tmpn);                       \
    } while (0)
#else
# define CRI_ASSERT_SPECIFIER_TERNOP3(Op, Name, Lhs, Rhs, Val, ValType) \
    1; CR_COMPILE_ERROR(Name without a tag parameter is unsupported on this compiler.)
#endif

#define CRI_ASSERT_SPECIFIER_TERNOP4(Op, Name, Tag, Lhs, Rhs, Val, ValType) \
    CRI_IF_DEFINED_NODEFER(CRI_ASSERT_TEST_TAGC_ ## Tag(), CRI_ASSERT_SPECIFIER_TERNOP4_SINGLE, , CRI_ASSERT_SPECIFIER_TERNOP4_ARRAY, )(Op, Name, Tag, Lhs, Rhs, Val, ValType)

#define CRI_ASSERT_SPECIFIER_TERNOP4_SINGLE(Op, Name, Tag, Lhs, Rhs, Val, ValType)    \
    1; do {                                                                           \
        CRI_ASSERT_NAMESPACES;                                                        \
        CRI_ASSERT_TYPE_TAG(Tag) cri_lhs = CRI_VALUE_ESCAPE(decltype (cri_lhs), Lhs); \
        CRI_ASSERT_TYPE_TAG(Tag) cri_rhs = CRI_VALUE_ESCAPE(decltype (cri_rhs), Rhs); \
        ValType cri_val = CRI_VALUE_ESCAPE(ValType, Val);                             \
        cri_cond_un = Op (Tag, cri_lhs, cri_rhs, cri_val);                            \
        cri_assert_node_init(&cri_tmpn);                                              \
        cri_tmpn.rtype = CRI_ASSERT_RT_STRING;                                        \
        cri_tmpn.repr = CR_STR(Name(Tag, Val));                                       \
        cri_tmpn.actual = (void *) CRI_USER_TOSTR(Tag, cri_lhs);                      \
        cri_tmpn.expected = (void *) CRI_USER_TOSTR(Tag, cri_rhs);                    \
        cri_tmpn.pass = cri_cond_un;                                                  \
        cri_assert_node_add(cri_node, &cri_tmpn);                                     \
    } while (0)

#define CRI_ASSERT_SPECIFIER_TERNOP4_ARRAY(Op, Name, Tag, Lhs, Rhs, Val, ValType)                             \
    1; do {                                                                                                   \
        CRI_ASSERT_NAMESPACES;                                                                                \
        CRI_ASSERT_TYPE_TAG(Tag) * cri_lhs = CRI_VALUE_ESCAPE(decltype (cri_lhs), Lhs);                       \
        CRI_ASSERT_TYPE_TAG(Tag) * cri_rhs = CRI_VALUE_ESCAPE(decltype (cri_rhs), Rhs);                       \
        ValType cri_val = CRI_VALUE_ESCAPE(ValType, Val);                                                     \
        size_t cri_size = CRI_ASSERT_TYPE_TAG_ARRLEN(Tag);                                                    \
        cri_assert_node_init(&cri_tmpn);                                                                      \
        cri_tmpn.repr = CR_STR(Name(Tag, Val));                                                               \
        cri_tmpn.pass = 1;                                                                                    \
        struct cri_assert_node *cri_tmp = cri_assert_node_add(cri_node, &cri_tmpn);                           \
        struct cri_assert_node *cri_node = cri_tmp;                                                           \
        for (size_t cri_i = 0; cri_node->pass && cri_i < cri_size; ++cri_i) {                                 \
            cri_assert_node_init(&cri_tmpn);                                                                  \
            cri_tmpn.rtype = CRI_ASSERT_RT_STRING;                                                            \
            cr_asprintf((char **) &cri_tmpn.repr,                                                             \
                    CR_STR(Name(CRI_ASSERT_TYPE_TAG_ID(Tag), Lhs[CRI_SIZE_T_FMT], Rhs[CRI_SIZE_T_FMT], Val)), \
                    cri_i, cri_i);                                                                            \
            cri_tmpn.actual = (void *) CRI_USER_TOSTR(Tag, cri_lhs[cri_i]);                                   \
            cri_tmpn.expected = (void *) CRI_USER_TOSTR(Tag, cri_rhs[cri_i]);                                 \
            cri_tmpn.pass = Op (Tag, cri_lhs[cri_i], cri_rhs[cri_i], cri_val);                                \
            cri_assert_node_add(cri_node, &cri_tmpn);                                                         \
            cri_node->pass = cri_node->pass && cri_tmpn.pass;                                                 \
        }                                                                                                     \
        cri_cond_un = cri_node->pass;                                                                         \
    } while (0)

/* Comparison specifiers */

#define CRI_ASSERT_TEST_SPECIFIER_eq(...)          ,
#define CRI_ASSERT_SPECIFIER_eq(...)               CRI_ASSERT_SPECIFIER_OP_HELPER(eq, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_eq2(Lhs, Rhs)         CRI_ASSERT_SPECIFIER_OP2(CRI_BINOP_EQ, eq, Lhs, Rhs)
#define CRI_ASSERT_SPECIFIER_eq3(Tag, Lhs, Rhs)    CRI_ASSERT_SPECIFIER_OP3(CRI_BINOP_T_EQ, eq, Tag, Lhs, Rhs)

#define CRI_ASSERT_TEST_SPECIFIER_ne(...)          ,
#define CRI_ASSERT_SPECIFIER_ne(...)               CRI_ASSERT_SPECIFIER_OP_HELPER(ne, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_ne2(Lhs, Rhs)         CRI_ASSERT_SPECIFIER_OP2(CRI_BINOP_NE, ne, Lhs, Rhs)
#define CRI_ASSERT_SPECIFIER_ne3(Tag, Lhs, Rhs)    CRI_ASSERT_SPECIFIER_OP3(CRI_BINOP_T_NE, ne, Tag, Lhs, Rhs)

#define CRI_ASSERT_TEST_SPECIFIER_le(...)          ,
#define CRI_ASSERT_SPECIFIER_le(...)               CRI_ASSERT_SPECIFIER_OP_HELPER(le, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_le2(Lhs, Rhs)         CRI_ASSERT_SPECIFIER_OP2(CRI_BINOP_LE, le, Lhs, Rhs)
#define CRI_ASSERT_SPECIFIER_le3(Tag, Lhs, Rhs)    CRI_ASSERT_SPECIFIER_OP3(CRI_BINOP_T_LE, le, Tag, Lhs, Rhs)

#define CRI_ASSERT_TEST_SPECIFIER_lt(...)          ,
#define CRI_ASSERT_SPECIFIER_lt(...)               CRI_ASSERT_SPECIFIER_OP_HELPER(lt, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_lt2(Lhs, Rhs)         CRI_ASSERT_SPECIFIER_OP2(CRI_BINOP_LT, lt, Lhs, Rhs)
#define CRI_ASSERT_SPECIFIER_lt3(Tag, Lhs, Rhs)    CRI_ASSERT_SPECIFIER_OP3(CRI_BINOP_T_LT, lt, Tag, Lhs, Rhs)

#define CRI_ASSERT_TEST_SPECIFIER_ge(...)          ,
#define CRI_ASSERT_SPECIFIER_ge(...)               CRI_ASSERT_SPECIFIER_OP_HELPER(ge, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_ge2(Lhs, Rhs)         CRI_ASSERT_SPECIFIER_OP2(CRI_BINOP_GE, ge, Lhs, Rhs)
#define CRI_ASSERT_SPECIFIER_ge3(Tag, Lhs, Rhs)    CRI_ASSERT_SPECIFIER_OP3(CRI_BINOP_T_GE, ge, Tag, Lhs, Rhs)

#define CRI_ASSERT_TEST_SPECIFIER_gt(...)          ,
#define CRI_ASSERT_SPECIFIER_gt(...)               CRI_ASSERT_SPECIFIER_OP_HELPER(gt, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_gt2(Lhs, Rhs)         CRI_ASSERT_SPECIFIER_OP2(CRI_BINOP_GT, gt, Lhs, Rhs)
#define CRI_ASSERT_SPECIFIER_gt3(Tag, Lhs, Rhs)    CRI_ASSERT_SPECIFIER_OP3(CRI_BINOP_T_GT, gt, Tag, Lhs, Rhs)

/* IEEE specifiers */

#ifdef __cplusplus

/* *INDENT-OFF* */
namespace criterion { namespace internal { namespace operators {
/* *INDENT-ON* */

template <typename Float, typename = typename std::enable_if<!std::numeric_limits<Float>::is_iec559>::type>
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
CRI_DEFINE_IEEE_ULP_EQ(ldbl, l)
#endif

#define CRI_ASSERT_TEST_SPECIFIER_ieee_ulp_eq(...)               ,
#define CRI_ASSERT_SPECIFIER_ieee_ulp_eq(...)                    CRI_ASSERT_SPECIFIER_OP_HELPER(ieee_ulp_eq, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_ieee_ulp_eq2(Lhs, Rhs, Ulp)         CRI_ASSERT_SPECIFIER_TERNOP3(CRI_IEEE_ULP_EQ, ieee_ulp_eq, Lhs, Rhs, Ulp, size_t)
#define CRI_ASSERT_SPECIFIER_ieee_ulp_eq3(Tag, Lhs, Rhs, Ulp)    CRI_ASSERT_SPECIFIER_TERNOP4(CRI_IEEE_ULP_T_EQ, ieee_ulp_eq, Tag, Lhs, Rhs, Ulp, size_t)

#define CRI_ASSERT_TEST_SPECIFIER_ieee_ulp_ne(...)               ,
#define CRI_ASSERT_SPECIFIER_ieee_ulp_ne(...)                    CRI_ASSERT_SPECIFIER_OP_HELPER(ieee_ulp_ne, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_ieee_ulp_ne2(Lhs, Rhs, Ulp)         CRI_ASSERT_SPECIFIER_TERNOP3(CRI_IEEE_ULP_NE, ieee_ulp_ne, Lhs, Rhs, Ulp, size_t)
#define CRI_ASSERT_SPECIFIER_ieee_ulp_ne3(Tag, Lhs, Rhs, Ulp)    CRI_ASSERT_SPECIFIER_TERNOP4(CRI_IEEE_ULP_T_NE, ieee_ulp_ne, Tag, Lhs, Rhs, Ulp, size_t)

/* Epsilon specifiers */

#define CRI_EPSILON_EQ(Lhs, Rhs, Eps)                           ((Rhs) - (Lhs) <= (Eps) && (Lhs) - (Rhs) <= (Eps))
#define CRI_EPSILON_NE(Lhs, Rhs, Eps)                           !CRI_EPSILON_EQ(Lhs, Rhs, Eps)

#define CRI_EPSILON_T_EQ(Tag, Lhs, Rhs, Eps)                    CRI_EPSILON_EQ(Lhs, Rhs, Eps)
#define CRI_EPSILON_T_NE(Tag, Lhs, Rhs, Eps)                    CRI_EPSILON_NE(Lhs, Rhs, Eps)

#define CRI_ASSERT_TEST_SPECIFIER_epsilon_eq(...)               ,
#define CRI_ASSERT_SPECIFIER_epsilon_eq(...)                    CRI_ASSERT_SPECIFIER_OP_HELPER(epsilon_eq, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_epsilon_eq2(Lhs, Rhs, Eps)         CRI_ASSERT_SPECIFIER_TERNOP3(CRI_EPSILON_EQ, epsilon_eq, Lhs, Rhs, Eps, long double)
#define CRI_ASSERT_SPECIFIER_epsilon_eq3(Tag, Lhs, Rhs, Eps)    CRI_ASSERT_SPECIFIER_TERNOP4(CRI_EPSILON_T_EQ, epsilon_eq, Tag, Lhs, Rhs, Eps, long double)

#define CRI_ASSERT_TEST_SPECIFIER_epsilon_ne(...)               ,
#define CRI_ASSERT_SPECIFIER_epsilon_ne(...)                    CRI_ASSERT_SPECIFIER_OP_HELPER(epsilon_ne, CRI_ASSERT_SPEC_OP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_epsilon_ne2(Lhs, Rhs, Eps)         CRI_ASSERT_SPECIFIER_TERNOP3(CRI_EPSILON_NE, epsilon_ne, Lhs, Rhs, Eps, long double)
#define CRI_ASSERT_SPECIFIER_epsilon_ne3(Tag, Lhs, Rhs, Eps)    CRI_ASSERT_SPECIFIER_TERNOP4(CRI_EPSILON_T_NE, epsilon_ne, Tag, Lhs, Rhs, Eps, long double)

/* Other specifiers */

#define CRI_ASSERT_TEST_SPECIFIER_zero(...)    ,
#define CRI_ASSERT_SPECIFIER_zero(...)         CRI_ASSERT_SPECIFIER_OP_HELPER(zero, CRI_ASSERT_SPEC_UNOP_LEN(__VA_ARGS__), __VA_ARGS__)
#define CRI_ASSERT_SPECIFIER_zero1(X)          CRI_ASSERT_SPECIFIER_UNOP1(CRI_UNOP_ZERO, zero, X)
#define CRI_ASSERT_SPECIFIER_zero2(Tag, X)     CRI_ASSERT_SPECIFIER_UNOP2(CRI_UNOP_T_ZERO, zero, Tag, X)

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
CRI_ASSERT_DECLARE_NATIVE_FN(flt, "f")
CRI_ASSERT_DECLARE_NATIVE_FN(dbl, "f")
CRI_ASSERT_DECLARE_NATIVE_FN(ldbl, "Lf")
CRI_ASSERT_DECLARE_STR_FN(str, "s")
CRI_ASSERT_DECLARE_STR_FN(wcs, "ls")
#endif

#if defined (_WIN32)
# include <tchar.h>
# if defined (_UNICODE)
CRI_ASSERT_DECLARE_NATIVE_FN(tcs, "ls")
# else
CRI_ASSERT_DECLARE_NATIVE_FN(tcs, "s")
# endif
#endif

#ifdef __cplusplus

# define CRI_ASSERT_DECLARE_COMPLEX_FN(Tag, Fmt)                      \
    static inline int CRI_USER_TAG_ID(eq, Tag)(                       \
        CRI_ASSERT_TYPE_TAG(Tag) actual,                              \
        CRI_ASSERT_TYPE_TAG(Tag) expected)                            \
    {                                                                 \
        return actual == expected ? 1 : 0;                            \
    }                                                                 \
    static inline char *CRI_USER_TAG_ID(tostr, Tag)(                  \
        CRI_ASSERT_TYPE_TAG(Tag) * e)                                 \
    {                                                                 \
        char *str = NULL;                                             \
        cr_asprintf(&str, "%" Fmt " + i%" Fmt, e->real(), e->imag()); \
        return str;                                                   \
    }

CRI_ASSERT_DECLARE_COMPLEX_FN(cx_flt, "f")
CRI_ASSERT_DECLARE_COMPLEX_FN(cx_dbl, "f")
CRI_ASSERT_DECLARE_COMPLEX_FN(cx_ldbl, "Lf")

#elif __STDC_VERSION__ == 199901L && !defined (__STDC_NO_COMPLEX__)

# define CRI_ASSERT_DECLARE_COMPLEX_FN(Tag, Fmt, S)                             \
    static inline int CR_CONCAT(cr_user_eq_, CRI_ASSERT_TYPE_TAG_ID(Tag))(      \
        CRI_ASSERT_TYPE_TAG(Tag) actual,                                        \
        CRI_ASSERT_TYPE_TAG(Tag) expected)                                      \
    {                                                                           \
        return actual == expected ? 1 : 0;                                      \
    }                                                                           \
    static inline char *CR_CONCAT(cr_user_tostr_, CRI_ASSERT_TYPE_TAG_ID(Tag))( \
        CRI_ASSERT_TYPE_TAG(Tag) * e)                                           \
    {                                                                           \
        char *str = NULL;                                                       \
        cr_asprintf(&str, "%" Fmt " + i%" Fmt, creal ## S(*e), cimag ## S(*e)); \
        return str;                                                             \
    }

CRI_ASSERT_DECLARE_COMPLEX_FN(cx_flt, "f", f)
CRI_ASSERT_DECLARE_COMPLEX_FN(cx_dbl, "f", )
CRI_ASSERT_DECLARE_COMPLEX_FN(cx_ldbl, "Lf", l)
#endif

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

static inline int cr_user_eq_mem(struct cr_mem *m1, struct cr_mem *m2)
{
    return m1->size == m2->size ? !memcmp(m1->data, m2->data, m1->size) : 0;
}

static inline int cr_user_cmp_mem(struct cr_mem *m1, struct cr_mem *m2)
{
    return m1->size == m2->size ? memcmp(m1->data, m2->data, m1->size) : (m1->size > m2->size ? 1 : -1);
}

static inline char *cr_user_tostr_mem(struct cr_mem *m)
{
    return cri_string_xxd(m->data, m->size);
}
#endif

#undef cr_assert_user
#define cr_assert_user(File, Line, Fail, ...) \
    CRI_ASSERT_CALL(File, Line, Fail, CR_VA_HEAD(__VA_ARGS__), , CR_VA_TAIL(__VA_ARGS__))

#undef cr_assert_fail_user
#define cr_assert_fail_user(File, Line, Fail, ...)    CRI_ASSERT_FAIL(File, Line, Fail, , __VA_ARGS__)

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
