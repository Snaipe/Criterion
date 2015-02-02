#include <stdio.h>
#include <stdlib.h>
#include "criterion.h"
#include "report.h"
#include "assert.h"

Test(misc, failing) {
    assert(1);
    assert(0);
}

Test(misc, simple) {
    expect(1);
}

ReportHook(PRE_INIT)(struct criterion_test *test) {
    printf("testing %s in category %s\n", test->name, test->category);
}

ReportHook(POST_TEST)(struct criterion_test_stats *stats) {
    printf("Asserts: [%d passed, %d failed, %d total]\n",
            stats->passed, stats->failed, stats->passed + stats->failed);
}

ReportHook(PRE_EVERYTHING)() {
    puts("criterion_init");
}

ReportHook(POST_EVERYTHING)() {
    puts("criterion_fini");
}
