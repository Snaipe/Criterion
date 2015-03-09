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
#ifndef CRITERION_LOGGING_H_
# define CRITERION_LOGGING_H_

# include <stdbool.h>
# include "common.h"

enum criterion_logging_level {
    CRITERION_INFO = 1,
    CRITERION_IMPORTANT,
};

extern enum criterion_logging_level logging_threshold;
extern bool enable_tap_format;

FORMAT(printf, 2, 3)
void criterion_log(enum criterion_logging_level level, const char *msg, ...);

# define criterion_info(...) criterion_log(CRITERION_INFO, __VA_ARGS__)
# define criterion_important(...) criterion_log(CRITERION_IMPORTANT, __VA_ARGS__)

#endif /* !CRITERION_LOGGING_H_ */
