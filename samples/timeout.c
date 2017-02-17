#include <criterion/criterion.h>

#ifdef _WIN32
# include <windows.h>
# define sleep(x)    Sleep(x * 1000)
#else
# include <unistd.h>
#endif

Test(timeout, simple, .timeout = 1.) {
    sleep(10);
}
