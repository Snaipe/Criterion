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
#include <fnmatch.h>
#include "criterion/types.h"
#include "criterion/stats.h"
#include "criterion/logging.h"
#include "criterion/options.h"
#include "criterion/ordered-set.h"
#include "report.h"

#define IMPL_CALL_REPORT_HOOKS(Kind)                                \
    IMPL_SECTION_LIMITS(f_report_hook, crit_ ## Kind);              \
    void call_report_hooks_##Kind(void *data) {                     \
        for (f_report_hook *hook = SECTION_START(crit_ ## Kind);    \
             hook < SECTION_END(crit_ ## Kind);                     \
             ++hook) {                                              \
            (*hook)(data);                                          \
        }                                                           \
    }

#define IMPL_REPORT_HOOK(Type)      \
    IMPL_CALL_REPORT_HOOKS(Type);   \
    ReportHook(Type)

#define log(Type, Arg) \
    (criterion_options.output_provider->log_ ## Type ?: nothing)(Arg);

__attribute__((always_inline))
static inline void nothing() {}

IMPL_REPORT_HOOK(PRE_ALL)(struct criterion_test_set *set) {
    if (criterion_options.pattern) {
        FOREACH_SET(struct criterion_suite_set *s, set->suites) {
            if ((s->suite.data && s->suite.data->disabled) || !s->tests)
                continue;

            FOREACH_SET(struct criterion_test *test, s->tests) {
                char *name = NULL;
                asprintf(&name, "%s/%s", s->suite.name, test->name);
                if (fnmatch(criterion_options.pattern, name, 0))
                    test->data->disabled = true;

                free(name);
            }
        }
    }
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

IMPL_REPORT_HOOK(POST_SUITE)(struct criterion_suite_set *set) {
    log(post_suite, set);
}

IMPL_REPORT_HOOK(POST_ALL)(struct criterion_global_stats *stats) {
    log(post_all, stats);
}
