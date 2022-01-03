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

#ifndef CSPTR_ARRAY_H_
# define CSPTR_ARRAY_H_

/**
 * @page csptr
 */
///@{

# include "common.h"
# include "smart_ptr.h"

typedef struct {
    size_t nmemb;
    size_t size;
} s_meta_array;

/**
 * @page csptr
 * Gets the length of the specified smart array.
 *
 * @param[in] ptr: a valid pointer to a smart array.
 * @returns the length of the smart array.
 */
CSPTR_PURE size_t array_length(void *ptr);

/**
 * Gets the size of the subscript type of the specified smart array.
 *
 * @param[in] ptr: a valid pointer to a smart array.
 * @returns the size in bytes of the subscript type.
 */
CSPTR_PURE size_t array_type_size(void *ptr);

/**
 * Gets the user metadata of the specified smart array.
 *
 * @param[in] ptr: a valid pointer to a smart array.
 * @returns a pointer to the user metadata structure.
 */
CSPTR_PURE void *array_user_meta(void *ptr);

///}@

#endif /* !CSPTR_ARRAY_H_ */
