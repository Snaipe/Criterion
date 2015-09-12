#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <string>
#include <iostream>
#include <fstream>
#include <cctype>

// Testing stdout/stderr

void redirect_all_std(void) {
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(redirect, test_outputs, .init = redirect_all_std) {
    std::FILE* f_stdout = cr_get_redirected_stdout();

    std::cout << "foo" << std::flush;

    cr_assert_file_contents_match_str(f_stdout, "foo");

    std::FILE* f_stderr = cr_get_redirected_stderr();

    std::cerr << "bar" << std::flush;

    cr_assert_file_contents_match_str(f_stderr, "bar");
}

// Testing general I/O with sample command-line rot13

char rot13_char(char c) {
    return std::isalpha(c) ? (c - 'a' + 13) % 26 + 'a' : c;
}

void rot13_io(void) {
    std::string s;

    std::cin >> s;
    for (size_t i = 0; i < s.length(); ++i)
        s[i] = rot13_char(s[i]);
    std::cout << s << std::flush;
}

Test(redirect, rot13, .init = cr_redirect_stdout) {
    std::FILE* f_stdin = cr_get_redirected_stdin();
    std::ofstream f_cin(f_stdin);

    f_cin << "the quick brown fox jumps over the lazy dog";
    f_cin.close();

    rot13_io();

    std::FILE* f_stdout = cr_get_redirected_stdout();
    cr_assert_file_contents_match_str(f_stdout, "gur dhvpx oebja sbk whzcf bire gur ynml qbt");
}
