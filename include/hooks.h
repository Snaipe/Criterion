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

#include "common.h"

typedef enum {
    PRE_EVERYTHING,
    PRE_INIT,
    PRE_TEST,
    POST_TEST,
    POST_FINI,
    POST_EVERYTHING,
} e_report_status;

typedef void (*f_report_hook)(void *);

# define HOOK_IDENTIFIER_(Suffix) HOOK_IDENTIFIER__(__LINE__, Suffix)
# define HOOK_IDENTIFIER__(Line, Suffix) HOOK_IDENTIFIER___(Line, Suffix)
# define HOOK_IDENTIFIER___(Line, Suffix) hook_l ## Line ## _ ## Suffix

# define HOOK_PROTOTYPE_                                                       \
    void HOOK_IDENTIFIER_(impl)(UNUSED void *data)

# define ReportHook(Kind)                                                      \
    HOOK_PROTOTYPE_;                                                           \
    SECTION_("criterion_hooks_" #Kind)                                         \
    const f_report_hook HOOK_IDENTIFIER_(func) = HOOK_IDENTIFIER_(impl);      \
    HOOK_PROTOTYPE_

#endif /* !CRITERION_HOOKS_H_ */
