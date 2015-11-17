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

# include "internal/common.h"
# include "assert.h"

# ifdef __cplusplus
#  include <cstdio>
#  include <memory>
#  include <fstream>
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

    template <typename CharT, typename Traits = std::char_traits<CharT>>
    class stdio_sync_filebuf : public std::basic_streambuf<CharT, Traits> {
    public:
        typedef Traits traits;
        typedef std::basic_filebuf<CharT, Traits> super;
        typedef typename Traits::int_type int_type;
        typedef typename Traits::pos_type pos_type;
        typedef typename Traits::off_type off_type;

        stdio_sync_filebuf(std::FILE *file)
            : file(file)
            , lastchar(Traits::eof())
        {}

        stdio_sync_filebuf(stdio_sync_filebuf&& other) = default;
        stdio_sync_filebuf& operator=(stdio_sync_filebuf&& other) = default;

        void swap(stdio_sync_filebuf& other) {
            super::swap(other);
            std::swap(file, other.file);
            std::swap(lastchar, other.lastchar);
        }

    protected:
        int_type syncgetc();
        int_type syncungetc(int_type);
        int_type syncputc(int_type);

        virtual std::streampos seekoff(std::streamoff off,
                std::ios_base::seekdir dir,
                std::ios_base::openmode = std::ios_base::in | std::ios_base::out) {

            int whence;
            if (dir == std::ios_base::beg)
                whence = SEEK_SET;
            else if (dir == std::ios_base::cur)
                whence = SEEK_CUR;
            else
                whence = SEEK_END;

            if (!fseek(file, off, whence))
                return std::streampos(std::ftell(file));
            return std::streamoff(-1);
        }

        virtual std::streampos seekpos(std::streampos pos,
                std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out) {
            return seekoff(std::streamoff(pos), std::ios_base::beg, mode);
        }

        virtual std::streamsize xsgetn(CharT* s, std::streamsize n);
        virtual std::streamsize xsputn(const CharT* s, std::streamsize n);

        virtual int sync() {
            return std::fflush(file);
        }

        virtual int_type underflow() {
            int_type c = syncgetc();
            return syncungetc(c);
        }

        virtual int_type uflow() {
            return lastchar = syncgetc();
        }

        static inline bool is_eof(int_type c) {
            static const int_type eof = Traits::eof();
            return Traits::eq_int_type(c, eof);
        }

        virtual int_type overflow(int_type c = Traits::eof()) {
            int_type ret;
            if (is_eof(c)) {
                if (std::fflush(file))
                    ret = Traits::eof();
                else
                    ret = Traits::not_eof(c);
            } else {
                ret = syncputc(c);
            }
            return ret;
        }

        virtual int_type pbackfail(int_type c = Traits::eof()) {
            int_type ret = syncungetc(is_eof(c) && !is_eof(lastchar) ? lastchar : c);
            lastchar = Traits::eof();
            return ret;
        }

    private:
        std::FILE *file;
        bool file_open;
        int_type lastchar;
    };

    template <>
    inline stdio_sync_filebuf<char>::int_type
    stdio_sync_filebuf<char>::syncgetc() {
        return std::getc(file);
    }

    template <>
    inline stdio_sync_filebuf<char>::int_type
    stdio_sync_filebuf<char>::syncungetc(stdio_sync_filebuf<char>::int_type c) {
        return std::ungetc(c, file);
    }

    template <>
    inline stdio_sync_filebuf<char>::int_type
    stdio_sync_filebuf<char>::syncputc(stdio_sync_filebuf<char>::int_type c) {
        return std::putc(c, file);
    }

    template <>
    inline std::streamsize
    stdio_sync_filebuf<char>::xsgetn(char *s, std::streamsize n) {
        std::streamsize res = std::fread(s, 1, n, file);
        lastchar = res > 0 ? traits::to_int_type(s[res - 1]) : traits::eof();
        return res;
    }

    template <>
    inline std::streamsize
    stdio_sync_filebuf<char>::xsputn(const char *s, std::streamsize n) {
        return std::fwrite(s, 1, n, file);
    }

    template <typename CharT, typename Super>
    class stream_mixin : public Super {
public:
        stream_mixin(FILE* f)
            : Super()
            , fbuf(new stdio_sync_filebuf<CharT>(f))
            , file(f)
        {
            std::ios::rdbuf(&*fbuf);
        }

        stream_mixin(const stream_mixin& other) = delete;
        stream_mixin& operator=(const stream_mixin& other) = delete;

        stream_mixin(stream_mixin&& other) :
            fbuf(std::move(other.fbuf)),
            file(std::move(other.file))
        {}

        stream_mixin& operator=(stream_mixin&& other) {
            fbuf = std::move(other.fbuf);
            file = std::move(other.file);
        }

        void close(void) {
            Super::flush();
            Super::close();
            std::fclose(file);
        }

    private:
        std::shared_ptr<stdio_sync_filebuf<CharT>> fbuf;
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

    using ofstream = basic_ofstream<char>;
    using ifstream = basic_ifstream<char>;
    using fstream  = basic_fstream<char>;

    struct get_redirected_out_stream_ {
        static inline ofstream& call(std::FILE* f) {
            static std::unique_ptr<ofstream> stream;

            if (!stream)
                stream.reset(new ofstream(f));
            return *stream;
        }

    };

    struct get_redirected_in_stream_ {
        static inline ifstream& call(std::FILE* f) {
            static std::unique_ptr<ifstream> stream;
            if (!stream)
                stream.reset(new ifstream(f));
            return *stream;
        }
    };

    static inline ofstream& get_redirected_cin(void) {
        return get_redirected_out_stream_::call(cr_get_redirected_stdin());
    }

    static inline ifstream& get_redirected_cout(void) {
        return get_redirected_in_stream_::call(cr_get_redirected_stdout());
    }

    static inline ifstream& get_redirected_cerr(void) {
        return get_redirected_in_stream_::call(cr_get_redirected_stderr());
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
