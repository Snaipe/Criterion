#include <stdio.h>
#include <criterion/criterion.h>

Test(sample, test) {
    cr_expect(0);
    cr_assert(1);
}

ReportHook(PRE_INIT)(struct criterion_test *test) {
    printf("testing %s in category %s\n", test->name, test->category);
}

ReportHook(POST_TEST)(struct criterion_test_stats *stats) {
    printf("Asserts: [%d passed, %d failed, %d total]\n",
            stats->passed_asserts, stats->failed_asserts, stats->passed_asserts + stats->failed_asserts);
}

ReportHook(PRE_ALL)() {
    puts("criterion_init");
}

ReportHook(POST_ALL)() {
    puts("criterion_fini");
}
