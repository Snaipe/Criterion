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
#ifndef CRITERION_INTERNAL_ASSERT_TAG_H_
#define CRITERION_INTERNAL_ASSERT_TAG_H_

#define CRI_ASSERT_SWALLOW_KEYWORD(Tag)    CRI_IF_DEFINED(CRI_ASSERT_TEST_KW_ ## Tag, CRI_ASSERT_SWALLOW_KW_ ## Tag, , Tag, )
#define CRI_ASSERT_TEST_KW_struct    ,
#define CRI_ASSERT_SWALLOW_KW_struct
#define CRI_ASSERT_TEST_KW_class     ,
#define CRI_ASSERT_SWALLOW_KW_class
#define CRI_ASSERT_TEST_KW_enum      ,
#define CRI_ASSERT_SWALLOW_KW_enum
#define CRI_ASSERT_TEST_KW_union     ,
#define CRI_ASSERT_SWALLOW_KW_union

#define CRI_ASSERT_TYPE_TAG(Tag)           CR_EXPAND(CRI_ASSERT_TYPE_TAG_(Tag))
#define CRI_ASSERT_TYPE_TAG_(Tag)          CRI_IF_DEFINED_NODEFER(CRI_ASSERT_TEST_TAG_ ## Tag, CR_VA_HEAD(CRI_ASSERT_TYPE_TAG_ ## Tag), , CRI_ASSERT_TYPE_TAG_UNKNOWN, (Tag))

#define CRI_USER_TAG_ID(Id, Tag)           CR_CONCAT(CR_CONCAT(cr_user_, CRI_ASSERT_TYPE_TAG_ID(Tag)), _ ## Id)

#define CRI_ASSERT_TYPE_TAG_ID(Tag)        CR_EXPAND(CRI_ASSERT_TYPE_TAG_ID_(Tag))
#define CRI_ASSERT_TYPE_TAG_ID_(Tag)       CRI_IF_DEFINED_NODEFER(CRI_ASSERT_TEST_TAG_ ## Tag, CR_VA_HEAD(CRI_ASSERT_TYPE_TAG_ID_ ## Tag), , CRI_ASSERT_SWALLOW_KEYWORD, (Tag))

