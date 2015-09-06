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
#ifndef CRITERION_COMMON_H_
# define CRITERION_COMMON_H_

# define CR_EXPAND(x) x

# if defined(_MSC_VER)
#  if _MSC_VER < 1900
#   error \
        Your version of MSVC++ is too old, please compile your tests using  \
        a c99 compiler, like MinGW or MSVC 14.0+ (Included in visual studio \
        2015)
#  endif
# endif

# ifndef CR_IS_MSVC
#  ifdef _MSC_VER
#   define CR_IS_MSVC _MSC_VER
#  else
#   define CR_IS_MSVC 0
#  endif
# endif

# ifdef __APPLE__
#  define SECTION_START_PREFIX       __first
#  define SECTION_END_PREFIX         __last
#  define SECTION_START_SUFFIX(Name) __asm("section$start$__DATA$" Name)
#  define SECTION_END_SUFFIX(Name)   __asm("section$end$__DATA$" Name)
#  define SECTION_(Name)             __attribute__((section("__DATA," Name)))
#  define SECTION_SUFFIX_
# elif CR_IS_MSVC
#  define SECTION_START_PREFIX       __start
#  define SECTION_END_PREFIX         __stop
#  define SECTION_START_SUFFIX(Name)
#  define SECTION_END_SUFFIX(Name)
#  define SECTION_(Name)                    \
    __pragma(data_seg(push))                \
    __pragma(section(Name, read))           \
    __declspec(allocate(Name))
#  define SECTION_SUFFIX_                   \
    __pragma(data_seg(pop))
# else
#  define SECTION_START_PREFIX       __start
#  define SECTION_END_PREFIX         __stop
#  define SECTION_START_SUFFIX(Name)
#  define SECTION_END_SUFFIX(Name)
#  define SECTION_(Name)             __attribute__((section(Name)))
#  define SECTION_SUFFIX_
# endif

# define MAKE_IDENTIFIER_(Prefix, Id) MAKE_IDENTIFIER__(Prefix, Id)
# define MAKE_IDENTIFIER__(Prefix, Id) Prefix ## _ ## Id

# define SECTION_START_(Name) MAKE_IDENTIFIER_(SECTION_START_PREFIX, Name)
# define SECTION_END_(Name)   MAKE_IDENTIFIER_(SECTION_END_PREFIX, Name)

# define SECTION_START(Name)  g_ ## Name ## _section_start
# define SECTION_END(Name)    g_ ## Name ## _section_end

# define DECL_SECTION_LIMITS(Type, Name) DECL_SECTION_LIMITS_(Type, Name)
# define DECL_SECTION_LIMITS_(Type, Name)                            \
    extern Type SECTION_START_(Name) SECTION_START_SUFFIX(#Name);   \
    extern Type SECTION_END_(Name)   SECTION_END_SUFFIX(#Name)

# define IMPL_SECTION_LIMITS(Type, Name)                        \
    Type *const SECTION_START(Name) = &SECTION_START_(Name);    \
    Type *const SECTION_END(Name)   = &SECTION_END_(Name)

# ifdef __GNUC__
#  define UNUSED __attribute__((unused))
#  define NORETURN __attribute__((noreturn))
# elif CR_IS_MSVC
#  define UNUSED
#  define NORETURN __declspec(noreturn)
# else
#  define UNUSED
#  define NORETURN
# endif

# ifdef _WIN32
#  define SIZE_T_FORMAT "%Iu"
# else
#  define SIZE_T_FORMAT "%zu"
# endif

# ifdef __GNUC__
#  define FORMAT(Archetype, Index, Ftc) __attribute__((format(Archetype, Index, Ftc)))
# else
#  define FORMAT(Archetype, Index, Ftc)
# endif

# if defined _WIN32 || defined __CYGWIN__
#  ifdef CRITERION_BUILDING_DLL
#   ifdef __GNUC__
#    define CR_API __attribute__ ((dllexport))
#   else
#    define CR_API __declspec(dllexport)
#   endif
#  else
#   ifdef __GNUC__
#    define CR_API __attribute__ ((dllimport))
#   else
#    define CR_API __declspec(dllimport)
#   endif
#  endif
#  define CR_LOCAL
# else
#  if __GNUC__ >= 4
#   define CR_API __attribute__ ((visibility ("default")))
#   define CR_LOCAL  __attribute__ ((visibility ("hidden")))
#  else
#   define CR_API
#   define CR_LOCAL
#  endif
# endif

#endif /* !CRITERION_COMMON_H_ */
