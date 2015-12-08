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
#ifndef CRITERION_HOOKS_H_
# define CRITERION_HOOKS_H_

# include "internal/hooks.h"

/**
 *  This enum lists all the phases of the runner lifecycle.
 */
typedef enum {
    PRE_ALL,
    PRE_SUITE,
    PRE_INIT,
    PRE_TEST,
    ASSERT,
    THEORY_FAIL,
    TEST_CRASH,
    POST_TEST,
    POST_FINI,
    POST_SUITE,
    POST_ALL,
} e_report_status;

typedef void (*f_report_hook)();

/**
 *  ReportHook(Kind)(Type *param) { Function Body }
 *
 *  Defines a report hook for the phase defined by Kind.
 *
 *  The type of the parameter depends on the phase:
 *
 *  - struct criterion_test_set for PRE_ALL.
 *  - struct criterion_suite_set for PRE_SUITE.
 *  - struct criterion_test for PRE_INIT and PRE_TEST.
 *  - struct criterion_assert_stats for ASSERT.
 *  - struct criterion_theory_stats for THEORY_FAIL.
 *  - struct criterion_test_stats for POST_TEST, POST_FINI, and TEST_CRASH.
 *  - struct criterion_suite_stats for POST_SUITE.
 *  - struct criterion_global_stats for POST_ALL.
 *
 *  @param Kind The report phase to hook the function onto.
 */
# define ReportHook(Kind) CR_REPORT_HOOK_IMPL(Kind)

#endif /* !CRITERION_HOOKS_H_ */
