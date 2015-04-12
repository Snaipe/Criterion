#include <criterion/criterion.h>

void setup_suite(void) {
}

void teardown_suite(void) {
}

TestSuite(suite1, .init = setup_suite, .fini = teardown_suite);

Test(suite1, test) {
    assert(1);
}

Test(suite2, test) {
    assert(1);
}

TestSuite(disabled, .disabled = true);

Test(disabled, test) {}