#define CRI_ASSERT_TYPE_TAG_ARRLEN(Tag)    CR_EXPAND(CRI_ASSERT_TYPE_TAG_ARRLEN_(Tag))
#define CRI_ASSERT_TYPE_TAG_ARRLEN_(Tag)                                                                   \
    CRI_IF_DEFINED(CRI_ASSERT_TEST_TAG_ ## Tag,                                                            \
            ((intptr_t) &((*(char (*)[])((char volatile *) 0))CR_VA_TAIL(CRI_ASSERT_TYPE_TAG_ ## Tag))), , \
            CRI_ASSERT_TYPE_TAG_UNKNOWN, (Tag))

#define CRI_ASSERT_TYPE_TAG_UNKNOWN(Tag)        COMPILE_ERROR(Unknown tag #Tag.)

#define CRI_ASSERT_TEST_TAG_i8          ,
#define CRI_ASSERT_TEST_TAGC_i8()       ,
#define CRI_ASSERT_TYPE_TAG_i8          int8_t,
#define CRI_ASSERT_TYPE_TAG_ID_i8       i8,

#define CRI_ASSERT_TEST_TAG_i16         ,
#define CRI_ASSERT_TEST_TAGC_i16()      ,
#define CRI_ASSERT_TYPE_TAG_i16         int16_t,
#define CRI_ASSERT_TYPE_TAG_ID_i16      i16,

#define CRI_ASSERT_TEST_TAG_i32         ,
#define CRI_ASSERT_TEST_TAGC_i32()      ,
#define CRI_ASSERT_TYPE_TAG_i32         int32_t,
#define CRI_ASSERT_TYPE_TAG_ID_i32      i32,

#define CRI_ASSERT_TEST_TAG_i64         ,
#define CRI_ASSERT_TEST_TAGC_i64()      ,
#define CRI_ASSERT_TYPE_TAG_i64         int64_t,
#define CRI_ASSERT_TYPE_TAG_ID_i64      i64,

#define CRI_ASSERT_TEST_TAG_u8          ,
#define CRI_ASSERT_TEST_TAGC_u8()       ,
#define CRI_ASSERT_TYPE_TAG_u8          uint8_t,
#define CRI_ASSERT_TYPE_TAG_ID_u8       u8,

#define CRI_ASSERT_TEST_TAG_u16         ,
#define CRI_ASSERT_TEST_TAGC_u16()      ,
#define CRI_ASSERT_TYPE_TAG_u16         uint16_t,
#define CRI_ASSERT_TYPE_TAG_ID_u16      u16,

#define CRI_ASSERT_TEST_TAG_u32         ,
#define CRI_ASSERT_TEST_TAGC_u32()      ,
#define CRI_ASSERT_TYPE_TAG_u32         uint32_t,
#define CRI_ASSERT_TYPE_TAG_ID_u32      u32,

#define CRI_ASSERT_TEST_TAG_u64         ,
#define CRI_ASSERT_TEST_TAGC_u64()      ,
#define CRI_ASSERT_TYPE_TAG_u64         uint64_t,
#define CRI_ASSERT_TYPE_TAG_ID_u64      u64,

#define CRI_ASSERT_TEST_TAG_chr         ,
#define CRI_ASSERT_TEST_TAGC_chr()      ,
#define CRI_ASSERT_TYPE_TAG_chr         char,
#define CRI_ASSERT_TYPE_TAG_ID_chr      chr,

#define CRI_ASSERT_TEST_TAG_int         ,
#define CRI_ASSERT_TEST_TAGC_int()      ,
#define CRI_ASSERT_TYPE_TAG_int         int,
#define CRI_ASSERT_TYPE_TAG_ID_int      int,

#define CRI_ASSERT_TEST_TAG_uint        ,
#define CRI_ASSERT_TEST_TAGC_uint()     ,
#define CRI_ASSERT_TYPE_TAG_uint        unsigned int,
#define CRI_ASSERT_TYPE_TAG_ID_uint     uint,

#define CRI_ASSERT_TEST_TAG_long        ,
#define CRI_ASSERT_TEST_TAGC_long()     ,
#define CRI_ASSERT_TYPE_TAG_long        long,
#define CRI_ASSERT_TYPE_TAG_ID_long     long,

#define CRI_ASSERT_TEST_TAG_ulong       ,
#define CRI_ASSERT_TEST_TAGC_ulong()    ,
#define CRI_ASSERT_TYPE_TAG_ulong       unsigned long,
#define CRI_ASSERT_TYPE_TAG_ID_ulong    ulong,

#define CRI_ASSERT_TEST_TAG_llong       ,
#define CRI_ASSERT_TEST_TAGC_llong()    ,
#define CRI_ASSERT_TYPE_TAG_llong       long long,
#define CRI_ASSERT_TYPE_TAG_ID_llong    llong,

#define CRI_ASSERT_TEST_TAG_ullong      ,
#define CRI_ASSERT_TEST_TAGC_ullong()   ,
#define CRI_ASSERT_TYPE_TAG_ullong      unsigned long long,
#define CRI_ASSERT_TYPE_TAG_ID_ullong   ullong,

#define CRI_ASSERT_TEST_TAG_ptr         ,
#define CRI_ASSERT_TEST_TAGC_ptr()      ,
#define CRI_ASSERT_TYPE_TAG_ptr         void *,
#define CRI_ASSERT_TYPE_TAG_ID_ptr      ptr,

#define CRI_ASSERT_TEST_TAG_str         ,
#define CRI_ASSERT_TEST_TAGC_str()      ,
#ifdef __cplusplus
# define CRI_ASSERT_TYPE_TAG_str        std::string,
#else
# define CRI_ASSERT_TYPE_TAG_str        char *,
#endif
#define CRI_ASSERT_TYPE_TAG_ID_str      str,

#define CRI_ASSERT_TEST_TAG_wcs         ,
#define CRI_ASSERT_TEST_TAGC_wcs()      ,
#ifdef __cplusplus
# define CRI_ASSERT_TYPE_TAG_wcs        std::wstring,
#else
# define CRI_ASSERT_TYPE_TAG_wcs        wchar_t *,
#endif
#define CRI_ASSERT_TYPE_TAG_ID_wcs      wcs,

#define CRI_ASSERT_TEST_TAG_tcs         ,
#define CRI_ASSERT_TEST_TAGC_tcs()      ,
#define CRI_ASSERT_TYPE_TAG_tcs         _TCHAR *,
#define CRI_ASSERT_TYPE_TAG_ID_tcs      tcs,

#define CRI_ASSERT_TEST_TAG_flt         ,
#define CRI_ASSERT_TEST_TAGC_flt()      ,
#define CRI_ASSERT_TYPE_TAG_flt         float,
#define CRI_ASSERT_TYPE_TAG_ID_flt      flt,

#define CRI_ASSERT_TEST_TAG_dbl         ,
#define CRI_ASSERT_TEST_TAGC_dbl()      ,
#define CRI_ASSERT_TYPE_TAG_dbl         double,
#define CRI_ASSERT_TYPE_TAG_ID_dbl      dbl,

#define CRI_ASSERT_TEST_TAG_ldbl        ,
#define CRI_ASSERT_TEST_TAGC_ldbl()     ,
#if defined (CRI_CAPS_LDBL_IS_DBL)
# define CRI_ASSERT_TYPE_TAG_ldbl       double,
#else
# define CRI_ASSERT_TYPE_TAG_ldbl       long double,
#endif
#define CRI_ASSERT_TYPE_TAG_ID_ldbl     ldbl,

#define CRI_ASSERT_TEST_TAG_sz          ,
#define CRI_ASSERT_TEST_TAGC_sz()       ,
#define CRI_ASSERT_TYPE_TAG_sz          size_t,
#define CRI_ASSERT_TYPE_TAG_ID_sz       sz,

#define CRI_ASSERT_TEST_TAG_iptr        ,
#define CRI_ASSERT_TEST_TAGC_iptr()     ,
#define CRI_ASSERT_TYPE_TAG_iptr        intptr_t,
#define CRI_ASSERT_TYPE_TAG_ID_iptr     iptr,

#define CRI_ASSERT_TEST_TAG_uptr        ,
#define CRI_ASSERT_TEST_TAGC_uptr()     ,
#define CRI_ASSERT_TYPE_TAG_uptr        uintptr_t,
#define CRI_ASSERT_TYPE_TAG_ID_uptr     uptr,

#define CRI_ASSERT_TEST_TAG_type        ,
#define CRI_ASSERT_TEST_TAGC_type(x)    CRI_ASSERT_TEST_TAGC_type_
#define CRI_ASSERT_TEST_TAGC_type_()    ,
#define CRI_ASSERT_TYPE_TAG_type(T)     T,
#define CRI_ASSERT_TYPE_TAG_ID_type(T)  CRI_ASSERT_SWALLOW_KEYWORD(T),

#define CRI_ASSERT_DECLARE_NATIVE_CMP_FN(Tag)     \
    static inline int CRI_USER_TAG_ID(lt, Tag)(   \
        CRI_ASSERT_TYPE_TAG(Tag) *actual,         \
        CRI_ASSERT_TYPE_TAG(Tag) *expected)       \
    {                                             \
        return *actual < *expected;               \
    }                                             \
    static inline int CRI_USER_TAG_ID(eq, Tag)(   \
        CRI_ASSERT_TYPE_TAG(Tag) *actual,         \
        CRI_ASSERT_TYPE_TAG(Tag) *expected)       \
    {                                             \
        return *actual == *expected;              \
    }                                             \
    static inline int CRI_USER_TAG_ID(zero, Tag)( \
        CRI_ASSERT_TYPE_TAG(Tag) *val)            \
    {                                             \
        return !*val;                             \
    }

#define CRI_ASSERT_DECLARE_NATIVE_FN(Tag, Fmt)       \
    CRI_ASSERT_DECLARE_NATIVE_CMP_FN(Tag)            \
    static inline char *CRI_USER_TAG_ID(tostr, Tag)( \
        CRI_ASSERT_TYPE_TAG(Tag) *e)                 \
    {                                                \
        char *str = NULL;                            \
        cr_asprintf(&str, "%" Fmt, *e);              \
        return str;                                  \
    }

#define CRI_ASSERT_DECLARE_STR_FN(Tag, Prefix, Fmt)                         \
    CR_API int CRI_USER_TAG_ID(lt, Tag)(                                    \
        CRI_ASSERT_TYPE_TAG(Tag) *actual,                                   \
        CRI_ASSERT_TYPE_TAG(Tag) *expected);                                \
    CR_API int CRI_USER_TAG_ID(eq, Tag)(                                    \
        CRI_ASSERT_TYPE_TAG(Tag) *actual,                                   \
        CRI_ASSERT_TYPE_TAG(Tag) *expected);                                \
    CR_API int CRI_USER_TAG_ID(zero, Tag)(CRI_ASSERT_TYPE_TAG(Tag) *val);   \
    CR_API char *CRI_USER_TAG_ID(tostr, Tag)(CRI_ASSERT_TYPE_TAG(Tag) *e);  \

#ifndef __cplusplus
# include <string.h>
# include <wchar.h>
# include <float.h>

CRI_ASSERT_DECLARE_NATIVE_FN(chr, "c")
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
CRI_ASSERT_DECLARE_STR_FN(str, "", "s")
CRI_ASSERT_DECLARE_STR_FN(wcs, "L", "ls")

CRI_ASSERT_DECLARE_NATIVE_CMP_FN(iptr)
CRI_ASSERT_DECLARE_NATIVE_CMP_FN(uptr)
CRI_ASSERT_DECLARE_NATIVE_CMP_FN(ptr)

static inline char *CRI_USER_TAG_ID(tostr, iptr)(intptr_t *e)
{
    uintptr_t absptr = *e;
    if (absptr > (uintptr_t)INTPTR_MAX)
        absptr = -absptr;

    char *str = NULL;
    cr_asprintf(&str, "%s0x%" PRIxPTR, *e < 0 ? "-" : "", absptr);
    return str;
}

static inline char *CRI_USER_TAG_ID(tostr, uptr)(uintptr_t *e)
{
    char *str = NULL;
    cr_asprintf(&str, "0x%" PRIxPTR, *e);
    return str;
}

static inline char *CRI_USER_TAG_ID(tostr, ptr)(void **e)
{
    char *str = NULL;
    cr_asprintf(&str, "0x%" PRIxPTR, (uintptr_t) *e);
    return str;
}

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

#if defined (CRI_CAPS_LDBL_IS_DBL)
# define CRI_LDBL_DIG CRI_DBL_DIG
#elif defined (LDBL_DECIMAL_DIG)
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

#if defined (CRI_CAPS_LDBL_IS_DBL)
CRI_ASSERT_DECLARE_NATIVE_FN(ldbl, "." CRI_DBL_DIG "g")
#else
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

#include "memory.h"

#ifdef CRI_CAPS_COMPLEX
# include "complex.h"
#endif

#endif /* !CRITERION_INTERNAL_ASSERT_TAG_H_ */
