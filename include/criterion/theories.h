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
#ifndef CRITERION_THEORIES_H_
# define CRITERION_THEORIES_H_

# include <stddef.h>
# include "criterion.h"

struct criterion_theory_context;

struct criterion_theory_context* cr_theory_init(void);
void cr_theory_push_arg(struct criterion_theory_context *ctx, bool is_float, size_t size, void *ptr);
void cr_theory_free(struct criterion_theory_context *ctx);
void cr_theory_abort(void);
int cr_theory_mark(void);

void cr_theory_reset(struct criterion_theory_context *ctx);
void cr_theory_call(struct criterion_theory_context *ctx, void (*fnptr)(void));

# define TheoryDataPoints(Category, Name) \
    static struct criterion_datapoints IDENTIFIER_(Category, Name, dps)[]

# define DataPoints(Type, ...) \
    { sizeof (Type), sizeof ((Type[]) { __VA_ARGS__ }) / sizeof (Type), #Type, &(Type[]) { __VA_ARGS__ } }

struct criterion_datapoints {
    size_t size;
    size_t len;
    const char *name;
    void *arr;
};

# define CR_NB_DATAPOINTS(Var) \
    (sizeof (Var) / sizeof (struct criterion_datapoints))

# define cr_assume(Condition) \
    do { \
        if (!(Condition)) \
            cr_theory_abort(); \
    } while (0);

void cr_theory_main(struct criterion_datapoints *dps, size_t datapoints, void (*fnptr)(void));

# define CR_VAARG_ID(Suffix, Category, Name, ...) \
    IDENTIFIER_(Category, Name, Suffix)

# define Theory(Args, ...)                                          \
    void CR_VAARG_ID(theory, __VA_ARGS__,)Args;                     \
    Test_(__VA_ARGS__, .sentinel_ = 0) {                            \
        cr_theory_main(                                             \
                CR_VAARG_ID(dps, __VA_ARGS__,),                     \
                CR_NB_DATAPOINTS(CR_VAARG_ID(dps, __VA_ARGS__,)),   \
                (void(*)(void)) CR_VAARG_ID(theory, __VA_ARGS__,)   \
            );                                                      \
    }                                                               \
    void CR_VAARG_ID(theory, __VA_ARGS__,)Args

#endif /* !CRITERION_THEORIES_H_ */
