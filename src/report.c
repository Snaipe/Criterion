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

#ifdef HAVE_PCRE
#include "extmatch.h"
#endif

#define IMPL_CALL_REPORT_HOOKS(Kind)                                    \
    IMPL_SECTION_LIMITS(f_report_hook, crit_ ## Kind);                  \
    void call_report_hooks_##Kind(void *data) {                         \
        for (f_report_hook *hook = GET_SECTION_START(crit_ ## Kind);    \
             hook < (f_report_hook*) GET_SECTION_END(crit_ ## Kind);    \
             ++hook) {                                                  \
            (*hook)(data);                                              \
        }                                                               \
    }

#define IMPL_REPORT_HOOK(Type)      \
    IMPL_CALL_REPORT_HOOKS(Type);   \
    ReportHook(Type)

__attribute__((always_inline))
static inline void nothing() {}

#ifdef HAVE_PCRE
void disable_unmatching(struct criterion_test_set *set) {
    FOREACH_SET(struct criterion_suite_set *s, set->suites) {
        if ((s->suite.data && s->suite.data->disabled) || !s->tests)
            continue;

        FOREACH_SET(struct criterion_test *test, s->tests) {
            const char *errmsg;
            int ret = extmatch(criterion_options.pattern, test->data->identifier_, &errmsg);
            if (ret == -10) {
                printf("pattern error: %s\n", errmsg);
                exit(1);
            } else if (ret < 0) {
                test->data->disabled = true;
            }
        }
    }
}
#endif

IMPL_REPORT_HOOK(PRE_ALL)(struct criterion_test_set *set) {
#ifdef HAVE_PCRE
    if (criterion_options.pattern) {
        disable_unmatching(set);
    }
#endif
    log(pre_all, set);
}

IMPL_REPORT_HOOK(PRE_SUITE)(struct criterion_suite_set *set) {
    log(pre_suite, set);
}

IMPL_REPORT_HOOK(PRE_INIT)(struct criterion_test *test) {
    log(pre_init, test);
}

IMPL_REPORT_HOOK(PRE_TEST)(struct criterion_test *test) {
    log(pre_test, test);
}

IMPL_REPORT_HOOK(ASSERT)(struct criterion_assert_stats *stats) {
    log(assert, stats);
}

IMPL_REPORT_HOOK(TEST_CRASH)(struct criterion_test_stats *stats) {
    log(test_crash, stats);
}

IMPL_REPORT_HOOK(POST_TEST)(struct criterion_test_stats *stats) {
    log(post_test, stats);
}

IMPL_REPORT_HOOK(POST_FINI)(struct criterion_test_stats *stats) {
    log(post_fini, stats);
}

IMPL_REPORT_HOOK(POST_SUITE)(struct criterion_suite_stats *stats) {
    log(post_suite, stats);
}

IMPL_REPORT_HOOK(POST_ALL)(struct criterion_global_stats *stats) {
    log(post_all, stats);
}
