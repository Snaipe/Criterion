/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2007 Russ Cox.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "extglobmatch.h"
#include "brz.h"

#define cat_one()                     \
    do {                              \
        if (atoms > 1) {              \
            --atoms;                  \
            glob *b = *--frag;        \
            glob *a = *--frag;        \
            *frag++ = glob_seq(a, b); \
            free_glob(a);             \
            free_glob(b);             \
        }                             \
    } while (0)

#define cat_all()                     \
    do {                              \
        while (--atoms > 0) {         \
            glob *b = *--frag;        \
            glob *a = *--frag;        \
            *frag++ = glob_seq(a, b); \
            free_glob(a);             \
            free_glob(b);             \
        }                             \
    } while (0)

static glob *match_glob = NULL;

static void cleanup(glob **start, glob **end)
{
    while (start != end) {
        free_glob(*start);
        ++start;
    }
}

static void print_error(char const c)
{
    fprintf(stderr, "pattern error: Unexpected character '%c'\n", c);
}

static glob *handle_operator(char op, glob const *g)
{
    glob *tmp = NULL;

    switch (op) {
        case '!': tmp = glob_not(g);
            break;
        case '*': tmp = glob_star(g);
            break;
        case '+': tmp = glob_plus(g);
            break;
        case '@': tmp = glob_at(g);
            break;
        case '?': tmp = glob_opt(g);
            break;
    }
    assert(tmp);
    return tmp;
}

#define push_frag(f)    *frag ++ = f
#define pop_frag()      *--frag
#define ARRAY_SIZE    100

static glob *compile(char const *pat)
{
    glob *frags[ARRAY_SIZE] = { 0 };
    glob **frag = frags;
    struct paren {
        int alter;
        int atoms;
        char op;
    } paren[ARRAY_SIZE], *pa;

    int alter = 0;
    int atoms = 0;

    pa = paren;

    assert(pat);

    for (;;) {
        unsigned char p = *pat++;
        if (!p)
            break;
        switch (p) {
            case '!':
            case '?':
            case '@':
            case '*':
            case '+':
                if (*pat != '(')
                    goto character;
                if (pa >= paren + ARRAY_SIZE) {
                    fprintf(stderr, "pattern error: to many nested globs.\n");
                    goto error;
                }
                cat_one();
                *pa = (struct paren) {
                    .alter = alter,
                    .atoms = atoms,
                    .op = p
                };
                pa++;
                alter = 0;
                atoms = 0;
                p = *pat++;
                break;

            case '(':
                print_error(p);
                goto error;

            case ')': {
                glob *tmp, *a;
                if (pa == paren || !atoms) {
                    print_error(p);
                    goto error;
                }
                --pa;
                cat_all();
                a = pop_frag();
                tmp = handle_operator(pa->op, a);
                free_glob(a);
                push_frag(tmp);

                for (; alter > 0; --alter) {
                    glob *b = pop_frag();
                    glob *a = pop_frag();
                    if (pa->op == '!')
                        push_frag(glob_and(a, b));
                    else
                        push_frag(glob_or(a, b));
                    --atoms;
                    free_glob(a);
                    free_glob(b);
                }

                atoms = pa->atoms;
                alter = pa->alter;
                atoms++;
            }
            break;

            case '[': {
                char *strPat;
                char const *end;
                cat_one();
                end = strchr(pat, ']');
                if (!end) {
                    fprintf(stderr, "pattern error: Mismatching brackets.\n");
                    goto error;
                }
                strPat = strdup(pat);
                strPat[end - pat] = '\0';
                push_frag(glob_cset(strPat));
                ++atoms;
                free(strPat);
                pat = end + 1;
                break;
            }

            case '|': {
                glob *a;
                glob *tmp;
                char op;
                if (atoms == 0 || pa == paren) {
                    print_error(p);
                    goto error;
                }
                cat_all();
                a = pop_frag();
                op = (pa - 1)->op;
                tmp = handle_operator(op, a);
                free_glob(a);
                push_frag(tmp);

                ++alter;
            }
            break;

            case '\\':
                p = *pat++;
            /* FALLTHROUGH */
            default: /* literal character */
            character:     cat_one();
                push_frag(glob_char(p));
                ++atoms;
        }
    }
    if (pa != paren) {
        fprintf(stderr, "pattern error: Mismatching parenthesis\n");
        goto error;
    }

    cat_all();

    return frags[0];

error:
    cleanup(frags, frag);
    return NULL;
}

int compile_pattern(char const *pattern)
{
    match_glob = compile(pattern);
    return match_glob ? 1 : 0;
}

int match(char const *string)
{
    return match_glob->matches(match_glob, string);
}

void free_pattern()
{
    free_glob(match_glob);
}

#ifdef _DEBUG_BRZ_

void print_pattern()
{
    match_glob->print(match_glob);
}
int main(int argc, char *argv[])
{
    if (argc < 3) {
        printf("usage: %s \"pattern\" \"string\"\n", argv[0]);
        return 1;
    }
    printf("argv: %s\n", argv[1]);
    if (compile_pattern(argv[1])) {
        print_pattern();
        puts("");
        puts("");
        printf("matched: %d\n", match(argv[2]));
        free_pattern();
        return 0;
    }
    return 1;
}
#endif
