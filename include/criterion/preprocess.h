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
#ifndef CRITERION_PREPROCESS_H_
# define CRITERION_PREPROCESS_H_

# define CR_NOOP do {} while(0)

# ifdef __cplusplus
#  define CR_NOTHROW throw()
# else
#  define CR_NOTHROW
# endif

# define CR_EXPAND(x) x
# define CR_IDENTITY(...) __VA_ARGS__

# define CR_STR(x) CR_EXPAND(CR_STR_(x))
# define CR_STR_(x) #x

# define CR_VA_TAIL(...) CR_EXPAND(CR_VA_TAIL_HELPER(CR_VA_TAIL_SELECT(__VA_ARGS__), __VA_ARGS__))

# define CR_VA_TAIL_HELPER(N, ...)  CR_EXPAND(CR_VA_TAIL_HELPER_(N, __VA_ARGS__))
# define CR_VA_TAIL_HELPER_(N, ...) CR_EXPAND(CR_VA_TAIL_HELPER_##N(__VA_ARGS__))
# define CR_VA_TAIL_HELPER_1(Head)
# define CR_VA_TAIL_HELPER_2(Head, ...) __VA_ARGS__

# define CR_VA_HEAD(...) CR_EXPAND(CR_VA_HEAD_HELPER(CR_VA_TAIL_SELECT(__VA_ARGS__), __VA_ARGS__))

# define CR_VA_HEAD_HELPER(N, ...)  CR_EXPAND(CR_VA_HEAD_HELPER_(N, __VA_ARGS__))
# define CR_VA_HEAD_HELPER_(N, ...) CR_EXPAND(CR_VA_HEAD_HELPER_##N(__VA_ARGS__))
# define CR_VA_HEAD_HELPER_1(Head) Head
# define CR_VA_HEAD_HELPER_2(Head, ...) Head

# define CR_VA_TAIL_SELECT(...) CR_EXPAND(CR_VA_TAIL_SELECT64(__VA_ARGS__, \
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, \
        2, 2, 1, _))

# define CR_VA_TAIL_SELECT64( \
        _01, _02, _03, _04, _05, _06, _07, _08, _09, _10, \
        _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
        _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
        _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
        _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
        _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
        _61, _62, _63, X, ...) X

#endif /* !CRITERION_PREPROCESS_H_ */
