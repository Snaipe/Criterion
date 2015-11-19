#ifndef CRITERION_INTERNAL_STDIO_FILEBUF_HXX_
# define CRITERION_INTERNAL_STDIO_FILEBUF_HXX_

# include <fstream>

namespace criterion { namespace internal {

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

}}

#endif /* !CRITERION_INTERNAL_STDIO_FILEBUF_HXX_ */
