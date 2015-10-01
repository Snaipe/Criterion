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

# include "designated-initializer-compat.h"
# include "common.h"
# include "types.h"
# include "assert.h"
# include "alloc.h"

# define IDENTIFIER_(Category, Name, Suffix) \
    Category ## _ ## Name ## _ ## Suffix

# ifdef __cplusplus
#  define TEST_PROTOTYPE_(Category, Name) \
    extern "C" void IDENTIFIER_(Category, Name, impl)(void)
#  define CR_LANG CR_LANG_CPP
# else
#  define TEST_PROTOTYPE_(Category, Name) \
    void IDENTIFIER_(Category, Name, impl)(void)
#  define CR_LANG CR_LANG_C
# endif

# define SUITE_IDENTIFIER_(Name, Suffix) \
    suite_ ## Name ## _ ## Suffix

# define Test(...) CR_EXPAND(Test_(__VA_ARGS__, .sentinel_ = 0))
# define Test_(Category, Name, ...)                                            \
    TEST_PROTOTYPE_(Category, Name);                                           \
    struct criterion_test_extra_data IDENTIFIER_(Category, Name, extra) =      \
        CR_EXPAND(CRITERION_MAKE_STRUCT(struct criterion_test_extra_data,      \
            .lang_ = CR_LANG,                                                  \
            .kind_ = CR_TEST_NORMAL,                                           \
            .param_ = (struct criterion_test_params(*)(void)) NULL,            \
            .identifier_ = #Category "/" #Name,                                \
            .file_    = __FILE__,                                              \
            .line_    = __LINE__,                                              \
            __VA_ARGS__                                                        \
        ));                                                                    \
    struct criterion_test IDENTIFIER_(Category, Name, meta) = {                \
        #Name,                                                                 \
        #Category,                                                             \
        IDENTIFIER_(Category, Name, impl),                                     \
        &IDENTIFIER_(Category, Name, extra)                                    \
    };                                                                         \
    SECTION_("cr_tst")                                                         \
    struct criterion_test *IDENTIFIER_(Category, Name, ptr)                    \
            = &IDENTIFIER_(Category, Name, meta) SECTION_SUFFIX_;              \
    TEST_PROTOTYPE_(Category, Name)

# define TestSuite(...) CR_EXPAND(TestSuite_(__VA_ARGS__, .sentinel_ = 0))
# define TestSuite_(Name, ...)                                                 \
    struct criterion_test_extra_data SUITE_IDENTIFIER_(Name, extra) =          \
        CR_EXPAND(CRITERION_MAKE_STRUCT(struct criterion_test_extra_data,      \
            .file_    = __FILE__,                                              \
            .line_    = 0,                                                     \
            __VA_ARGS__                                                        \
        ));                                                                    \
    struct criterion_suite SUITE_IDENTIFIER_(Name, meta) = {                   \
        #Name,                                                                 \
        &SUITE_IDENTIFIER_(Name, extra),                                       \
    };                                                                         \
    SECTION_("cr_sts")                                                         \
    struct criterion_suite *SUITE_IDENTIFIER_(Name, ptr)                       \
	    = &SUITE_IDENTIFIER_(Name, meta) SECTION_SUFFIX_

CR_BEGIN_C_API

CR_API struct criterion_test_set *criterion_initialize(void);
CR_API void criterion_finalize(struct criterion_test_set *tests);
CR_API int criterion_run_all_tests(struct criterion_test_set *tests);
CR_API int criterion_handle_args(int argc, char *argv[], bool handle_unknown_arg);
CR_API void criterion_register_test(struct criterion_test_set *tests,
                                    struct criterion_test *test);

CR_END_C_API

#endif /* !CRITERION_H_ */
