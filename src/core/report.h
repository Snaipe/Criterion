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
#ifndef REPORT_H_
#define REPORT_H_

#include "criterion/hooks.h"
#include "criterion/options.h"

#define report(Kind, Data)     report_(Kind, Data)
#define report_(Kind, Data)    call_report_hooks_ ## Kind(Data)

#define DECL_CALL_REPORT_HOOKS(Kind) \
    void call_report_hooks_ ## Kind(void *data)

DECL_CALL_REPORT_HOOKS(PRE_ALL);
DECL_CALL_REPORT_HOOKS(PRE_SUITE);
DECL_CALL_REPORT_HOOKS(PRE_INIT);
DECL_CALL_REPORT_HOOKS(PRE_TEST);
DECL_CALL_REPORT_HOOKS(ASSERT);
DECL_CALL_REPORT_HOOKS(THEORY_FAIL);
DECL_CALL_REPORT_HOOKS(TEST_CRASH);
DECL_CALL_REPORT_HOOKS(POST_TEST);
DECL_CALL_REPORT_HOOKS(POST_FINI);
DECL_CALL_REPORT_HOOKS(POST_SUITE);
DECL_CALL_REPORT_HOOKS(POST_ALL);

static inline void nothing() {}

#define log(Type, ...) \
    log_(criterion_options.logger->log_ ## Type, __VA_ARGS__)
#define log_(Log, ...) \
    (Log ? Log(__VA_ARGS__) : nothing())

void cri_report_init(void);
void cri_report_term(void);

#endif /* !REPORT_H_ */
