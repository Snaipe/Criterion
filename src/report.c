#include <stdio.h>
#include "criterion.h"
#include "report.h"

#define IMPL_CALL_REPORT_HOOKS(Kind)                        \
    static f_report_hook * const g_##Kind##_section_start = \
        &__start_criterion_hooks_##Kind;                    \
    static f_report_hook * const g_##Kind##_section_end =   \
        &__stop_criterion_hooks_##Kind;                     \
    void call_report_hooks_##Kind(void *data) {             \
        for (f_report_hook *hook = g_##Kind##_section_start;\
             hook < g_##Kind##_section_end;                 \
             ++hook) {                                      \
            (*hook)(data);                                  \
        }                                                   \
    }

IMPL_CALL_REPORT_HOOKS(PRE_EVERYTHING);
IMPL_CALL_REPORT_HOOKS(PRE_INIT);
IMPL_CALL_REPORT_HOOKS(PRE_TEST);
IMPL_CALL_REPORT_HOOKS(POST_TEST);
IMPL_CALL_REPORT_HOOKS(POST_FINI);
IMPL_CALL_REPORT_HOOKS(POST_EVERYTHING);

ReportHook(PRE_INIT) {
    struct criterion_test *test = data;

    fprintf(stderr, "%s::%s: RUNNING\n", test->category, test->name);
}

ReportHook(POST_TEST) {
    struct criterion_test_stats *stats = data;

    int success = stats->failed == 0;

    fprintf(stderr, "%s::%s: %s\n", stats->test->category, stats->test->name, success ? "SUCCESS" : "FAILURE");
}

ReportHook(PRE_TEST) {}
ReportHook(POST_FINI) {}

ReportHook(PRE_EVERYTHING) {}
ReportHook(POST_EVERYTHING) {}
