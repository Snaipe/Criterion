#include <criterion/criterion.h>

void setup_suite(void) {
    // setup suite
}

TestSuite(suite1, .init = setup_suite);

Test(suite1, test) {
    assert(1);
}

Test(suite2, test) {
    assert(1);
}

TestSuite(disabled, .disabled = true);

Test(disabled, test) {}
