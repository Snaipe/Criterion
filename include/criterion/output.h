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
#ifndef CRITERION_OUTPUT_H_
# define CRITERION_OUTPUT_H_

# include "stats.h"

typedef void criterion_reporter(FILE *stream, struct criterion_global_stats *);

/**
 *  Register an output provider.
 *
 *  @param[in] name The name the output provider shall be registered as.
 *  @param[in] reporter The output reporting function.
 *  @returns 1 if no output provider is registered at that name, 0 otherwise,
 *    and -1 on error.
 */
int criterion_register_output_provider(const char *name, criterion_reporter *reporter);

/**
 *  Use an output provider to write a report in a specific path.
 *
 *  @param[in] provider The name of a registered output provider.
 *  @param[in] path The path to the file to write the report to.
 *  @returns -1 on error.
 */
int criterion_add_output(const char *provider, const char *path);

#endif /* !CRITERION_OUTPUT_H_ */
