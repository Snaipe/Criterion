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
#define CRI_ASSERT_TYPE_TAG_(Tag)          CRI_IF_DEFINED(CRI_ASSERT_TEST_TAG_ ## Tag, CR_VA_HEAD(CRI_ASSERT_TYPE_TAG_ ## Tag), , CRI_ASSERT_TYPE_TAG_UNKNOWN, (Tag))

#define CRI_USER_TAG_ID(Id, Tag)           CR_CONCAT(cr_user_ ## Id ## _, CRI_ASSERT_TYPE_TAG_ID(Tag))

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

#if defined (CRI_CAPS_LDBL)
# define CRI_ASSERT_TEST_TAG_ldbl       ,
# define CRI_ASSERT_TEST_TAGC_ldbl()    ,
# define CRI_ASSERT_TYPE_TAG_ldbl       long double,
# define CRI_ASSERT_TYPE_TAG_ID_ldbl    ldbl,
#endif

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

#endif /* !CRITERION_INTERNAL_ASSERT_TAG_H_ */
