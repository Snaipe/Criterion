/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_PREPROCESS_H_
#define CRITERION_PREPROCESS_H_

#define CR_NOOP    do {} while (0)

#ifdef __cplusplus
# define CR_NOTHROW    throw ()
#else
# define CR_NOTHROW
#endif

#define CR_EXPAND(x)                      x
#define CR_IDENTITY(...)                  __VA_ARGS__
#define CR_EMPTY()
#define CR_DEFER(x)                       x CR_EMPTY()
#define CR_OBSTRUCT(...)                  __VA_ARGS__ CR_DEFER(CR_EMPTY)()

#define CR_EAT(...)                       __VA_ARGS__

#define CRI_DEFER1(x)                     CR_DEFER(x)
#define CRI_DEFER2(x)                     CR_DEFER(CRI_DEFER1)(x)
#define CRI_DEFER3(x)                     CR_DEFER(CRI_DEFER2)(x)
#define CRI_DEFER4(x)                     CR_DEFER(CRI_DEFER3)(x)

#define CRI_OBSTRUCT1(...)                __VA_ARGS__ CRI_DEFER4(CR_EMPTY)()
#define CRI_OBSTRUCT2(...)                __VA_ARGS__ CRI_DEFER4(CRI_OBSTRUCT1)()
#define CRI_OBSTRUCT3(...)                __VA_ARGS__ CRI_DEFER4(CRI_OBSTRUCT2)()
#define CRI_OBSTRUCT_N(...)               __VA_ARGS__ CRI_DEFER4(CRI_OBSTRUCT3)()

#define CR_EVAL(...)                      CR_EVAL1(CR_EVAL1(CR_EVAL1(CR_EVAL1(__VA_ARGS__))))
#define CR_EVAL1(...)                     CR_EVAL2(CR_EVAL2(CR_EVAL2(CR_EVAL2(__VA_ARGS__))))
#define CR_EVAL2(...)                     CR_EVAL3(CR_EVAL3(CR_EVAL3(CR_EVAL3(__VA_ARGS__))))
#define CR_EVAL3(...)                     CR_IDENTITY(__VA_ARGS__)

#define CR_STR(...)                       CR_EXPAND(CR_STR_(__VA_ARGS__))
#define CR_STR_(...)                      #__VA_ARGS__

#define CR_VA_TAIL(...)                   CR_EXPAND(CR_VA_TAIL_HELPER(CR_VA_TAIL_SELECT(__VA_ARGS__), __VA_ARGS__))

#define CR_VA_TAIL_HELPER(N, ...)         CR_EXPAND(CR_VA_TAIL_HELPER_(N, __VA_ARGS__))
#define CR_VA_TAIL_HELPER_(N, ...)        CR_EXPAND(CR_VA_TAIL_HELPER_ ## N(__VA_ARGS__))
#define CR_VA_TAIL_HELPER_1(Head)
#define CR_VA_TAIL_HELPER_2(Head, ...)    __VA_ARGS__

#define CR_VA_HEAD(...)                   CR_EXPAND(CR_VA_HEAD_HELPER(CR_VA_TAIL_SELECT(__VA_ARGS__), __VA_ARGS__))

#define CR_VA_HEAD_HELPER(N, ...)         CR_EXPAND(CR_VA_HEAD_HELPER_(N, __VA_ARGS__))
#define CR_VA_HEAD_HELPER_(N, ...)        CR_EXPAND(CR_VA_HEAD_HELPER_ ## N(__VA_ARGS__))
#define CR_VA_HEAD_HELPER_1(Head)         Head
#define CR_VA_HEAD_HELPER_2(Head, ...)    Head

#define CR_VA_TAIL_SELECT(...)                 \
    CR_EXPAND(CR_VA_TAIL_SELECT64(__VA_ARGS__, \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 2, 2, 2, 2, 2, 2, 2, 2,      \
            2, 2, 1, _))

#define CR_VA_TAIL_SELECT64(                              \
        _01, _02, _03, _04, _05, _06, _07, _08, _09, _10, \
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
        _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
        _61, _62, _63, X, ...)                                           X

#define CR_CONCAT_E(A, B)                                                CR_CONCAT(CR_EXPAND(A), CR_EXPAND(B))
#define CR_CONCAT(A, B)                                                  CR_CONCAT2(A, B)
#define CR_CONCAT2(A, B)                                                 CR_CONCAT3(A, B)
#define CR_CONCAT3(A, B)                                                 A ## B

#define CRI_IF_DEFINED__(M, N)                                           M ## N
#define CRI_IF_DEFINED_(M, N)                                            CRI_IF_DEFINED__(M, N)
#define CRI_IF_DEFINED(M, Then, ThenParams, Else, ElseParams)            CRI_IF_DEFINED_(CRI_IF_DEFINED_, CR_VA_TAIL_SELECT(M))(Then, ThenParams, Else, ElseParams)
#define CRI_IF_DEFINED_1(Then, ThenParams, Else, ElseParams)             CRI_DEFER2(Else) ElseParams
#define CRI_IF_DEFINED_2(Then, ThenParams, Else, ElseParams)             CRI_DEFER2(Then) ThenParams

#define CRI_IF_DEFINED_NODEFER__(M, N)                                   M ## N
#define CRI_IF_DEFINED_NODEFER_(M, N)                                    CRI_IF_DEFINED_NODEFER__(M, N)
#define CRI_IF_DEFINED_NODEFER(M, Then, ThenParams, Else, ElseParams)    CRI_IF_DEFINED_NODEFER_(CRI_IF_DEFINED_NODEFER_, CR_VA_TAIL_SELECT(M))(Then, ThenParams, Else, ElseParams)
#define CRI_IF_DEFINED_NODEFER_1(Then, ThenParams, Else, ElseParams)     Else ElseParams
#define CRI_IF_DEFINED_NODEFER_2(Then, ThenParams, Else, ElseParams)     Then ThenParams

#endif /* !CRITERION_PREPROCESS_H_ */
