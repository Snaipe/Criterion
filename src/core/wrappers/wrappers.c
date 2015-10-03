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
#include "wrap.h"

f_wrapper *g_wrappers[CR_LANG_SIZE_] = {
    [CR_LANG_C]         = c_wrap,

#ifdef CXX_BRIDGE
    [CR_LANG_CXX]       = cxx_wrap,
#endif

#ifdef OBJC_BRIDGE
    [CR_LANG_OBJC]      = objc_wrap,
    [CR_LANG_OBJCXX]    = objc_wrap,
#endif
};

const char *const cr_language_names[CR_LANG_SIZE_] = {
    [CR_LANG_C]         = "C",
    [CR_LANG_CXX]       = "C++",
    [CR_LANG_OBJC]      = "Objective-C",
    [CR_LANG_OBJCXX]    = "Objective-C++",
};

