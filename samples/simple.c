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

ReportHook(PRE_INIT) {
    struct criterion_test *test = data;

    printf("testing %s in category %s\n", test->name, test->category);
}

void criterion_init(void) {
    puts("criterion_init");
}

void criterion_fini(void) {
    puts("criterion_fini");
}
