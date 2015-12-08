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
#ifndef CRITERION_INTERNAL_THEORIES_H_
# define CRITERION_INTERNAL_THEORIES_H_

# include "test.h"

# ifdef __cplusplus
#  include <cstddef>
using std::size_t;
# else
#  include <stddef.h>
# endif

# ifdef __cplusplus
template <typename... T>
constexpr size_t criterion_va_num__(const T &...) {
    return sizeof...(T);
}
# endif

struct criterion_datapoints {
    size_t size;
    size_t len;
    const char *name;
    void *arr;
};

CR_BEGIN_C_API

CR_API void cr_theory_main(struct criterion_datapoints *dps, size_t datapoints, void (*fnptr)(void));

CR_END_C_API

# ifdef __cplusplus
#  define CR_TH_VA_NUM(Type, ...)     criterion_va_num__(__VA_ARGS__)
#  define CR_TH_TEMP_ARRAY(Type, ...) []() -> Type* { static Type arr[] = { __VA_ARGS__ }; return reinterpret_cast<Type*>(&arr); }()
# else
#  define CR_TH_VA_NUM(Type, ...) sizeof ((Type[]) { __VA_ARGS__ }) / sizeof (Type)
#  define CR_TH_TEMP_ARRAY(Type, ...) &(Type[]) { __VA_ARGS__ }
# endif

# define CR_TH_INTERNAL_TDPS(Category, Name) \
    static struct criterion_datapoints CR_IDENTIFIER_(Category, Name, dps)[]

# define CR_TH_INTERNAL_TDP(Category, Name) \
    (CR_IDENTIFIER_(Category, Name, dps))

# define CR_TH_INTERNAL_DP(Type, ...) { \
        sizeof (Type), \
        CR_EXPAND(CR_TH_VA_NUM(Type, __VA_ARGS__)), \
        #Type, \
        CR_EXPAND(CR_TH_TEMP_ARRAY(Type, __VA_ARGS__)), \
    }

# define CR_NB_DATAPOINTS(Var) \
    (sizeof (Var) / sizeof (struct criterion_datapoints))

# define CR_VAARG_ID(Suffix, Category, Name, ...) \
    CR_IDENTIFIER_(Category, Name, Suffix)

# define CR_THEORY_BASE(Args, ...)                                             \
    void CR_EXPAND(CR_VAARG_ID(theory, __VA_ARGS__,))Args;                     \
    CR_EXPAND(CR_TEST_BASE(__VA_ARGS__, .sentinel_ = 0)) {                     \
        cr_theory_main(                                                        \
                CR_EXPAND(CR_VAARG_ID(dps, __VA_ARGS__,)),                     \
                CR_NB_DATAPOINTS(CR_EXPAND(CR_VAARG_ID(dps, __VA_ARGS__,))),   \
                (void(*)(void)) CR_EXPAND(CR_VAARG_ID(theory, __VA_ARGS__,))   \
            );                                                                 \
    }                                                                          \
    void CR_EXPAND(CR_VAARG_ID(theory, __VA_ARGS__,))Args

#endif /* !CRITERION_INTERNAL_THEORIES_H_ */
