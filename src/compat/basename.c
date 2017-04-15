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
#include <stdlib.h>
#include <string.h>

#include "basename.h"
#include "config.h"
#include "string/fmt.h"

#if defined (HAVE_GETCWD)
# include <unistd.h>
# include <limits.h>
#elif defined (HAVE_GETCURRENTDIRECTORY)
# include <windows.h>
#endif

const char *basename_compat(const char *str)
{
    const char *start = str;

    for (const char *c = str; *c; ++c)
        if ((*c == '/' || *c == '\\') && c[1])
            start = c + 1;
    return start;
}

char *cri_path_cwd(void)
{
#if defined (HAVE_GETCWD)
    char *cwd = malloc(PATH_MAX);
    if (!cwd || !getcwd(cwd, PATH_MAX))
        return NULL;
#elif defined (HAVE_GETCURRENTDIRECTORY)
    DWORD buflen = GetCurrentDirectoryA(0, NULL);
    if (!buflen)
        return NULL;

    char *cwd = malloc(buflen + 1);
    if (!cwd || !GetCurrentDirectoryA(buflen + 1, cwd))
        return NULL;
#endif
    return cwd;
}

bool cri_path_isrelative(const char *path)
{
#ifdef HAVE_PATHISRELATIVE
    return PathIsRelativeA(path);
#else
    return *path != '/';
#endif
}

char *cri_path_relativeof(const char *cstpath)
{
    char *path = strdup(cstpath);
    if (!path)
        return NULL;
    char *cwd = cri_path_cwd();
    if (!cwd)
        return NULL;

    if (!strcmp(cwd, "/"))
        return path;

    char *rel = NULL;
    size_t off = 0;
    size_t size = 0;

    char *lastdir = cwd + 1;
    char *firstdiff = NULL;
    char *ccwd = cwd;
    char *cpath = path;

    for (; *ccwd && *cpath; ++ccwd, ++cpath) {
        if (*ccwd != *cpath) {
            cri_fmt_bprintf(&rel, &off, &size, "../");

            while (*ccwd && *ccwd != '/' && *ccwd != '\\')
                ++ccwd;
            while (*cpath && *cpath != '/' && *cpath != '\\')
                ++cpath;

            if (!firstdiff)
                firstdiff = lastdir;
            if (!*cpath || !*ccwd)
                break;
        }
        if (*cpath == '/' || *cpath == '\\') {
            lastdir = cpath + 1;
        }
    }
    if (!firstdiff) {
        if (*cpath == '/' || *cpath == '\\') {
            cri_fmt_bprintf(&rel, &off, &size, "./");
        } else {
            while (*cpath != '/' && *cpath != '\\')
                --cpath;
            cri_fmt_bprintf(&rel, &off, &size, "../");
        }
        firstdiff = cpath;
    }

    char *saveptr;
    char *component = cri_strtok_r(firstdiff, "/\\", &saveptr);
    if (component) {
        cri_fmt_bprintf(&rel, &off, &size, "%s", component);
        while ((component = cri_strtok_r(NULL, "/\\", &saveptr))) {
            cri_fmt_bprintf(&rel, &off, &size, "/%s", component);
        }
    }

    free(cwd);
    free(path);

    return rel;
}
