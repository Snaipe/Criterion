#include <stdio.h>
#include <criterion/criterion.h>
#include <criterion/hooks.h>

ReportHook(PRE_INIT)(CR_UNUSED struct criterion_test *test) {
    puts("PRE_INIT hook from bug584-other-file.c");
}

_Static_assert(__LINE__ == 9, "PRE_INIT hook must stay on the same line as in bug584.c");
