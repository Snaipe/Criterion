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
#ifndef CRITERION_FLOAT_H_
# define CRITERION_FLOAT_H_

# include <inttypes.h>
# include "common.h"

// This is part of the ABI, do **not** reorder or remove entries.
// Add new entries at the end, but before CR_FP_CMP_NB_STRATEGIES_.
enum criterion_fpcmp_strategy {
    CR_FP_CMP_ABSOLUTE_EPSILON,
    CR_FP_CMP_RELATIVE_EPSILON,
    CR_FP_CMP_ULPS,
    CR_FP_CMP_ADAPTIVE,

    CR_FP_CMP_NB_STRATEGIES_, // sentinel
};

union criterion_fp_single {
    float f;
    int32_t i;
};

union criterion_fp_double {
    double f;
    int64_t i;
};

struct criterion_fp_any {
    unsigned long datasize;
    union inner {
        union criterion_fp_single sp;
        union criterion_fp_double dp;

#ifdef __cplusplus
        inner(float val) : sp {val} {}
        inner(double val) : dp {val} {}
#endif
    } val;

#ifdef __cplusplus
    criterion_fp_any(float val) : datasize(sizeof (float)), val(val) {}
    criterion_fp_any(double val) : datasize(sizeof (double)), val(val) {}
#endif
};

CR_BEGIN_C_API

typedef int (*cr_flt_cmp)(register union criterion_fp_single,
                          register union criterion_fp_single);
typedef int (*cr_dbl_cmp)(register union criterion_fp_double,
                          register union criterion_fp_double);

extern cr_flt_cmp criterion_flt_strategies[CR_FP_CMP_NB_STRATEGIES_];
extern cr_dbl_cmp criterion_dbl_strategies[CR_FP_CMP_NB_STRATEGIES_];

CR_API int cr_float_cmp(struct criterion_fp_any *a, struct criterion_fp_any *b);

CR_END_C_API

#endif /* !CRITERION_FLOAT_H_ */
