/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CRITERION_REDIRECT_H_
# define CRITERION_REDIRECT_H_

# ifdef __cplusplus
#  include <cstdio>
# else
#  include <stdio.h>
# endif

# include "internal/common.h"
# include "internal/redirect.h"

CR_BEGIN_C_API

/**
 *  Redirect stdout for testing.
 */
CR_API void cr_redirect_stdout(void);

/**
 *  Redirect stderr for testing.
 */
CR_API void cr_redirect_stderr(void);

/**
 *  Redirect stdin for testing.
 *  This is implicitely called before each test.
 */
CR_API void cr_redirect_stdin(void);

/**
 *  Get a file handle representing the read-end of the redirected stdout.
 *
 *  @returns the file handle.
 */
CR_API CR_STDN FILE* cr_get_redirected_stdout(void);

/**
 *  Get a file handle representing the read-end of the redirected stderr.
 *
 *  @returns the file handle.
 */
CR_API CR_STDN FILE* cr_get_redirected_stderr(void);

/**
 *  Get a file handle representing the write-end of the redirected stdin.
 *
 *  @returns the file handle.
 */
CR_API CR_STDN FILE* cr_get_redirected_stdin(void);

/**
 *  Compare the contents of a file with a string.
 *
 *  @param[in] f    The file to compare the contents to.
 *  @param[in] str  The string to compare the contents to.
 *  @returns 1 if the contents of the file is equal to the string, 0 otherwise.
 */
CR_API int cr_file_match_str(CR_STDN FILE* f, const char *str);

/**
 *  Compare the contents of a file with the contents of another file.
 *
 *  @param[in] f    The first file to compare the contents to.
 *  @param[in] ref  The second file to compare the contents to.
 *  @returns 1 if the contents of the files are equal, 0 otherwise.
 */
CR_API int cr_file_match_file(CR_STDN FILE* f, CR_STDN FILE* ref);

/**
 *  Create a file mock.
 *
 *  @param[in] max_size The maximum size in bytes of the file mock.
 *  @returns the file handle representing the mock.
 */
CR_API CR_STDN FILE *cr_mock_file_size(size_t max_size);

CR_END_C_API

# define cr_assert_file_contents_eq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, ==, __VA_ARGS__))
# define cr_expect_file_contents_eq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, ==, __VA_ARGS__))

# define cr_assert_file_contents_neq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, !=, __VA_ARGS__))
# define cr_expect_file_contents_neq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, !=, __VA_ARGS__))

# define cr_assert_file_contents_eq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_ABORT_,     cr_file_match_file, ==, __VA_ARGS__))
# define cr_expect_file_contents_eq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_CONTINUES_, cr_file_match_file, ==, __VA_ARGS__))

# define cr_assert_file_contents_neq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_ABORT_,     cr_file_match_file, !=, __VA_ARGS__))
# define cr_expect_file_contents_neq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_CONTINUES_, cr_file_match_file, !=, __VA_ARGS__))

# define cr_assert_stdout_eq_str(...) CR_EXPAND(cr_assert_redir_unop_va_(CR_FAIL_ABORT_,     cr_stdout_match_str, ==, stdout, __VA_ARGS__))
# define cr_expect_stdout_eq_str(...) CR_EXPAND(cr_assert_redir_unop_va_(CR_FAIL_CONTINUES_, cr_stdout_match_str, ==, stdout, __VA_ARGS__))

# define cr_assert_stdout_neq_str(...) CR_EXPAND(cr_assert_redir_unop_va_(CR_FAIL_ABORT_,     cr_stdout_match_str, !=, stdout, __VA_ARGS__))
# define cr_expect_stdout_neq_str(...) CR_EXPAND(cr_assert_redir_unop_va_(CR_FAIL_CONTINUES_, cr_stdout_match_str, !=, stdout, __VA_ARGS__))

# define cr_assert_stderr_eq_str(...) CR_EXPAND(cr_assert_redir_unop_va_(CR_FAIL_ABORT_,     cr_stderr_match_str, ==, stderr, __VA_ARGS__))
# define cr_expect_stderr_eq_str(...) CR_EXPAND(cr_assert_redir_unop_va_(CR_FAIL_CONTINUES_, cr_stderr_match_str, ==, stderr, __VA_ARGS__))

# define cr_assert_stderr_neq_str(...) CR_EXPAND(cr_assert_redir_unop_va_(CR_FAIL_ABORT_,     cr_stderr_match_str, !=, stderr, __VA_ARGS__))
# define cr_expect_stderr_neq_str(...) CR_EXPAND(cr_assert_redir_unop_va_(CR_FAIL_CONTINUES_, cr_stderr_match_str, !=, stderr, __VA_ARGS__))

# define cr_assert_stdout_eq(...) CR_EXPAND(cr_assert_redir_f_unop_va_(CR_FAIL_ABORT_,     cr_stdout_match_file, ==, stdout, __VA_ARGS__))
# define cr_expect_stdout_eq(...) CR_EXPAND(cr_assert_redir_f_unop_va_(CR_FAIL_CONTINUES_, cr_stdout_match_file, ==, stdout, __VA_ARGS__))

# define cr_assert_stdout_neq(...) CR_EXPAND(cr_assert_redir_f_unop_va_(CR_FAIL_ABORT_,     cr_stdout_match_file, !=, stdout, __VA_ARGS__))
# define cr_expect_stdout_neq(...) CR_EXPAND(cr_assert_redir_f_unop_va_(CR_FAIL_CONTINUES_, cr_stdout_match_file, !=, stdout, __VA_ARGS__))

# define cr_assert_stderr_eq(...) CR_EXPAND(cr_assert_redir_f_unop_va_(CR_FAIL_ABORT_,     cr_stderr_match_file, ==, stderr, __VA_ARGS__))
# define cr_expect_stderr_eq(...) CR_EXPAND(cr_assert_redir_f_unop_va_(CR_FAIL_CONTINUES_, cr_stderr_match_file, ==, stderr, __VA_ARGS__))

# define cr_assert_stderr_neq(...) CR_EXPAND(cr_assert_redir_f_unop_va_(CR_FAIL_ABORT_,     cr_stderr_match_file, !=, stderr, __VA_ARGS__))
# define cr_expect_stderr_neq(...) CR_EXPAND(cr_assert_redir_f_unop_va_(CR_FAIL_CONTINUES_, cr_stderr_match_file, !=, stderr, __VA_ARGS__))

# ifdef __cplusplus
#  include "internal/stream.hxx"

namespace criterion {

    typedef internal::basic_ofstream<char> ofstream;
    typedef internal::basic_ifstream<char> ifstream;
    typedef internal::basic_fstream<char>  fstream;

    static inline ofstream& get_redirected_cin(void) {
        return internal::get_redirected_out_stream_::call(cr_get_redirected_stdin());
    }

    static inline ifstream& get_redirected_cout(void) {
        return internal::get_redirected_in_stream_::call(cr_get_redirected_stdout());
    }

    static inline ifstream& get_redirected_cerr(void) {
        return internal::get_redirected_in_stream_::call(cr_get_redirected_stderr());
    }

#  if __GNUC__ >= 5
    static inline fstream mock_file(size_t max_size) {
        return fstream(cr_mock_file_size(max_size));
    }

    static inline fstream mock_file(void) {
        return mock_file(4096);
    }
#  endif
}
# endif

#endif /* !CRITERION_REDIRECT_H_ */
