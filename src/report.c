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
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "criterion/types.h"
#include "criterion/stats.h"
#include "criterion/logging.h"
#include "criterion/options.h"
#include "criterion/ordered-set.h"
#include "report.h"
#include "config.h"
#include "posix-compat.h"

static inline void nothing() {}

#define IMPL_CALL_REPORT_HOOKS(Kind)                                        \
    IMPL_SECTION_LIMITS(f_report_hook, HOOK_SECTION(Kind));                 \
    void call_report_hooks_##Kind(void *data) {                             \
        for (f_report_hook *hook = GET_SECTION_START(HOOK_SECTION(Kind));   \
             hook < (f_report_hook*) GET_SECTION_END(HOOK_SECTION(Kind));   \
             ++hook) {                                                      \
            (*hook ? *hook : nothing)(data);                                \
        }                                                                   \
    }

#ifdef _MSC_VER
f_report_hook SECTION_START_(HOOK_SECTION(PRE_ALL));
f_report_hook SECTION_START_(HOOK_SECTION(PRE_SUITE));
f_report_hook SECTION_START_(HOOK_SECTION(PRE_INIT));
f_report_hook SECTION_START_(HOOK_SECTION(PRE_TEST));
f_report_hook SECTION_START_(HOOK_SECTION(ASSERT));
f_report_hook SECTION_START_(HOOK_SECTION(THEORY_FAIL));
f_report_hook SECTION_START_(HOOK_SECTION(TEST_CRASH));
f_report_hook SECTION_START_(HOOK_SECTION(POST_TEST));
f_report_hook SECTION_START_(HOOK_SECTION(POST_FINI));
f_report_hook SECTION_START_(HOOK_SECTION(POST_SUITE));
f_report_hook SECTION_START_(HOOK_SECTION(POST_ALL));

f_report_hook SECTION_STOP_(HOOK_SECTION(PRE_ALL));
f_report_hook SECTION_STOP_(HOOK_SECTION(PRE_SUITE));
f_report_hook SECTION_STOP_(HOOK_SECTION(PRE_INIT));
f_report_hook SECTION_STOP_(HOOK_SECTION(PRE_TEST));
f_report_hook SECTION_STOP_(HOOK_SECTION(ASSERT));
f_report_hook SECTION_STOP_(HOOK_SECTION(THEORY_FAIL));
f_report_hook SECTION_STOP_(HOOK_SECTION(TEST_CRASH));
f_report_hook SECTION_STOP_(HOOK_SECTION(POST_TEST));
f_report_hook SECTION_STOP_(HOOK_SECTION(POST_FINI));
f_report_hook SECTION_STOP_(HOOK_SECTION(POST_SUITE));
f_report_hook SECTION_STOP_(HOOK_SECTION(POST_ALL));
#endif

IMPL_CALL_REPORT_HOOKS(PRE_ALL);
IMPL_CALL_REPORT_HOOKS(PRE_SUITE);
IMPL_CALL_REPORT_HOOKS(PRE_INIT);
IMPL_CALL_REPORT_HOOKS(PRE_TEST);
IMPL_CALL_REPORT_HOOKS(ASSERT);
IMPL_CALL_REPORT_HOOKS(THEORY_FAIL);
IMPL_CALL_REPORT_HOOKS(TEST_CRASH);
IMPL_CALL_REPORT_HOOKS(POST_TEST);
IMPL_CALL_REPORT_HOOKS(POST_FINI);
IMPL_CALL_REPORT_HOOKS(POST_SUITE);
IMPL_CALL_REPORT_HOOKS(POST_ALL);

ReportHook(PRE_ALL)() {}
ReportHook(PRE_SUITE)() {}
ReportHook(PRE_INIT)() {}
ReportHook(PRE_TEST)() {}
ReportHook(ASSERT)() {}
ReportHook(THEORY_FAIL)() {}
ReportHook(TEST_CRASH)() {}
ReportHook(POST_TEST)() {}
ReportHook(POST_FINI)() {}
ReportHook(POST_SUITE)() {}
ReportHook(POST_ALL)() {}

