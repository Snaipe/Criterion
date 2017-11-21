#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <string>
#include <iostream>
#include <cctype>

/* Testing stdout/stderr */

void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(redirect, test_outputs, .init = redirect_all_std) {
    std::cout << "foo" << std::flush;
    std::cerr << "bar" << std::flush;

    cr_assert_stdout_eq_str("foo");
    cr_assert_stderr_eq_str("bar");
}

/* Testing general I/O with sample command-line rot13 */

char rot13_char(char c)
{
    return std::isalpha(c) ? (c - 'a' + 13) % 26 + 'a' : c;
}

void rot13_io(void)
{
    std::string s;

    std::getline(std::cin, s);
    for (size_t i = 0; i < s.length(); ++i)
        s[i] = rot13_char(s[i]);
    std::cout << s << std::flush;
}

Test(redirect, rot13, .init = cr_redirect_stdout) {
    auto &f_cin = criterion::get_redirected_cin();

    f_cin << "the quick brown fox jumps over the lazy dog";
    f_cin.close();

    rot13_io();

    cr_assert_stdout_eq_str("gur dhvpx oebja sbk whzcf bire gur ynml qbt");
}
