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

#ifndef CSPTR_SMART_PTR_H_
# define CSPTR_SMART_PTR_H_

# include <string.h>
# include "common.h"
# include "smalloc.h"

CSPTR_INLINE void sfree_stack(void *ptr) {
    union {
        void **real_ptr;
        void *ptr;
    } conv;
    conv.ptr = ptr;
    sfree(*conv.real_ptr);
    *conv.real_ptr = NULL;
}

# define ARGS_ args.dtor, { args.meta.ptr, args.meta.size }

# define smart __attribute__ ((cleanup(sfree_stack)))
# define smart_ptr(Kind, Type, ...)                                         \
    ({                                                                      \
        struct s_tmp {                                                      \
            CSPTR_SENTINEL_DEC                                              \
            __typeof__(Type) value;                                         \
            f_destructor dtor;                                              \
            struct {                                                        \
                const void *ptr;                                            \
                size_t size;                                                \
            } meta;                                                         \
        } args = {                                                          \
            CSPTR_SENTINEL                                                  \
            __VA_ARGS__                                                     \
        };                                                                  \
        const __typeof__(Type[1]) dummy;                                    \
        void *var = sizeof (dummy[0]) == sizeof (dummy)                     \
            ? smalloc(sizeof (Type), 0, Kind, ARGS_)                        \
            : smalloc(sizeof (dummy[0]),                                    \
                    sizeof (dummy) / sizeof (dummy[0]), Kind, ARGS_);       \
        if (var != NULL)                                                    \
            memcpy(var, &args.value, sizeof (Type));                        \
        var;                                                                \
    })

# define smart_arr(Kind, Type, Length, ...)                                 \
    ({                                                                      \
        struct s_tmp {                                                      \
            CSPTR_SENTINEL_DEC                                              \
            __typeof__(__typeof__(Type)[Length]) value;                     \
            f_destructor dtor;                                              \
            struct {                                                        \
                const void *ptr;                                            \
                size_t size;                                                \
            } meta;                                                         \
        } args = {                                                          \
            CSPTR_SENTINEL                                                  \
            __VA_ARGS__                                                     \
        };                                                                  \
        void *var = smalloc(sizeof (Type), Length, Kind, ARGS_);            \
        if (var != NULL)                                                    \
            memcpy(var, &args.value, sizeof (Type));                        \
        var;                                                                \
    })

# define shared_ptr(Type, ...) smart_ptr(SHARED, Type, __VA_ARGS__)
# define unique_ptr(Type, ...) smart_ptr(UNIQUE, Type, __VA_ARGS__)

# define shared_arr(Type, Length, ...) smart_arr(SHARED, Type, Length, __VA_ARGS__)
# define unique_arr(Type, Length, ...) smart_arr(UNIQUE, Type, Length, __VA_ARGS__)

#endif /* !CSPTR_SMART_PTR_H_ */
