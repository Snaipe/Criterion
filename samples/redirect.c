#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <stdio.h>

Test(redirect, stdout) {
    cr_redirect_stdout();

    fprintf(stdout, "foo");
    fflush(stdout);

    FILE* stdout_in = cr_get_redirected_stdout();

    cr_assert_file_contents_match_str(stdout_in, "foo");
}
