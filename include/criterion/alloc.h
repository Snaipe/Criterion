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
#ifndef CRITERION_ALLOC_H_
# define CRITERION_ALLOC_H_

# ifdef __cplusplus
#  include <memory>
#  include <cstddef>
using std::size_t;
# else
#  include <stddef.h>
# endif
# include "common.h"

CR_BEGIN_C_API

CR_API void *cr_malloc(size_t size);
CR_API void *cr_calloc(size_t nmemb, size_t size);
CR_API void *cr_realloc(void *ptr, size_t size);
CR_API void cr_free(void *ptr);

CR_END_C_API

# ifdef __cplusplus
#  include <type_traits>

namespace criterion {

    void *(*const malloc)(size_t)           = cr_malloc;
    void (*const free)(void *)              = cr_free;
    void *(*const calloc)(size_t, size_t)   = cr_calloc;
    void *(*const realloc)(void *, size_t)  = cr_realloc;

    template<typename T, typename... Params>
    T* new_obj(Params... params) {
        T* obj = static_cast<T*>(cr_malloc(sizeof (T)));
        new (obj) T(params...);
        return obj;
    }

    template<typename T>
    typename std::enable_if<std::is_fundamental<T>::value>::type*
    new_arr(size_t len) {
        void *ptr = cr_malloc(sizeof (size_t) + sizeof (T) * len);
        *(reinterpret_cast<size_t*>(ptr)) = len;
        T* arr = reinterpret_cast<T*>(reinterpret_cast<size_t*>(ptr) + 1);
        return arr;
    }

    template<typename T>
    T* new_arr(size_t len) {
        void *ptr = cr_malloc(sizeof (size_t) + sizeof (T) * len);
        *(reinterpret_cast<size_t*>(ptr)) = len;

        T* arr = reinterpret_cast<T*>(reinterpret_cast<size_t*>(ptr) + 1);
        for (size_t i = 0; i < len; ++i)
            new (arr + i) T();
        return arr;
    }

    template<typename T>
    void delete_obj(T* ptr) {
        ptr->~T();
        cr_free(ptr);
    }

    template<typename T>
    void delete_arr(typename std::enable_if<std::is_fundamental<T>::value>::type* ptr) {
        cr_free(ptr);
    }

    template<typename T>
    void delete_arr(T* ptr) {
        size_t *ptr_ = reinterpret_cast<size_t*>(ptr);
        size_t len = *(ptr_ - 1);
        T* arr = reinterpret_cast<T*>(ptr_);
        for (size_t i = 0; i < len; ++i)
            arr[i].~T();
        cr_free(ptr_ - 1);
    }

    template<typename T>
    struct allocator {
        typedef T value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        template<typename U>
        struct rebind {
            typedef allocator<U> other;
        };

        inline explicit allocator() {}
        inline ~allocator() {}
        inline explicit allocator(allocator const&) {}
        template<typename U>
        inline explicit allocator(allocator<U> const&) {}

        inline pointer address(reference r) { return &r; }
        inline const_pointer address(const_reference r) { return &r; }

        inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0) {
            return reinterpret_cast<pointer>(cr_malloc(cnt * sizeof (T))); 
        }

        inline void deallocate(pointer p, size_type) { cr_free(p); }

        inline size_type max_size() const {
            return size_type(-1) / sizeof(T);
        }

        inline void construct(pointer p, const T& t) { new(p) T(t); }
        inline void construct(pointer p, T&& t) { new (p) T(std::move(t)); }
        inline void destroy(pointer p) { p->~T(); }

        inline bool operator==(allocator const&) { return true; }
        inline bool operator!=(allocator const& a) { return !operator==(a); }
    };

}
# endif

#endif /* !CRITERION_ALLOC_H_ */
