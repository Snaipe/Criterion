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

# include "common.h"
# include "assert.h"

# ifdef __cplusplus
#  include <cstdio>
#  include <memory>
#  include <fstream>

#  ifdef __GNUC__
#   include <ext/stdio_filebuf.h>
#  endif
# else
#  include <stdio.h>
# endif

CR_BEGIN_C_API

CR_API void cr_redirect_stdout(void);
CR_API void cr_redirect_stderr(void);
CR_API void cr_redirect_stdin(void);

CR_API CR_STDN FILE* cr_get_redirected_stdout(void);
CR_API CR_STDN FILE* cr_get_redirected_stderr(void);
CR_API CR_STDN FILE* cr_get_redirected_stdin(void);

CR_API int cr_file_match_str(CR_STDN FILE* f, const char *str);

CR_END_C_API

# define cr_assert_redir_op_(Fail, Fun, Op, File, Str, ...)     \
    CR_EXPAND(cr_assert_impl(                                   \
            Fail,                                               \
            !(Fun((File), (Str)) Op 0),                         \
            dummy,                                              \
            CRITERION_ASSERT_MSG_FILE_STR_MATCH,                \
            (CR_STR(File), Str),                                \
            __VA_ARGS__                                         \
    ))

# define cr_assert_redir_op_va_(Fail, Fun, Op, ...)             \
    CR_EXPAND(cr_assert_redir_op_(                              \
            Fail,                                               \
            Fun,                                                \
            Op,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                            \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                 \
    ))

# define cr_assert_file_contents_match_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, ==, __VA_ARGS__))
# define cr_expect_file_contents_match_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, ==, __VA_ARGS__))

# define cr_assert_file_contents_not_match_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, !=, __VA_ARGS__))
# define cr_expect_file_contents_not_match_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, !=, __VA_ARGS__))

# ifdef __cplusplus
namespace criterion {

    template <typename CharT>
    class basic_ofstream : public std::basic_ofstream<CharT> {
    public:
        basic_ofstream(FILE* f)
#  ifdef __GNUC__
            : std::ofstream()
            , fbuf(new ::__gnu_cxx::stdio_filebuf<CharT>(f, std::ios::out))
#  else
            : std::ofstream(f)
#  endif
            , file(f)
        {
            std::ios::rdbuf(&*fbuf);
        }

        void close(void) {
            std::basic_ofstream<CharT>::flush();
            std::basic_ofstream<CharT>::close();
            std::fclose(file);
        }

    private:
#  ifdef __GNUC__
        std::unique_ptr<::__gnu_cxx::stdio_filebuf<CharT>> fbuf;
#  endif
        std::FILE* file;
    };

    template <typename CharT>
    class basic_ifstream : public std::basic_ifstream<CharT> {
    public:
        basic_ifstream(FILE* f)
#  ifdef __GNUC__
            : std::ifstream()
            , fbuf(new ::__gnu_cxx::stdio_filebuf<CharT>(f, std::ios::in))
#  else
            : std::ifstream(f)
#  endif
            , file(f)
        {
            std::ios::rdbuf(&*fbuf);
        }

        void close(void) {
            std::basic_ifstream<CharT>::flush();
            std::basic_ifstream<CharT>::close();
            std::fclose(file);
        }

    private:
#  ifdef __GNUC__
        std::unique_ptr<::__gnu_cxx::stdio_filebuf<CharT>> fbuf;
#  endif
        std::FILE* file;
    };

    template <typename CharT>
    struct get_redirected_out_stream_ {
        static inline basic_ofstream<CharT>& call(std::FILE* f) {
            static std::unique_ptr<basic_ofstream<CharT>> stream;

            if (!stream)
                stream.reset(new basic_ofstream<CharT>(f));
            return *stream;
        }

    };

    template <typename CharT>
    struct get_redirected_in_stream_ {
        static inline basic_ifstream<CharT>& call(std::FILE* f) {
            static std::unique_ptr<basic_ifstream<CharT>> stream;
            if (!stream)
                stream.reset(new basic_ifstream<CharT>(f));
            return *stream;
        }
    };

    using ofstream = basic_ofstream<char>;
    using ifstream = basic_ifstream<char>;

    static inline ofstream& get_redirected_cin(void) {
        return get_redirected_out_stream_<char>::call(cr_get_redirected_stdin());
    }

    static inline ifstream& get_redirected_cout(void) {
        return get_redirected_in_stream_<char>::call(cr_get_redirected_stdout());
    }

    static inline ifstream& get_redirected_cerr(void) {
        return get_redirected_in_stream_<char>::call(cr_get_redirected_stderr());
    }
}
# endif

#endif /* !CRITERION_REDIRECT_H_ */
