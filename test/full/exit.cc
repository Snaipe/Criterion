#include <stdio.h>
#include <stdlib.h>
#include <criterion/criterion.h>

Test(exit, normal, .exit_code = 0) {
}

Test(exit, expected_exit, .exit_code = 42) {
    exit(42);
}

Test(exit, unexpected_exit) {
    exit(127);
}

void do_exit(void)
{
    exit(127);
}

Test(exit_with_fixtures, init_exits, .init = do_exit) {
}

Test(exit_with_fixtures, fini_exits, .fini = do_exit) {
}
