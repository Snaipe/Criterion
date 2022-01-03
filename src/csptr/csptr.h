/*
 * The MIT License (MIT)
 *
 * Copyright © 2015-2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#ifndef CSPTR_H_
#define CSPTR_H_

/* csptr attribute */

#ifdef __GNUC__

inline __attribute__ ((always_inline))
void sfree_stack(void *ptr)
{
    sfree(*(void **)ptr);
}
# define csptr __attribute__((cleanup(sfree_stack)))

#else
# define csptr \
    CSPTRI_WARN(the csptr attribute is not supported on this compiler.)
#endif

/* Internal stuff */

#define CSPTRI_WARN(Msg)    CSPTRI_WARN_(message(Msg))

#ifdef _MSC_VER
# define CSPTRI_WARN_(Msg)  __pragma(Msg)
#else
# define CSPTRI_WARN_(Msg)  _Pragma(#Msg)
#endif

#ifdef __GNUC__
# define CSPTRI_MALLOC_API  __attribute__ ((malloc))
# define CSPTRI_PURE        __attribute__ ((pure))
#else
# define CSPTRI_MALLOC_API
# define CSPTRI_PURE
#endif

#endif /* !CSPTR_H_ */
