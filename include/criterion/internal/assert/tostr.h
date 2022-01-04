/*
 * The MIT License (MIT)
 *
 * Copyright © 2017 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_INTERNAL_ASSERT_TOSTR_H_
#define CRITERION_INTERNAL_ASSERT_TOSTR_H_

#include "tag.h"

#ifdef __cplusplus

# include <type_traits>
# include <utility>

extern "C" char *cr_user_str_tostr(const char **);
extern "C" char *cr_user_wcs_tostr(const wchar_t **);

namespace criterion { namespace internal { namespace stream_override {

template< class... T>
struct make_void { typedef void type; };

template< class... T>
using __void_t = typename make_void<T...>::type;

template<typename T, typename = void>
struct __is_printable: std::false_type {};

template<typename T>
struct __is_printable<T,
        __void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>>
: std::true_type {};

template<typename T, typename = void>
struct __is_iterable: std::false_type {};

template<typename T>
struct __is_iterable<T,
        __void_t<decltype(std::declval<T>().begin())>>
: std::true_type {};

struct ostream {
    constexpr ostream(std::ostream &s)
        : base(s)
    {}

    std::ostream &base;
};

inline ostream& operator<<(ostream& s, std::ostream& (*func)(std::ostream&))
{
    s.base << func;
    return s;
}

template <typename T,
    typename = typename std::enable_if<!__is_iterable<T>::value || __is_printable<T>::value>::type, typename = void>
inline ostream &operator<<(ostream &s, const T &value)
{
    s.base << value;
    return s;
}

template <typename T, typename U>
inline ostream &operator<<(ostream &s, const std::pair<T, U> &value)
{
    s.base << "[";
    s << value.first;
    s.base << "]: ";
    s << value.second;
    return s;
}

inline ostream &operator<<(ostream &s, const std::string &str)
{
    const char *cstr = str.c_str();
    char *fmt = cr_user_str_tostr(&cstr);
    s.base << fmt;
    free(fmt);
    return s;
}

inline ostream &operator<<(ostream &s, const std::wstring &str)
{
    const wchar_t *cstr = str.c_str();
    char *fmt = cr_user_wcs_tostr(&cstr);
    s.base << fmt;
    free(fmt);
    return s;
}

inline ostream &operator<<(ostream &s, const std::nullptr_t &ptr)
{
    (void) ptr;
    s.base << "nullptr";
    return s;
}

inline ostream &operator<<(ostream &s, void *const &ptr)
{
    if (!ptr)
        return ::criterion::internal::stream_override::operator<<(s, nullptr);

    auto flags = s.base.flags();
    s.base << "@" << std::hex << (uintptr_t)ptr;
    s.base.flags(flags);
    return s;
}

/* These overloads are necessary as int8_t and uint8_t are outputted as
   characters by default. why. */

inline ostream &operator<<(ostream &s, const int8_t &i)
{
    s.base << signed (i);
    return s;
}

inline ostream &operator<<(ostream &s, const uint8_t &i)
{
    s.base << unsigned (i);
    return s;
}

inline ostream &operator<<(ostream &s, const char &c)
{
    s.base << c;
    return s;
}

/* These overloads are there because none of the stl containers implement <<.
   We provide a sensible string representation for iterable types. */

template <typename T,
    typename = typename std::enable_if<__is_iterable<T>::value && !__is_printable<T>::value>::type>
inline ostream &operator<<(ostream &s, const T &container)
{
    s.base << "{";
    size_t count = 0;
    for (auto &e : container) {
        s << std::endl << "\t" << e << ", ";
        count++;
    }
    if (count > 0) {
        s.base << std::endl;
    }
    s.base << "}";
    return s;
}

}}} /* criterion::internal::stream_override */

/* The value escape is only necessary for C++ where some type conversion
   is needed between literals and more complex types of incompatible types. */

# define CRI_VALUE_ESCAPE(T, X) cri_val_escape<std::remove_reference<T>::type>(X)

/* Generic implementation of cri_val_escape */

namespace criterion { namespace internal {

template <class T>
struct is_trivial : std::integral_constant<bool,
    std::is_fundamental<T>::value || std::is_pointer<T>::value> {};

}} /* criterion::internal */

template <typename T,
    typename = typename std::enable_if<!std::is_pointer<T>::value>::type>
constexpr T&& cri_val_escape(T&& v)
{
    return std::forward<T>(v);
}

template <typename T,
    typename = typename std::enable_if<!std::is_pointer<T>::value>::type>
constexpr T& cri_val_escape(T& v)
{
    return v;
}

/* force an array decay */
template <typename T,
    typename = typename std::enable_if<std::is_pointer<T>::value>::type>
constexpr T cri_val_escape(T v)
{
    return v;
}

/* We need these specializations to convert seemingly between string literals
   and STL string types */

template <class Char,
    typename = typename std::enable_if<std::is_same<Char, char>::value>::type>
std::string cri_val_escape(Char const *const &s)
{
    return std::string(s);
}

template <class Char,
    typename = typename std::enable_if<std::is_same<Char, wchar_t>::value>::type>
