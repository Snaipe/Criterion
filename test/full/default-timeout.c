#include <criterion/criterion.h>

#ifdef _WIN32
# include <windows.h>
# define sleep(x)    Sleep(x * 1000)
#else
# include <unistd.h>
#endif

/* No .timeout set, should be killed when --default-timeout is passed. */
Test(timeout, no_timeout_set) {
    sleep(10);
}

/* Explicit .timeout = 5, --default-timeout 1 must not override it. */
Test(timeout, explicit_not_overridden, .timeout = 5.) {
    sleep(2);
}
