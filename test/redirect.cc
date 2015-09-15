#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>

// set a timeout for I/O tests
TestSuite(redirect, .timeout = 0.1);

#if __GNUC__ >= 5
Test(redirect, mock) {
    auto fmock = criterion::mock_file();

    fmock << "Hello" << std::flush;
    fmock.seekg(0);

    std::string contents;
    fmock >> contents;

    cr_assert_eq(contents, "Hello");
}
#endif

Test(redirect, mock_c) {
    std::FILE* fmock = cr_mock_file_size(4096);

    std::fprintf(fmock, "Hello");
    std::fflush(fmock);
    std::rewind(fmock);

    char contents[sizeof ("Hello")] = {0};
    fgets(contents, sizeof (contents), fmock);

    cr_assert_str_eq(contents, "Hello");
}

Test(redirect, stdout_) {
    cr_redirect_stdout();

    std::cout << "Foo" << std::flush;

    cr_expect_stdout_eq_str("Foo");
    cr_expect_stdout_neq_str("Bar");
}

Test(redirect, stderr_) {
    cr_redirect_stderr();

    std::cerr << "Foo" << std::flush;

    cr_expect_stderr_eq_str("Foo");
    cr_expect_stderr_neq_str("Bar");
}

Test(redirect, stdin_) {
    auto& f_cin = criterion::get_redirected_cin();
    f_cin << "Foo";
    f_cin.close();

    std::string read;
    std::cin >> read;

    cr_expect_eq(read, "Foo");
    cr_expect_neq(read, "Bar");
}
