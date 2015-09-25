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
#include "config.h"

#ifndef HAVE_IEEE754_FLOATS
# error IEEE 754 floats are not supported on this platform.
#endif

#define _GNU_SOURCE 1
#include <assert.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include "criterion/float.h"
#include "criterion/options.h"
#include "common.h"

static const union criterion_fp_single single_min = { .f = FLT_MIN };
static const union criterion_fp_single single_neg_min = { .f = -FLT_MIN };

static INLINE int32_t abs32(int32_t x) {
    return x < 0 ? -x : x;
}

static INLINE int64_t abs64(int64_t x) {
    return x < 0 ? -x : x;
}

#ifdef __GNUC__
# define likely(x)   __builtin_expect((x),1)
# define unlikely(x) __builtin_expect((x),0)
#else
# define likely(x) x
# define unlikely(x) x
#endif

#define fp_abs(Order) fp_abs_ ## Order
#define fp_abs_single abs32
#define fp_abs_double abs64

#define fp_type(Order) fp_type_ ## Order
#define fp_type_single float
#define fp_type_double double

#define fp_fabs(Order) fp_fabs_ ## Order
#define fp_fabs_single fabsf
#define fp_fabs_double fabs

#define fpcmp_opt(Field) criterion_options.floating_point_compare. Field

#define fp_range(Order) fpcmp_opt(fp_range_ ## Order)
#define fp_range_single flt_range
#define fp_range_double dbl_range

#define fp_epsilon(Order) fpcmp_opt(fp_epsilon_ ## Order)
#define fp_epsilon_single flt_epsilon
#define fp_epsilon_double dbl_epsilon

typedef int (*flt_cmp)(register union criterion_fp_single, register union criterion_fp_single);
typedef int (*dbl_cmp)(register union criterion_fp_double, register union criterion_fp_double);

#define IMPL_CMP_ABSEPS(Order)                                                          \
    int ieee754_ ## Order ## _cmp_abseps(register union criterion_fp_ ## Order val1,    \
                                         register union criterion_fp_ ## Order val2) {  \
                                                                                        \
        fp_type(Order) epsilon = fp_range(Order) * fp_epsilon(Order);                   \
        fp_type(Order) diff = val1.f - val2.f;                                          \
        return fp_fabs(Order)(diff) < epsilon ? 0 : (diff > 0 ? 1 : -1);                \
    }

#define IMPL_CMP_RELEPS(Order)                                                          \
    int ieee754_ ## Order ## _cmp_releps(register union criterion_fp_ ## Order val1,    \
                                         register union criterion_fp_ ## Order val2) {  \
                                                                                        \
        fp_type(Order) epsilon = fp_range(Order) * fp_epsilon(Order);                   \
        fp_type(Order) diff = val1.f - val2.f;                                          \
        fp_type(Order) absdiff = fp_fabs(Order)(diff);                                  \
                                                                                        \
        val1.f = fp_fabs(Order)(val1.f);                                                \
        val2.f = fp_fabs(Order)(val2.f);                                                \
        fp_type(Order) largest = val1.f > val2.f ? val1.f : val2.f;                     \
                                                                                        \
        return absdiff <= largest * epsilon ? 0 : (diff > 0 ? 1 : -1);                  \
    }

#define IMPL_CMP_ULP(Order)                                                             \
    int ieee754_ ## Order ## _cmp_ulp(register union criterion_fp_ ## Order val1,       \
                                      register union criterion_fp_ ## Order val2) {     \
                                                                                        \
        size_t max_ulps = fp_range(Order);                                              \
        const int p1 = fpclassify(val1.f), p2 = fpclassify(val2.f);                     \
                                                                                        \
        if (unlikely((val1.i == val2.i || (p1 == FP_ZERO && p2 == FP_ZERO))             \
                && p1 != FP_NAN && p2 != FP_NAN))                                       \
            return 0;                                                                   \
                                                                                        \
        if (unlikely(p1 == FP_INFINITE || p2 == FP_INFINITE))                           \
            return val1.f > val2.f;                                                     \
                                                                                        \
        const int sign1 = signbit(val1.f), sign2 = signbit(val2.f);                     \
                                                                                        \
        if (sign1 != sign2)                                                             \
            return val1.i == val2.i ? 0 : (val1.f > val2.f ? 1 : -1);                   \
                                                                                        \
        int32_t ulpdiff = val1.i - val2.i;                                              \
        return fp_abs(Order)(ulpdiff) < (int32_t) max_ulps ? 0                          \
                : (ulpdiff > 0 ? 1 : -1);                                               \
    }

IMPL_CMP_ABSEPS(single)
IMPL_CMP_ABSEPS(double)
IMPL_CMP_RELEPS(single)
IMPL_CMP_RELEPS(double)
IMPL_CMP_ULP(single)
IMPL_CMP_ULP(double)

flt_cmp criterion_flt_strategies[] = {
    [CR_FP_CMP_RELATIVE_EPSILON]    = ieee754_single_cmp_releps,
    [CR_FP_CMP_ABSOLUTE_EPSILON]    = ieee754_single_cmp_abseps,
    [CR_FP_CMP_ULPS]                = ieee754_single_cmp_ulp,
};

dbl_cmp criterion_dbl_strategies[] = {
    [CR_FP_CMP_RELATIVE_EPSILON]    = ieee754_double_cmp_releps,
    [CR_FP_CMP_ABSOLUTE_EPSILON]    = ieee754_double_cmp_abseps,
    [CR_FP_CMP_ULPS]                = ieee754_double_cmp_ulp,
};

int cr_float_cmp(struct criterion_fp_any *a, struct criterion_fp_any *b) {
    assert(a->datasize == b->datasize);

    if (a->datasize == sizeof (float)) {
        criterion_flt_strategies[fpcmp_opt(strategy)](a->val.sp, b->val.sp);
    } else if (a->datasize == sizeof (double)) {
        criterion_dbl_strategies[fpcmp_opt(strategy)](a->val.dp, b->val.dp);
    }
    abort();
}
