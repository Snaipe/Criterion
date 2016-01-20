#include <signal.h>
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


// Cross platform segfault simulator ™
// a.k.a. "I can't believe I have to write this for a sample"

#ifdef _WIN32
# include <windows.h>
#endif

void crash(void) {
#ifdef _WIN32
    // This translates to a SIGSEGV
    RaiseException(EXCEPTION_ACCESS_VIOLATION, EXCEPTION_NONCONTINUABLE, 0, NULL);
#else
    raise(SIGSEGV);
#endif
}
