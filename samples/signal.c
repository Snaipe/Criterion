#include <signal.h>
#include <criterion/criterion.h>

Test(simple, caught, .signal = SIGSEGV) {
    raise(SIGSEGV);
}

Test(simple, wrong_signal, .signal = SIGINT) {
    raise(SIGSEGV);
}

Test(simple, uncaught) {
    raise(SIGSEGV);
}
