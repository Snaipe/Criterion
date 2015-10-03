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
#ifndef CRITERION_TYPES_H_
# define CRITERION_TYPES_H_

# include "alloc.h"
# ifdef __cplusplus
#  include <cstddef>
#  include <vector>
using std::size_t;
# else
#  include <stdbool.h>
#  include <stddef.h>
# endif
# include "common.h"

enum criterion_language {
    CR_LANG_C,
    CR_LANG_CXX,
    CR_LANG_OBJC,
    CR_LANG_OBJCXX,

    CR_LANG_SIZE_ // leave this at the end
};

extern const char *const cr_language_names[CR_LANG_SIZE_];

enum criterion_test_kind {
    CR_TEST_NORMAL,
    CR_TEST_PARAMETERIZED,
};

struct criterion_test_params {
    size_t size;
    void *params;
    size_t length;
    void (*cleanup)(struct criterion_test_params *);

# ifdef __cplusplus
    constexpr criterion_test_params(size_t size, void *params, size_t length)
        : size(size)
        , params(params)
        , length(length)
        , cleanup(nullptr)
    {}

    constexpr criterion_test_params(size_t size, void *params, size_t length,
            void (*cleanup)(struct criterion_test_params *))
        : size(size)
        , params(params)
        , length(length)
        , cleanup(cleanup)
    {}

    template <typename T>
    constexpr criterion_test_params(std::vector<T, criterion::allocator<T>>& vec, 
            void (*cleanup)(criterion_test_params *) = nullptr)
        : size(sizeof (T))
        , params(&vec[0])
        , length(vec.size())
        , cleanup(cleanup)
    {}

    template <typename T, unsigned int N>
    constexpr criterion_test_params(T (&arr)[N],
            void (*cleanup)(criterion_test_params *) = nullptr)
        : size(sizeof (arr[0]))
        , params(static_cast<void*>(&arr))
        , length(N)
        , cleanup(cleanup)
    {}
# endif
};

struct criterion_test_extra_data {
    int sentinel_;
    enum criterion_language lang_;
    enum criterion_test_kind kind_;
    struct criterion_test_params (*param_)(void);
    const char *identifier_;
    const char *file_;
    unsigned line_;
    void (*init)(void);
    void (*fini)(void);
    int signal;
    int exit_code;
    bool disabled;
    const char *description;
    double timeout;
    void *data;
};

struct criterion_test {
    const char *name;
    const char *category;
    void (*test)(void);
    struct criterion_test_extra_data *data;
};

struct criterion_suite {
    const char *name;
    struct criterion_test_extra_data *data;
};

struct criterion_ordered_set;

struct criterion_suite_set {
    struct criterion_suite suite;
    struct criterion_ordered_set *tests;
};

struct criterion_test_set {
    struct criterion_ordered_set *suites;
    size_t tests;
};

typedef void (*cr_worker_func)(struct criterion_test *, struct criterion_suite *);

#endif /* !CRITERION_TYPES_H_ */
