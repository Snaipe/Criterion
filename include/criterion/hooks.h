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
#ifndef CRITERION_HOOKS_H_
# define CRITERION_HOOKS_H_

# include "common.h"

typedef enum {
    PRE_ALL,
    PRE_SUITE,
    PRE_INIT,
    PRE_TEST,
    ASSERT,
    THEORY_FAIL,
    TEST_CRASH,
    POST_TEST,
    POST_FINI,
    POST_SUITE,
    POST_ALL,
} e_report_status;

typedef void (*f_report_hook)();

# define HOOK_IDENTIFIER_(Suffix) HOOK_IDENTIFIER__(__LINE__, Suffix)
# define HOOK_IDENTIFIER__(Line, Suffix) HOOK_IDENTIFIER___(Line, Suffix)
# define HOOK_IDENTIFIER___(Line, Suffix) hook_l ## Line ## _ ## Suffix

# define HOOK_PROTOTYPE_                                                       \
    void HOOK_IDENTIFIER_(impl)

// Section abbreviations
# define HOOK_SECTION_PRE_ALL       cr_pra
# define HOOK_SECTION_PRE_SUITE     cr_prs
# define HOOK_SECTION_PRE_INIT      cr_pri
# define HOOK_SECTION_PRE_TEST      cr_prt
# define HOOK_SECTION_ASSERT        cr_ast
# define HOOK_SECTION_THEORY_FAIL   cr_thf
# define HOOK_SECTION_TEST_CRASH    cr_tsc
# define HOOK_SECTION_POST_TEST     cr_pot
# define HOOK_SECTION_POST_FINI     cr_pof
# define HOOK_SECTION_POST_SUITE    cr_pos
# define HOOK_SECTION_POST_ALL      cr_poa

# define HOOK_SECTION(Kind) HOOK_SECTION_ ## Kind

# define HOOK_SECTION_STRINGIFY__(Sec) #Sec
# define HOOK_SECTION_STRINGIFY_(Sec) HOOK_SECTION_STRINGIFY__(Sec)
# define HOOK_SECTION_STRINGIFY(Kind) HOOK_SECTION_STRINGIFY_(HOOK_SECTION(Kind))

# define ReportHook(Kind)                                                      \
    HOOK_PROTOTYPE_();                                                         \
    SECTION_(HOOK_SECTION_STRINGIFY(Kind))                                     \
    const f_report_hook HOOK_IDENTIFIER_(func) = HOOK_IDENTIFIER_(impl);       \
    HOOK_PROTOTYPE_

#endif /* !CRITERION_HOOKS_H_ */
