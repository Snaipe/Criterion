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
#ifndef CRITERION_PARAMETERIZED_H_
# define CRITERION_PARAMETERIZED_H_

# include "alloc.h"
# include "assert.h"
# include "internal/parameterized.h"

# define ParameterizedTest(...) CR_EXPAND(CR_PARAM_TEST_BASE(__VA_ARGS__, .sentinel_ = 0))

# define ParameterizedTestParameters(Category, Name) CR_PARAM_TEST_PARAMS(Category, Name)

# ifdef __cplusplus
#  include <vector>

namespace criterion {
    template <typename T>
    using parameters = std::vector<T, criterion::allocator<T>>;
}
# endif

#endif /* !CRITERION_PARAMETERIZED_H_ */
