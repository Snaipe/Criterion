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
#ifndef CRITERION_RUNNER_H_
# define CRITERION_RUNNER_H_

# include "criterion/types.h"
# include "compat/pipe.h"

CR_DECL_SECTION_LIMITS(struct criterion_test*, cr_tst);
CR_DECL_SECTION_LIMITS(struct criterion_suite*, cr_sts);

struct criterion_test_set *criterion_init(void);
void run_test_child(struct criterion_test *test, struct criterion_suite *suite);

# define FOREACH_TEST_SEC(Test)                                         \
    for (struct criterion_test **Test = GET_SECTION_START(cr_tst);      \
            Test < (struct criterion_test**) GET_SECTION_END(cr_tst);   \
            ++Test)

# define FOREACH_SUITE_SEC(Suite)                                       \
    for (struct criterion_suite **Suite = GET_SECTION_START(cr_sts);    \
            Suite < (struct criterion_suite**) GET_SECTION_END(cr_sts); \
            ++Suite)

void run_single_test_by_name(const char *testname);

#endif /* !CRITERION_RUNNER_H_ */