std::wstring cri_val_escape(Char const *const &s)
{
    return std::wstring(s);
}

template <size_t N>
std::string cri_val_escape(const char (&s)[N])
{
    return std::string(s);
}

template <size_t N>
std::wstring cri_val_escape(const wchar_t (&s)[N])
{
    return std::wstring(s);
}

# define CRI_USER_TOSTR(Tag, Var)                                     \
    ([&]() -> char * {                                                \
        std::stringstream sstr;                                       \
        criterion::internal::stream_override::ostream cri_os { sstr };\
        cri_os << (Var);                                              \
                                                                      \
        const std::string str = sstr.str();                           \
        char *out = static_cast<char *>(std::malloc(str.size() + 1)); \
        std::copy(str.begin(), str.end(), out);                       \
        out[str.size()] = '\0';                                       \
        return out;                                                   \
    } ())
# define CRI_ASSERT_UNPRINTABLE(Var) CRI_USER_TOSTR(, Var)
# define CRI_ASSERT_NAMESPACES                      \
    using namespace criterion::internal::operators

# define CRI_USER_TOSTR_ARR(Str, Arr, Tag)                                          \
    do {                                                                            \
        std::stringstream cri_sstr;                                                 \
        cri_sstr << "(" CR_STR(CRI_ASSERT_TYPE_TAG(Tag)) "["                        \
             << CRI_ASSERT_TYPE_TAG_ARRLEN(Tag)                                     \
             << "]) {\n";                                                           \
                                                                                    \
        for (size_t cri_i = 0; cri_i < cri_size; ++cri_i) {                         \
            char *cri_repr      = CRI_USER_TOSTR(Tag, (Arr)[cri_i]);                \
            char *cri_saveptr   = NULL;                                             \
            char *cri_line      = cri_strtok_r(cri_repr, "\n", &cri_saveptr);       \
                                                                                    \
            if (cri_line) {                                                         \
                cri_sstr << "\t[" << cri_i << "] = " << cri_line;                   \
                                                                                    \
                while ((cri_line = cri_strtok_r(NULL, "\n", &cri_saveptr))) {       \
                    cri_sstr << "\n\t" << cri_line;                                 \
                }                                                                   \
                cri_sstr << ",\n";                                                  \
            }                                                                       \
            cr_asprintf_free(cri_repr);                                             \
        }                                                                           \
        cri_sstr << "}";                                                            \
                                                                                    \
        const std::string cri_ccstr = cri_sstr.str();                               \
        Str = static_cast<char *>(std::malloc(cri_ccstr.size() + 1));               \
        std::copy(cri_ccstr.begin(), cri_ccstr.end(), Str);                         \
        Str[cri_ccstr.size()] = '\0';                                               \
    } while (0)

#else /* !__cplusplus */

# define CRI_VALUE_ESCAPE(T, X) X
# define CRI_USER_TOSTR(Tag, Var) CRI_USER_TAG_ID(tostr, Tag)(&(Var))
# define CRI_ASSERT_UNPRINTABLE(Var) "<unprintable>"
# define CRI_ASSERT_NAMESPACES do {} while (0)

# define CRI_USER_TOSTR_ARR(Str, Arr, Tag)                                              \
    do {                                                                                \
        size_t cri_off = 0;                                                             \
        size_t cri_sz = 0;                                                              \
        cri_fmt_bprintf(&(Str), &cri_off, &cri_sz, "("                                  \
                CR_STR(CRI_ASSERT_TYPE_TAG(Tag)) "[%" CRI_PRIuSIZE "]) {\n",            \
                    CRI_ASSERT_TYPE_TAG_ARRLEN(Tag));                                   \
                                                                                        \
        for (size_t cri_i = 0; cri_i < cri_size; ++cri_i) {                             \
            char *cri_repr      = CRI_USER_TAG_ID(tostr, Tag)(&(Arr)[cri_i]);           \
            char *cri_saveptr   = NULL;                                                 \
            char *cri_line      = cri_strtok_r(cri_repr, "\n", &cri_saveptr);           \
                                                                                        \
            if (cri_line) {                                                             \
                cri_fmt_bprintf(&(Str), &cri_off, &cri_sz,                              \
                        "\t[%" CRI_PRIuSIZE "] = %s", cri_i, cri_line);                 \
                                                                                        \
                while ((cri_line = cri_strtok_r(NULL, "\n", &cri_saveptr))) {           \
                    cri_fmt_bprintf(&(Str), &cri_off, &cri_sz, "\n\t%s", cri_line);     \
                }                                                                       \
                cri_fmt_bprintf(&(Str), &cri_off, &cri_sz, ",\n");                      \
            }                                                                           \
            cr_asprintf_free(cri_repr);                                                 \
        }                                                                               \
        cri_fmt_bprintf(&(Str), &cri_off, &cri_sz, "}");                                \
    } while (0)

#endif /* !__cplusplus */

#endif /* !CRITERION_INTERNAL_ASSERT_TOSTR_H_ */
