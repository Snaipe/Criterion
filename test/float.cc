#include "criterion/parameterized.h"
#include "criterion/options.h"
#include "criterion/theories.h"
#include "criterion/float.h"
#include <cmath>
#include <cfloat>
#include <iostream>
#include <unistd.h>

struct float_compare_params {
    float arg1;
    float arg2;
};

ParameterizedTestParameters(float, eq) {
    static float_compare_params params[] = {
        { 0.f,        0.f        },
        { 1.0f/0.0f,  1.0f/0.0f  },
        { -1.0f/0.0f, -1.0f/0.0f },
    };

    return criterion_test_params(params);
}

ParameterizedTest(float_compare_params *p, float, eq) {
    criterion_fp_any v1(p->arg1);
    criterion_fp_any v2(p->arg2);

    cr_assert_eq(cr_float_cmp(&v1, &v2), 0);
}

# define FLOAT_DP DataPoints(float,                                           \
    0.f, FLT_EPSILON, FLT_EPSILON * 4.f, FLT_EPSILON / 2.f, 1.0f / 0.0f,      \
    -0.f, -FLT_EPSILON, -FLT_EPSILON * 4.f, -FLT_EPSILON / 2.f, -1.0f / 0.0f  \
    )

TheoryDataPoints(float, cmp) {
    DataPoints(enum criterion_fpcmp_strategy,
            CR_FP_CMP_RELATIVE_EPSILON,
            CR_FP_CMP_ABSOLUTE_EPSILON,
            CR_FP_CMP_ULPS),
    FLOAT_DP,
    FLOAT_DP,
};

Theory((enum criterion_fpcmp_strategy strategy, float arg1, float arg2), float, cmp) {
    cr_assume(arg1 != arg2);

    criterion_options.floating_point_compare.strategy = strategy;
    criterion_options.floating_point_compare.flt_range = 0; // never evaluate for equality

    switch (strategy) {
        case CR_FP_CMP_ABSOLUTE_EPSILON:
            cr_assume_neq(std::fpclassify(arg1), FP_SUBNORMAL);
            cr_assume_neq(std::fpclassify(arg2), FP_SUBNORMAL);
            break;
        case CR_FP_CMP_RELATIVE_EPSILON:
        case CR_FP_CMP_ULPS:
            cr_assume_neq(std::fpclassify(arg1), FP_ZERO);
            cr_assume_neq(std::fpclassify(arg2), FP_ZERO);
            break;
        default: break;
    }

    criterion_fp_any v1(arg1);
    criterion_fp_any v2(arg2);

    cr_assert_eq(cr_float_cmp(&v1, &v2), arg1 < arg2 ? -1 : 1);
}
