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
# include "internal/common.h"

CR_BEGIN_C_API

/**
 *  Allocates a block of memory usable by the test.
 *
 *  It is undefined behaviour to access a pointer returned by malloc(3)
 *  inside a test or its setup and teardown functions; cr_malloc must
 *  be use in its place for this purpose.
 *
 *  This function is semantically identical to malloc(3).
 *
 *  @param[in] size The minimal size in bytes of the newly allocated memory.
 *  @returns The pointer to the start of the allocated memory.
 */
CR_API void *cr_malloc(size_t size);

/**
 *  Allocates and zero-initialize a block of memory usable by the test.
 *
 *  It is undefined behaviour to access a pointer returned by calloc(3)
 *  inside a test or its setup and teardown functions; cr_calloc must
 *  be use in its place for this purpose.
 *
 *  This function is semantically identical to calloc(3).
 *
 *  @param[in] nmemb The number of elements to allocate
 *  @param[in] size The minimal size of each element.
 *  @returns The pointer to the start of the allocated memory.
 */
CR_API void *cr_calloc(size_t nmemb, size_t size);

/**
 *  Reallocates a block of memory usable by the test.
 *
 *  It is undefined behaviour to access a pointer returned by realloc(3)
 *  inside a test or its setup and teardown functions; cr_realloc must
 *  be used in its place for this purpose.
 *
 *  This function is semantically identical to realloc(3).
 *
 *  @param[in] ptr A pointer to the memory that needs to be resized.
 *  @param[in] size The minimal size of the reallocated memory.
 *  @returns The pointer to the start of the reallocated memory.
 */
CR_API void *cr_realloc(void *ptr, size_t size);

/**
 *  Free a block of memory allocated by cr_malloc, cr_free or cr_realloc.
 *
 *  @param[in] ptr A pointer to the memory that needs to be freed.
 */
CR_API void cr_free(void *ptr);

CR_END_C_API

# ifdef __cplusplus
#  include <type_traits>

namespace criterion {

    void *(*const malloc)(size_t)           = cr_malloc;
    void (*const free)(void *)              = cr_free;
    void *(*const calloc)(size_t, size_t)   = cr_calloc;
    void *(*const realloc)(void *, size_t)  = cr_realloc;

    /**
     *  Allocates and construct a new object.
     *
     *  It is undefined behaviour to access a pointer returned by the new
     *  operator inside a test or its setup and teardown functions;
     *  new_obj must be used in its place for this purpose.
     *
     *  This function is semantically identical to the new operator.
     *
     *  @tparam T The type of the object to construct
     *  @param[in] params The constructor parameters of T.
     *  @returns The pointer to the newly constructed object.
     */
    template<typename T, typename... Params>
    T* new_obj(Params... params) {
        T* obj = static_cast<T*>(cr_malloc(sizeof (T)));
        new (obj) T(params...);
        return obj;
    }

    /**
     *  Allocates and construct a new array of primitive types
     *
     *  It is undefined behaviour to access a pointer returned by the new[]
     *  operator inside a test or its setup and teardown functions;
     *  new_arr must be used in its place for this purpose.
     *
     *  This function is semantically identical to the new[] operator.
     *
     *  @tparam T The compound type of the array to construct
     *  @param[in] len The length of the array.
     *  @returns The pointer to the newly constructed array.
     */
    template<typename T>
    typename std::enable_if<std::is_fundamental<T>::value>::type*
    new_arr(size_t len) {
        void *ptr = cr_malloc(sizeof (size_t) + sizeof (T) * len);
        *(reinterpret_cast<size_t*>(ptr)) = len;
        T* arr = reinterpret_cast<T*>(reinterpret_cast<size_t*>(ptr) + 1);
        return arr;
    }

    /**
     *  Allocates and construct a new array of object types
     *
     *  It is undefined behaviour to access a pointer returned by the new[]
     *  operator inside a test or its setup and teardown functions;
     *  new_arr must be used in its place for this purpose.
     *
     *  This function is semantically identical to the new[] operator.
     *
     *  @tparam T The compound type of the array to construct
     *  @param[in] len The length of the array.
     *  @returns The pointer to the newly constructed array.
     */
    template<typename T>
    T* new_arr(size_t len) {
        void *ptr = cr_malloc(sizeof (size_t) + sizeof (T) * len);
        *(reinterpret_cast<size_t*>(ptr)) = len;

        T* arr = reinterpret_cast<T*>(reinterpret_cast<size_t*>(ptr) + 1);
        for (size_t i = 0; i < len; ++i)
            new (arr + i) T();
        return arr;
    }

    /**
     *  Destroys and frees an object allocated by new_obj.
     *
     *  This function is semantically identical to the delete operator.
     *
     *  @tparam T The type of the object to construct
     *  @param[in] ptr The object to destroy.
     */
    template<typename T>
    void delete_obj(T* ptr) {
        ptr->~T();
        cr_free(ptr);
    }

    /**
     *  Destroys and frees an array allocated by delete_arr.
     *
     *  This function is semantically identical to the delete[] operator.
     *
     *  @tparam T The type of the object to construct
     *  @param[in] ptr The object to destroy.
     */
    template<typename T>
    void delete_arr(typename std::enable_if<std::is_fundamental<T>::value>::type* ptr) {
        cr_free(ptr);
    }

    /**
     *  Destroys and frees an array allocated by delete_arr.
     *
     *  This function is semantically identical to the delete[] operator.
     *
     *  @tparam T The type of the object to construct
     *  @param[in] ptr The object to destroy.
     */
    template<typename T>
    void delete_arr(T* ptr) {
        size_t *ptr_ = reinterpret_cast<size_t*>(ptr);
        size_t len = *(ptr_ - 1);
        T* arr = reinterpret_cast<T*>(ptr_);
        for (size_t i = 0; i < len; ++i)
            arr[i].~T();
        cr_free(ptr_ - 1);
    }

    /**
     *  Allocator for use in the STL.
     *
     *  This internally uses calls to the cr_malloc function family, which
     *  means that STL collections can be safely used inside tests or
     *  setup/teardown functions if this allocator is used.
     */
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
