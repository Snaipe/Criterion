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
#ifndef COMPAT_PATH_H_
#define COMPAT_PATH_H_

#include <stddef.h>
#include <stdbool.h>

const char *basename_compat(const char *str);

char *cri_path_cwd(void);
bool cri_path_isrelative(const char *path);
bool cri_path_isdirectory(const char *path);
bool cri_path_exists(const char *path);
char *cri_path_relativeof(const char *path);
size_t cri_path_combine(char **output, size_t *size, const char *path, ...);
char *cri_path_gen_unique_filename(const char *path, const char *filename, const char *extension);

size_t cri_path_build(char separator, char **output, size_t *size, const char *path, ...);

#endif /* !COMPAT_PATH_H_ */
