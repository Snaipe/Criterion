/*
 * The MIT License (MIT)
 *
 * Copyright © 2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CRITERION_INTERNAL_ASSERT_OP_HXX_
#define CRITERION_INTERNAL_ASSERT_OP_HXX_

#include <locale>
#include <clocale>
#include <cwchar>

/* *INDENT-OFF* */
namespace criterion { namespace internal { namespace operators {
/* *INDENT-ON* */

#define CRI_DEFINE_MEMBER_DETECTOR(Member)                                              \
    template <class T>                                                                  \
    class HasMember_ ## Member {                                                        \
    private:                                                                            \
        using yes = char[2];                                                            \
        using no  = char[1];                                                            \
        struct fallback { int Member; };                                                \
        struct derived : T, fallback {};                                                \
        template <class U> static no &test(decltype (U::Member) *);                     \
        template <typename U> static yes &test(U *);                                    \
    public:                                                                             \
        static constexpr bool result = sizeof (test<derived>(nullptr)) == sizeof (yes); \
    };                                                                                  \
    template <class T>                                                                  \
    struct has_member_ ## Member : public std::integral_constant<bool, HasMember_ ## Member<T>::result> {}

CRI_DEFINE_MEMBER_DETECTOR(empty);

template <typename T>
bool zero(const T *t) { return !t; }

template <typename T>
bool zero(const typename std::enable_if<!std::is_pointer<T>::value, T>::type &t) { return !t; }

template <typename T, typename = typename has_member_empty<T>::type>
bool zero(const T &t) { return t.empty(); }

template <> inline bool zero<>(const char *t) { return !*t; }
template <> inline bool zero<>(const wchar_t *t) { return !*t; }

std::ostream &operator<<(std::ostream &s, const std::wstring &ws)
{
    std::mbstate_t state = std::mbstate_t();
    char *curloc = std::setlocale(LC_ALL, NULL);
    const wchar_t *wc = ws.c_str();

    std::setlocale(LC_ALL, "en_US.utf8");
    size_t sz = std::wcsrtombs(NULL, &wc, 0, &state);
    if (sz == (size_t) -1) {
        s << "<invalid multibyte string>";
    } else {
        std::string ns(sz + 1, 0);
        std::wcsrtombs(&ns[0], &wc, ns.size(), &state);
        ns.pop_back();
        s << ns;
    }

    std::setlocale(LC_ALL, curloc);
    return s;
}

/* *INDENT-OFF* */
}}}
/* *INDENT-ON* */

#endif /* !CRITERION_INTERNAL_ASSERT_OP_HXX_ */
