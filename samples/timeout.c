#include <criterion/criterion.h>

#ifdef _WIN32
# include <Windows.h>
# define sleep(x) Sleep(x * 1000)
#else
# include <unistd.h>
#endif

Test(timeout, simple, .timeout = 1.) {
    sleep(10);
}
