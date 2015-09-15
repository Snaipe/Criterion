#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <iostream>

// set a timeout for I/O tests
TestSuite(redirect, .timeout = 0.1);

Test(redirect, mock) {
    auto fmock = criterion::mock_file();

    fmock << "Hello" << std::flush;
    fmock.seekg(0);

    std::string contents;
    fmock >> contents;

    cr_assert_eq(contents, "Hello");
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
