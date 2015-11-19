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
#ifndef CRITERION_INTERNAL_HOOKS_H_
# define CRITERION_INTERNAL_HOOKS_H_

# include "common.h"
# include "../types.h"

# define CR_HOOK_IDENTIFIER_(Suffix) CR_HOOK_IDENTIFIER__(__LINE__, Suffix)
# define CR_HOOK_IDENTIFIER__(Line, Suffix) CR_HOOK_IDENTIFIER___(Line, Suffix)
# define CR_HOOK_IDENTIFIER___(Line, Suffix) hook_l ## Line ## _ ## Suffix

# ifdef __cplusplus
#  define CR_HOOK_PROTOTYPE_ \
    extern "C" void CR_HOOK_IDENTIFIER_(impl)
# else
#  define CR_HOOK_PROTOTYPE_ \
    void CR_HOOK_IDENTIFIER_(impl)
# endif

// Section abbreviations
# define CR_HOOK_SECTION_PRE_ALL       cr_pra
# define CR_HOOK_SECTION_PRE_SUITE     cr_prs
# define CR_HOOK_SECTION_PRE_INIT      cr_pri
# define CR_HOOK_SECTION_PRE_TEST      cr_prt
# define CR_HOOK_SECTION_ASSERT        cr_ast
# define CR_HOOK_SECTION_THEORY_FAIL   cr_thf
# define CR_HOOK_SECTION_TEST_CRASH    cr_tsc
# define CR_HOOK_SECTION_POST_TEST     cr_pot
# define CR_HOOK_SECTION_POST_FINI     cr_pof
# define CR_HOOK_SECTION_POST_SUITE    cr_pos
# define CR_HOOK_SECTION_POST_ALL      cr_poa

# define CR_HOOK_SECTION(Kind) CR_HOOK_SECTION_ ## Kind

# define CR_HOOK_SECTION_STRINGIFY__(Sec) #Sec
# define CR_HOOK_SECTION_STRINGIFY_(Sec) CR_HOOK_SECTION_STRINGIFY__(Sec)
# define CR_HOOK_SECTION_STRINGIFY(Kind) CR_HOOK_SECTION_STRINGIFY_(CR_HOOK_SECTION(Kind))

# define CR_HOOK_PARAM_TYPE_PRE_ALL        struct criterion_test_set *
# define CR_HOOK_PARAM_TYPE_PRE_SUITE      struct criterion_suite_set *
# define CR_HOOK_PARAM_TYPE_PRE_INIT       struct criterion_test *
# define CR_HOOK_PARAM_TYPE_PRE_TEST       struct criterion_test *
# define CR_HOOK_PARAM_TYPE_ASSERT         struct criterion_assert_stats *
# define CR_HOOK_PARAM_TYPE_THEORY_FAIL    struct criterion_theory_stats *
# define CR_HOOK_PARAM_TYPE_TEST_CRASH     struct criterion_test_stats *
# define CR_HOOK_PARAM_TYPE_POST_TEST      struct criterion_test_stats *
# define CR_HOOK_PARAM_TYPE_POST_FINI      struct criterion_test_stats *
# define CR_HOOK_PARAM_TYPE_POST_SUITE     struct criterion_suite_stats *
# define CR_HOOK_PARAM_TYPE_POST_ALL       struct criterion_global_stats *

# define CR_HOOK_PARAM_TYPE(Kind) CR_HOOK_PARAM_TYPE_ ## Kind

# define CR_REPORT_HOOK_IMPL(Kind)                  \
    CR_HOOK_PROTOTYPE_(CR_HOOK_PARAM_TYPE(Kind));   \
    CR_SECTION_(CR_HOOK_SECTION_STRINGIFY(Kind))    \
    f_report_hook CR_HOOK_IDENTIFIER_(func) =       \
        (f_report_hook) CR_HOOK_IDENTIFIER_(impl)   \
    CR_SECTION_SUFFIX_;                             \
    CR_HOOK_PROTOTYPE_


#endif /* !CRITERION_INTERNAL_HOOKS_H_ */
