/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Matthias "ailu" Günzel <a1lu@arcor.de>
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
struct stc_glob;

typedef int (*match_func)(struct stc_glob const *self, char const *string);
typedef struct stc_glob *(*derive_func)(struct stc_glob const *self, char const chr);
typedef int (*nullable_func)(struct stc_glob const *self);
typedef struct stc_glob *(*copy_func)(struct stc_glob const *cpy);
typedef void (*printFunc)(struct stc_glob const *self);

typedef enum
{ EMPTY = 0, BLANK, CHAR, CHARSET, OR, AND, STAR, PLUS, AT, OPT, SEQ, NOT } glob_type;

typedef struct stc_glob {
    glob_type type;
    match_func matches;
    derive_func derive;
    nullable_func nullable;
    copy_func copy;
#ifdef _DEBUG_BRZ_
    printFunc print;
#endif
    union {
        char c;
        char *cset;
        struct stc_glob const *first;
        struct stc_glob const *pattern;
    };
    union {
        struct stc_glob const *second;
    };
} glob;

void free_glob(glob *tmp);

glob *glob_empty();
glob *glob_blank();
glob *glob_char(char c);
glob *glob_cset(char *c);
glob *glob_or(glob const *first, glob const *second);
glob *glob_and(glob const *first, glob const *second);
glob *glob_star(glob const *pattern);
glob *glob_plus(glob const *pattern);
glob *glob_at(glob const *pattern);
glob *glob_opt(glob const *pattern);
glob *glob_not(glob const *pattern);
glob *glob_seq(glob const *first, glob const *second);
