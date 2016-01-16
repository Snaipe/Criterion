#include <csignal>
#include <criterion/criterion.h>

Test(simple, caught, .signal = SIGSEGV) {
    std::raise(SIGSEGV);
}

Test(simple, wrong_signal, .signal = SIGINT) {
    std::raise(SIGSEGV);
}

Test(simple, uncaught) {
    std::raise(SIGSEGV);
}
