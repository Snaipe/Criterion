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
#ifndef SECTION_H_
# define SECTION_H_

# include "criterion/common.h"

# ifdef _WIN32
void *get_win_section_start(const char *section);
void *get_win_section_end(const char *section);
#  define CR_STRINGIFY_(Param) #Param
#  define CR_STRINGIFY(Param) CR_STRINGIFY_(Param)
#  define GET_SECTION_START(Name) get_win_section_start(CR_STRINGIFY(Name))
#  define GET_SECTION_END(Name)   get_win_section_end(CR_STRINGIFY(Name))
# elif defined(__APPLE__)
void *get_osx_section_start(const char *section);
void *get_osx_section_end(const char *section);
#  define CR_STRINGIFY_(Param) #Param
#  define CR_STRINGIFY(Param) CR_STRINGIFY_(Param)
#  define GET_SECTION_START(Name) get_osx_section_start(CR_STRINGIFY(Name))
#  define GET_SECTION_END(Name)   get_osx_section_end(CR_STRINGIFY(Name))
# else
#  define GET_SECTION_START(Name) CR_SECTION_START(Name)
#  define GET_SECTION_END(Name)   CR_SECTION_END(Name)
# endif

#endif /* !SECTION_H_ */
