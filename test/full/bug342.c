#include <criterion/criterion.h>
#include <signal.h>
#include <stdlib.h>

static void skip_from_init(void)
{
    cr_skip_test("skipped from init");
}

TestSuite(bug342_init, .init = skip_from_init);

Test(bug342_init, signal, .signal = SIGSEGV) {
    raise(SIGSEGV);
}

Test(bug342_init, exit_code, .exit_code = 1) {
    exit(1);
}

Test(bug342_init, plain) {
    cr_assert(1);
}

Test(bug342_body, signal, .signal = SIGSEGV) {
    cr_skip_test("skipped from body");
}

Test(bug342_body, exit_code, .exit_code = 1) {
    cr_skip_test("skipped from body");
}
