#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/new/assert.h>
#include <iostream>
#include <string>

/* set a timeout for I/O tests */
TestSuite(redirect, .timeout = 1);

#if __GNUC__ >= 5
Test(redirect, mock) {
    auto fmock = criterion::mock_file();

    fmock << "Hello" << std::flush;
    fmock.seekg(0);

    std::string contents;
    fmock >> contents;

    cr_assert(eq(str, contents, "Hello"));
}
#endif

Test(redirect, mock_c) {
    std::FILE *fmock = cr_mock_file_size(4096);

    std::fprintf(fmock, "Hello");
    std::fflush(fmock);
    std::rewind(fmock);

    char contents[sizeof ("Hello")] = { 0 };
    fgets(contents, sizeof (contents), fmock);

    cr_assert(eq(str, contents, "Hello"));
}

Test(redirect, assertions) {
    std::FILE *f1 = cr_mock_file_size(4096);
    std::FILE *f2 = cr_mock_file_size(4096);
    std::FILE *f3 = cr_mock_file_size(4096);

    fprintf(f1, "Foo");
    fprintf(f2, "Foo");
    fprintf(f3, "Bar");

    fflush(f1);
    fflush(f2);
    fflush(f3);

    cr_expect_file_contents_eq(f1, f1);
    rewind(f1);

    cr_expect_file_contents_eq(f1, f2);
    rewind(f1);

    cr_expect_file_contents_neq(f1, f3);

    fclose(f1);
    fclose(f2);
    fclose(f3);
}

Test(redirect, stdout_) {
    cr_redirect_stdout();

    std::cout << "Foo" << std::flush;

    cr_expect_stdout_eq_str("Foo");
}

Test(redirect, stderr_) {
    cr_redirect_stderr();

    std::cerr << "Foo" << std::flush;

    cr_expect_stderr_eq_str("Foo");
}

Test(redirect, stdin_) {
    auto &f_cin = criterion::get_redirected_cin();

    f_cin << "Foo";
    f_cin.close();

    std::string read;
    std::cin >> read;

    cr_expect_eq(read, "Foo");
}

Test(redirect, stdout_empty) {
    cr_redirect_stdout();
    cr_expect_stdout_eq_str("");
}
