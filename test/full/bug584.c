#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/hooks.h>

ReportHook(PRE_INIT)(CR_UNUSED struct criterion_test *test) {
    puts("PRE_INIT hook from bug584.c");
}

_Static_assert(__LINE__ == 9, "PRE_INIT hook must stay on the same line as in bug584-other-file.c");

ReportHook(PRE_ALL)(CR_UNUSED struct criterion_test_set *tests) {
    setbuf(stdout, NULL);
}

#define TWO_HOOKS_ON_ONE_LINE                                                 \
    ReportHook(PRE_TEST)(CR_UNUSED struct criterion_test *test) {             \
        puts("PRE_TEST hook from a macro");                                   \
    }                                                                         \
    ReportHook(POST_TEST)(CR_UNUSED struct criterion_test_stats *stats) {     \
        puts("POST_TEST hook from the same macro");                           \
    }

TWO_HOOKS_ON_ONE_LINE

Test(bug584, hooks_on_colliding_lines_all_fire) {
    cr_assert(1);
}
