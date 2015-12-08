#ifndef CRITERION_INTERNAL_STREAM_HXX_
# define CRITERION_INTERNAL_STREAM_HXX_

# include <fstream>
# include <cstdio>
# include <memory>

# include "stdio_filebuf.hxx"

namespace criterion { namespace internal {

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

# if __cplusplus > 199711L
        stream_mixin(const stream_mixin& other) = delete;
        stream_mixin& operator=(const stream_mixin& other) = delete;
# endif

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
    class basic_ofstream : public stream_mixin<CharT, std::basic_ofstream<CharT>> {
        typedef stream_mixin<CharT, std::basic_ofstream<CharT>> super;
    public:
        basic_ofstream(FILE* f)
            : super(f)
        {}

        basic_ofstream(basic_ofstream&& other)
            : super(std::move(other))
        {}
    };

    template <typename CharT>
    class basic_ifstream : public stream_mixin<CharT, std::basic_ifstream<CharT>> {
        typedef stream_mixin<CharT, std::basic_ifstream<CharT>> super;
    public:
        basic_ifstream(FILE* f)
            : super(f)
        {}

        basic_ifstream(basic_ifstream&& other)
            : super(std::move(other))
        {}
    };

    template <typename CharT>
    class basic_fstream : public stream_mixin<CharT, std::basic_fstream<CharT>> {
        typedef stream_mixin<CharT, std::basic_fstream<CharT>> super;
    public:
        basic_fstream(FILE* f)
            : super(f)
        {}

        basic_fstream(basic_fstream&& other)
            : super(std::move(other))
        {}
    };

    struct get_redirected_out_stream_ {
        static inline basic_ofstream<char>& call(std::FILE* f) {
            static std::unique_ptr<basic_ofstream<char>> stream;

            if (!stream)
                stream.reset(new basic_ofstream<char>(f));
            return *stream;
        }

    };

    struct get_redirected_in_stream_ {
        static inline basic_ifstream<char>& call(std::FILE* f) {
            static std::unique_ptr<basic_ifstream<char>> stream;
            if (!stream)
                stream.reset(new basic_ifstream<char>(f));
            return *stream;
        }
    };

}}

#endif /* !CRITERION_INTERNAL_STREAM_HXX_ */
