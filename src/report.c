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

IMPL_CALL_REPORT_HOOKS(PRE_INIT);
IMPL_CALL_REPORT_HOOKS(PRE_TEST);
IMPL_CALL_REPORT_HOOKS(POST_TEST);
IMPL_CALL_REPORT_HOOKS(POST_FINI);

ReportHook(PRE_INIT) {
    struct criterion_test *test = data;

    printf("%s::%s: ", test->category, test->name);
    fflush(stdout);
}

ReportHook(POST_TEST) {

}

ReportHook(PRE_TEST) {}
ReportHook(POST_FINI) {}
