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
#   include <ext/stdio_sync_filebuf.h>
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
CR_API int cr_file_match_file(CR_STDN FILE* f, CR_STDN FILE* ref);

CR_API CR_STDN FILE *cr_mock_file_size(size_t max_size);

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

# define cr_assert_redir_f_op_(Fail, Fun, Op, File, Ref, ...)   \
    CR_EXPAND(cr_assert_impl(                                   \
            Fail,                                               \
            !(Fun((File), (Ref)) Op 0),                         \
            dummy,                                              \
            CRITERION_ASSERT_MSG_FILE_MATCH,                    \
            (CR_STR(File), CR_STR(Ref)),                        \
            __VA_ARGS__                                         \
    ))

# define cr_assert_redir_f_op_va_(Fail, Fun, Op, ...)           \
    CR_EXPAND(cr_assert_redir_op_(                              \
            Fail,                                               \
            Fun,                                                \
            Op,                                                 \
            CR_VA_HEAD(__VA_ARGS__),                            \
            CR_VA_HEAD(CR_VA_TAIL(__VA_ARGS__)),                \
            CR_VA_TAIL(CR_VA_TAIL(__VA_ARGS__))                 \
    ))

# define cr_assert_file_contents_eq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, ==, __VA_ARGS__))
# define cr_expect_file_contents_eq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, ==, __VA_ARGS__))

# define cr_assert_file_contents_neq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, !=, __VA_ARGS__))
# define cr_expect_file_contents_neq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, !=, __VA_ARGS__))

# define cr_assert_file_contents_eq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_ABORT_,     cr_file_match_file, ==, __VA_ARGS__))
# define cr_expect_file_contents_eq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_CONTINUES_, cr_file_match_file, ==, __VA_ARGS__))

# define cr_assert_file_contents_neq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_ABORT_,     cr_file_match_file, !=, __VA_ARGS__))
# define cr_expect_file_contents_neq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_CONTINUES_, cr_file_match_file, !=, __VA_ARGS__))

# define cr_assert_stdout_eq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, ==, cr_get_redirected_stdout(), __VA_ARGS__))
# define cr_expect_stdout_eq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, ==, cr_get_redirected_stdout(), __VA_ARGS__))

# define cr_assert_stdout_neq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, !=, cr_get_redirected_stdout(), __VA_ARGS__))
# define cr_expect_stdout_neq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, !=, cr_get_redirected_stdout(), __VA_ARGS__))

# define cr_assert_stderr_eq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, ==, cr_get_redirected_stderr(), __VA_ARGS__))
# define cr_expect_stderr_eq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, ==, cr_get_redirected_stderr(), __VA_ARGS__))

# define cr_assert_stderr_neq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_ABORT_,     cr_file_match_str, !=, cr_get_redirected_stderr(), __VA_ARGS__))
# define cr_expect_stderr_neq_str(...) CR_EXPAND(cr_assert_redir_op_va_(CR_FAIL_CONTINUES_, cr_file_match_str, !=, cr_get_redirected_stderr(), __VA_ARGS__))

# define cr_assert_stdout_eq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_ABORT_,     cr_file_match_file, ==, cr_get_redirected_stdout(), __VA_ARGS__))
# define cr_expect_stdout_eq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_CONTINUES_, cr_file_match_file, ==, cr_get_redirected_stdout(), __VA_ARGS__))

# define cr_assert_stdout_neq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_ABORT_,     cr_file_match_file, !=, cr_get_redirected_stdout(), __VA_ARGS__))
# define cr_expect_stdout_neq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_CONTINUES_, cr_file_match_file, !=, cr_get_redirected_stdout(), __VA_ARGS__))

# define cr_assert_stderr_eq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_ABORT_,     cr_file_match_file, ==, cr_get_redirected_stderr(), __VA_ARGS__))
# define cr_expect_stderr_eq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_CONTINUES_, cr_file_match_file, ==, cr_get_redirected_stderr(), __VA_ARGS__))

# define cr_assert_stderr_neq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_ABORT_,     cr_file_match_file, !=, cr_get_redirected_stderr(), __VA_ARGS__))
# define cr_expect_stderr_neq(...) CR_EXPAND(cr_assert_redir_f_op_va_(CR_FAIL_CONTINUES_, cr_file_match_file, !=, cr_get_redirected_stderr(), __VA_ARGS__))

# ifdef __cplusplus
namespace criterion {

    template <typename CharT, typename Super>
    class stream_mixin : public Super {
public:
        stream_mixin(FILE* f)
#  ifdef __GNUC__
            : Super()
            , fbuf(new ::__gnu_cxx::stdio_sync_filebuf<CharT>(f))
#  else
            : Super(f)
#  endif
            , file(f)
        {
#  ifdef __GNUC__
            std::ios::rdbuf(&*fbuf);
#  endif
        }

        stream_mixin(const stream_mixin& other) = delete;
        stream_mixin& operator=(const stream_mixin& other) = delete;

        stream_mixin(stream_mixin&& other) :
#  ifdef __GNUC__
            fbuf(std::move(other.fbuf)),
#  endif
            file(std::move(other.file))
        {}

        stream_mixin& operator=(stream_mixin&& other) {
#  ifdef __GNUC__
            fbuf = std::move(other.fbuf);
#  endif
            file = std::move(other.file);
        }

        void close(void) {
            Super::flush();
            Super::close();
            std::fclose(file);
        }

    private:
#  ifdef __GNUC__
        std::shared_ptr<::__gnu_cxx::stdio_sync_filebuf<CharT>> fbuf;
#  endif
        std::FILE* file;
    };

    template <typename CharT>
    using ofstream_mixin = stream_mixin<CharT, std::basic_ofstream<CharT>>;

    template <typename CharT>
    using ifstream_mixin = stream_mixin<CharT, std::basic_ifstream<CharT>>;

    template <typename CharT>
    using fstream_mixin = stream_mixin<CharT, std::basic_fstream<CharT>>;

    template <typename CharT>
    class basic_ofstream : public ofstream_mixin<CharT> {
    public:
        basic_ofstream(FILE* f)
            : ofstream_mixin<CharT>(f)
        {}

        basic_ofstream(basic_ofstream&& other)
            : ofstream_mixin<CharT>(std::move(other))
        {}
    };

    template <typename CharT>
    class basic_ifstream : public ifstream_mixin<CharT> {
    public:
        basic_ifstream(FILE* f)
            : ifstream_mixin<CharT>(f)
        {}

        basic_ifstream(basic_ifstream&& other)
            : ifstream_mixin<CharT>(std::move(other))
        {}
    };

    template <typename CharT>
    class basic_fstream : public fstream_mixin<CharT> {
    public:
        basic_fstream(FILE* f)
            : fstream_mixin<CharT>(f)
        {}

        basic_fstream(basic_fstream&& other)
            : fstream_mixin<CharT>(std::move(other))
        {}
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
    using fstream  = basic_fstream<char>;

    static inline ofstream& get_redirected_cin(void) {
        return get_redirected_out_stream_<char>::call(cr_get_redirected_stdin());
    }

    static inline ifstream& get_redirected_cout(void) {
        return get_redirected_in_stream_<char>::call(cr_get_redirected_stdout());
    }

    static inline ifstream& get_redirected_cerr(void) {
        return get_redirected_in_stream_<char>::call(cr_get_redirected_stderr());
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
