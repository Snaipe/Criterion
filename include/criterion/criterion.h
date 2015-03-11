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
#ifndef CRITERION_H_
# define CRITERION_H_

# include <stdbool.h>
# include <stddef.h>
# include "common.h"
# include "assert.h"

struct criterion_test_extra_data {
    const char *const file_;
    const unsigned line_;
    void (*init)(void);
    void (*fini)(void);
    int signal;
    bool disabled;
    void *data;
};

struct criterion_test {
    const char *name;
    const char *category;
    void (*test)(void);
    struct criterion_test_extra_data *const data;
};

struct criterion_test_set {
    struct criterion_test **tests;
    size_t nb_tests;
};

# define IDENTIFIER_(Category, Name, Suffix) \
    Category ## _ ## Name ## _ ## Suffix
# define TEST_PROTOTYPE_(Category, Name) \
    void IDENTIFIER_(Category, Name, impl)(void)

# define Test(Category, Name, ...)                                             \
    TEST_PROTOTYPE_(Category, Name);                                           \
    struct criterion_test_extra_data IDENTIFIER_(Category, Name, extra) = {    \
        .file_ = __FILE__,                                                     \
        .line_ = __LINE__,                                                     \
        __VA_ARGS__                                                            \
    };                                                                         \
    SECTION_("criterion_tests")                                                \
    const struct criterion_test IDENTIFIER_(Category, Name, meta) = {          \
        .name     = #Name,                                                     \
        .category = #Category,                                                 \
        .test     = IDENTIFIER_(Category, Name, impl),                         \
        .data     = &IDENTIFIER_(Category, Name, extra)                        \
    };                                                                         \
    TEST_PROTOTYPE_(Category, Name)

int criterion_run_all_tests(void);

#endif /* !CRITERION_H_ */
