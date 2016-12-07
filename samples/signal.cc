#include <csignal>
#include <criterion/criterion.h>

void crash(void);

Test(simple, caught, .signal = SIGSEGV) {
    crash();
}

Test(simple, wrong_signal, .signal = SIGINT) {
    crash();
}

Test(simple, uncaught) {
    crash();
}

void crash(void)
{
    volatile int *ptr = NULL;

    *ptr = 42;
}
