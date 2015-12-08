/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_DESIGNATED_INITIALIZER_COMPAT_HH_
# define CRITERION_DESIGNATED_INITIALIZER_COMPAT_HH_

# include "common.h"

# define CRITERION_ARG_LENGTH(...) CR_EXPAND(CRITERION_ARG_LENGTH_(__VA_ARGS__,\
    63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,\
    44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26,\
    25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6,\
    5, 4, 3, 2, 1, 0))
# define CRITERION_ARG_LENGTH_(_63, _62, _61, _60, _59, _58, _57, _56, _55, _54, _53, \
    _52, _51, _50, _49, _48, _47, _46, _45, _44, _43, _42, _41, _40, _39, _38, \
    _37, _36, _35, _34, _33, _32, _31, _30, _29, _28, _27, _26, _25, _24, _23, \
    _22, _21, _20, _19, _18, _17, _16, _15, _14, _13, _12, _11, _10, _9, _8,   \
    _7, _6, _5, _4, _3, _2, _1, count, ...) count

# define CRITERION_APPLY_1(Macro, ...)
# define CRITERION_APPLY_2(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_1(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_3(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_2(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_4(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_3(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_5(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_4(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_6(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_5(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_7(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_6(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_8(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_7(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_9(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_8(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_10(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_9(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_11(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_10(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_12(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_11(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_13(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_12(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_14(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_13(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_15(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_14(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_16(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_15(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_17(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_16(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_18(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_17(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_19(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_18(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_20(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_19(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_21(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_20(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_22(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_21(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_23(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_22(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_24(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_23(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_25(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_24(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_26(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_25(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_27(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_26(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_28(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_27(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_29(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_28(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_30(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_29(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_31(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_30(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_32(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_31(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_33(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_32(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_34(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_33(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_35(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_34(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_36(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_35(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_37(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_36(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_38(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_37(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_39(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_38(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_40(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_39(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_41(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_40(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_42(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_41(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_43(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_42(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_44(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_43(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_45(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_44(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_46(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_45(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_47(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_46(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_48(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_47(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_49(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_48(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_50(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_49(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_51(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_50(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_52(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_51(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_53(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_52(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_54(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_53(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_55(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_54(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_56(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_55(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_57(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_56(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_58(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_57(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_59(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_58(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_60(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_59(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_61(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_60(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_62(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_61(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_63(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_62(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_64(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_63(Macro, Prefix, __VA_ARGS__))
# define CRITERION_APPLY_65(Macro, Prefix, Head, ...) Macro(Prefix, Head) CR_EXPAND(CRITERION_APPLY_64(Macro, Prefix, __VA_ARGS__))

# define CRITERION_APPLY__(Macro, Prefix, n, ...) CR_EXPAND(CRITERION_APPLY_##n(Macro, Prefix, __VA_ARGS__,))
# define CRITERION_APPLY_(Macro, n, Prefix, ...) CR_EXPAND(CRITERION_APPLY__(Macro, Prefix, n, __VA_ARGS__))
# define CRITERION_APPLY(Macro, ...) CR_EXPAND(CRITERION_APPLY_(Macro, CRITERION_ARG_LENGTH(__VA_ARGS__), __VA_ARGS__))

# define CRITERION_ADD_PREFIX_ONCE(Prefix, Field) Prefix Field;
# define CRITERION_ADD_PREFIX(...) \
    CR_EXPAND(CRITERION_APPLY(CRITERION_ADD_PREFIX_ONCE, __VA_ARGS__))

# ifdef __cplusplus
#  define CRITERION_MAKE_STRUCT(Type, ...) []() -> Type {   \
        Type t;                                             \
        std::memset(&t, 0, sizeof (t));                     \
        CR_EXPAND(CRITERION_ADD_PREFIX(t, __VA_ARGS__))     \
        return t;                                           \
    }()
# else
#  define CRITERION_MAKE_STRUCT(Type, ...) { __VA_ARGS__ }
# endif

#endif /* !CRITERION_DESIGNATED_INITIALIZER_COMPAT_HH_ */
