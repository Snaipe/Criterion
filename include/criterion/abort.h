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
/**
 * @file
 * @brief Functions to abort and continue tests
 *****************************************************************************/
#ifndef CRITERION_ABORT_H_
#define CRITERION_ABORT_H_

#include "internal/common.h"

CR_BEGIN_C_API

/**
 *  Aborts the current test, marking it as failed.
 *
 *  This function does not return.
 */
CR_API CR_NORETURN void criterion_abort_test(void);

/**
 *  Aborts the current test, marking it as skipped.
 *
 *  This function does not return.
 */
CR_API CR_NORETURN void criterion_skip_test(const char *format, ...);

/**
 *  Continues the current test.
 *
 *  Used as a counterpart to criterion_abort_test.
 */
CR_API void criterion_continue_test(void);

/**
 *  Kills the current test, marking it as failed.
 *
 *  @param[in] msg printf like format string
 *  @param[in] ... Additional arguments depending on msg
 */
CR_API void criterion_test_die(const char *msg, ...);

CR_END_C_API

#endif /* !CRITERION_ABORT_H_ */
