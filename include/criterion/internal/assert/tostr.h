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

extern "C" char *cr_user_str_tostr(const char *);
extern "C" char *cr_user_wcs_tostr(const wchar_t *);

namespace criterion { namespace internal { namespace stream_char_conv {

std::ostream &operator<<(std::ostream &s, const std::string &str)
{
    char *fmt = cr_user_str_tostr(str.c_str());
    std::operator<<(s, fmt);
    free(fmt);
    return s;
}

std::ostream &operator<<(std::ostream &s, const std::wstring &str)
{
    char *fmt = cr_user_wcs_tostr(str.c_str());
    std::operator<<(s, fmt);
    free(fmt);
    return s;
}

/* These overloads are necessary as int8_t and uint8_t are outputted as
   characters by default. why. */

std::ostream &operator<<(std::ostream &s, int8_t i)
{
    s << signed (i);
    return s;
}

std::ostream &operator<<(std::ostream &s, uint8_t i)
{
    s << unsigned (i);
    return s;
}

std::ostream &operator<<(std::ostream &s, char c)
{
    std::operator<<(s, c);
    return s;
}

}}} /* criterion::internal::stream_char_conv */

/* The value escape is only necessary for C++ where some type conversion
   is needed between literals and more complex types of incompatible types. */

# define CRI_VALUE_ESCAPE(T, X) cri_val_escape<std::remove_reference<T>::type>(X)

/* Generic implementation of cri_val_escape */

template <typename T>
constexpr T && cri_val_escape(T && t)
{
    return std::move(t);
}

template <typename T>
constexpr T &cri_val_escape(T &t)
{
    return t;
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
        using namespace criterion::internal::stream_char_conv;        \
        std::stringstream sstr;                                       \
        sstr << (Var);                                                \
                                                                      \
        const std::string str = sstr.str();                           \
        char *out = static_cast<char *>(std::malloc(str.size() + 1)); \
        std::copy(str.begin(), str.end(), out);                       \
        out[str.size()] = '\0';                                       \
        return out;                                                   \
    } ())
# define CRI_ASSERT_UNPRINTABLE(Var) CRI_USER_TOSTR(, Var)
# define CRI_ASSERT_NAMESPACES                      \
    using namespace std::rel_ops;                   \
    using namespace criterion::internal::operators

# define CRI_USER_TOSTR_ARR(Str, Arr, Tag)                                          \
    do {                                                                            \
        std::stringstream cri_sstr;                                                 \
        cri_sstr << "(" CR_STR(CRI_ASSERT_TYPE_TAG(Tag)) "["                        \
             << CRI_ASSERT_TYPE_TAG_ARRLEN(Tag)                                     \
             << "]) {\n";                                                           \
                                                                                    \
        for (size_t cri_i = 0; cri_i < cri_size; ++cri_i) {                         \
            char *cri_repr      = CRI_USER_TOSTR(Tag, &(Arr)[cri_i]);               \
            char *cri_saveptr   = NULL;                                             \
            char *cri_line      = cri_strtok_r(cri_repr, "\n", &cri_saveptr);       \
                                                                                    \
            if (cri_line) {                                                         \
                cri_sstr << "[" << cri_i << "] = " << cri_line;                     \
                                                                                    \
                while ((cri_line = cri_strtok_r(NULL, "\n", &cri_saveptr))) {       \
                    cri_sstr << "\n  " << cri_line;                                 \
                }                                                                   \
                cri_sstr << ",\n";                                                  \
            }                                                                       \
            cr_asprintf_free(cri_repr);                                             \
        }                                                                           \
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
                CR_STR(CRI_ASSERT_TYPE_TAG(Tag)) "["                                    \
                CR_SIZE_T_FORMAT "]) {\n", CRI_ASSERT_TYPE_TAG_ARRLEN(Tag));            \
                                                                                        \
        for (size_t cri_i = 0; cri_i < cri_size; ++cri_i) {                             \
            char *cri_repr      = CRI_USER_TAG_ID(tostr, Tag)(&(Arr)[cri_i]);           \
            char *cri_saveptr   = NULL;                                                 \
            char *cri_line      = cri_strtok_r(cri_repr, "\n", &cri_saveptr);           \
                                                                                        \
            if (cri_line) {                                                             \
                cri_fmt_bprintf(&(Str), &cri_off, &cri_sz,                              \
                        "\t[" CR_SIZE_T_FORMAT "] = %s", cri_i, cri_line);              \
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
