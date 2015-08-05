#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <criterion/criterion.h>

Test(simple, caught, .signal = SIGSEGV) {
    int *i = NULL;
    *i = 42;
}

Test(simple, wrong_signal, .signal = SIGINT) {
    int *i = NULL;
    *i = 42;
}

Test(simple, uncaught) {
    int *i = NULL;
    *i = 42;
}
