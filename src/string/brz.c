/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 George Spelvim <linux@horizon.com>
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
#include "brz.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define copy_glob(g)           (g)->copy(g)
#define derive_glob(g, chr)    (g)->derive(g, chr)

static int matches(glob const *self, char const *string);

static int is_true(glob const *self);
static int is_false(glob const *self);
static int nullable_char(glob const *self);
static int nullable_or(glob const *self);
static int nullable_seq(glob const *self);
static int nullable_first(glob const *self);
static int nullable_not(glob const *self);

static glob *derive_empty(glob const *self, char const chr);
static glob *derive_blank(glob const *self, char const chr);
static glob *derive_char(glob const *self, char const chr);
static glob *derive_cset(glob const *self, char const chr);
static glob *derive_or(glob const *self, char const chr);
static glob *derive_seq(glob const *self, char const chr);
static glob *derive_plus(glob const *self, char const chr);
static glob *derive_at(glob const *self, char const chr);
static glob *derive_not(glob const *self, char const chr);
static glob *derive_and(glob const *self, char const chr);

static glob *copy_zero(glob const *cpy);
static glob *copy_one(glob const *cpy);
static glob *copy_two(glob const *cpy);

#ifdef _DEBUG_BRZ_
# define print_glob_v(g)    do { printf(#g " "); (g)->print(g), puts(""); } while (0)
# define print_glob(g)      do { (g)->print(g); } while (0)
static void print_empty(glob const *self);
static void print_blank(glob const *self);
static void print_char(glob const *self);
static void print_cset(glob const *self);
static void print_or(glob const *self);
static void print_and(glob const *self);
static void print_star(glob const *self);
static void print_plus(glob const *self);
static void print_at(glob const *self);
static void print_opt(glob const *self);
static void print_seq(glob const *self);
static void print_not(glob const *self);
#endif

void free_glob(glob *tmp)
{
    glob *first = (glob *) tmp->first;
    glob *second = (glob *) tmp->second;

    if (tmp->type != CHAR && tmp->type != CHARSET && first)
        free_glob(first);
    else if (tmp->type == CHARSET)
        free(tmp->cset);
    if (second)
        free_glob(second);
    free(tmp);
}

static glob *new_glob()
{
    glob *tmp = malloc(sizeof (glob));

    if (tmp) {
        tmp->derive = NULL;
        tmp->nullable = NULL;
        tmp->first = NULL;
        tmp->second = NULL;
        tmp->matches = matches;
        tmp->type = 0;
    } else {
        fprintf(stderr, "Could not allocate glob object.\n");
        exit(1);
    }
    return tmp;
}

static int matches(glob const *self, char const *string)
{
    if (strlen(string) == 0) {
        return self->nullable(self);
    } else {
        int ret;
#ifdef _DEBUG_BRZ_
        printf("\nstring: ");
        puts(string);
#endif
        glob *tmp = derive_glob(self, *string);
#ifdef _DEBUG_BRZ_
        print_glob_v(tmp);
#endif
        ret = tmp->matches(tmp, ++string);
        free_glob(tmp);
        return ret;
    }
}

/*
 * Empty Set
 * not nullable
 * no derivative
 */
glob *glob_empty()
{
    glob *tmp = new_glob();

    tmp->type = EMPTY;
    tmp->nullable = is_false;
    tmp->derive = derive_empty;
#ifdef _DEBUG_BRZ_
    tmp->print = print_empty;
#endif
    tmp->copy = copy_zero;

    return tmp;
}

/*
 * Empty string
 * is nullable
 * derivative is Empty set
 */
glob *glob_blank()
{
    glob *tmp = new_glob();

    tmp->type = BLANK;
    tmp->nullable = is_true;
    tmp->derive = derive_blank;
#ifdef _DEBUG_BRZ_
    tmp->print = print_blank;
#endif
    tmp->copy = copy_zero;

    return tmp;
}

/*
 * single chacacter
 * nullable for ? and *
 * derivative: Empty string if chr == c
 *             Empty set otherwise
 */
glob *glob_char(char const c)
{
    glob *tmp = new_glob();

    tmp->type = CHAR;
    tmp->nullable = nullable_char;
    tmp->derive = derive_char;
    tmp->c = c;
#ifdef _DEBUG_BRZ_
    tmp->print = print_char;
#endif
    tmp->copy = copy_zero;

    return tmp;
}

/*
 * single character
 * not nullable
 * derivative: Empty string if chr in [..] or not in [!..]
 *             Empty set otherwise
 */
glob *glob_cset(char *cset)
{
    glob *tmp = new_glob();

    tmp->type = CHARSET;
    tmp->nullable = is_false;
    tmp->derive = derive_cset;
    tmp->cset = strdup(cset);
#ifdef _DEBUG_BRZ_
    tmp->print = print_cset;
#endif
    tmp->copy = copy_one;

    return tmp;
}

/*
 * Choice: E | F
 * nullable: nullable (E) | nullable(F)
 * derivative: der(E) | der(F)
 *
 * Optimization:
 * (1) E | Empty -> E
 */
glob *glob_or(glob const *first, glob const *second)
{
    if (first->type == EMPTY) {
        return copy_glob(second);
    } else if (second->type == EMPTY) {
        return copy_glob(first);
    } else {
        glob *tmp = new_glob();
        tmp->type = OR;
        tmp->nullable = nullable_or;
        tmp->derive = derive_or;

        tmp->first = copy_glob(first);
        tmp->second = copy_glob(second);
#ifdef _DEBUG_BRZ_
        tmp->print = print_or;
#endif
        tmp->copy = copy_two;

        return tmp;
    }
}

/*
 * Intersection: E & F
 * nullable: nullable (E) & nullable(F)
 * derivative: der(E) & der(F)
 *
 * Optimization:
 * (1) E & Empty -> Empty
 */
glob *glob_and(glob const *first, glob const *second)
{
    if (first->type == EMPTY || second->type == EMPTY) {
        return glob_empty();
    } else {
        glob *tmp = new_glob();
        tmp->type = AND;
        tmp->nullable = nullable_seq;
        tmp->derive = derive_and;

        tmp->first = copy_glob(first);
        tmp->second = copy_glob(second);
#ifdef _DEBUG_BRZ_
        tmp->print = print_and;
#endif
        tmp->copy = copy_two;

        return tmp;
    }
}

glob *glob_star(glob const *pattern)
{
    glob *tmp = new_glob();

    tmp->type = STAR;
    tmp->nullable = is_true;
    tmp->derive = derive_plus;

    tmp->first = copy_glob(pattern);
#ifdef _DEBUG_BRZ_
    tmp->print = print_star;
#endif
    tmp->copy = copy_one;

    return tmp;
}

/*
 * Repetition one or more
 * +(pat)
 * nullable: nullable(pat)
 * derivative: der(pat) Star(pat)
 *
 */
glob *glob_plus(glob const *pattern)
{
    glob *tmp = glob_star(pattern);

    tmp->type = PLUS;
    tmp->nullable = nullable_first;
#ifdef _DEBUG_BRZ_
    tmp->print = print_plus;
#endif
    return tmp;
}

/*
 * Repetition one time
 * @(pat)
 * nullable: nullable(pat)
 * derivative: der(pat)
 *
 */
glob *glob_at(glob const *pattern)
{
    if (pattern->type == EMPTY) {
        return glob_empty();
    } else if (pattern->type == BLANK) {
        return glob_blank();
    } else {
        glob *tmp = glob_opt(pattern);
        tmp->type = AT;
        tmp->nullable = nullable_first;

#ifdef _DEBUG_BRZ_
        tmp->print = print_at;
#endif
        return tmp;
    }
}

/*
 * Repetition one or zero
 * ?(pat)
 * nullable: true
 * derivative: der(pat)
 *
 */
glob *glob_opt(glob const *pattern)
{
    glob *tmp = new_glob();

    tmp->type = OPT;
    tmp->nullable = is_true;
    tmp->derive = derive_at;

    tmp->first = copy_glob(pattern);
#ifdef _DEBUG_BRZ_
    tmp->print = print_opt;
#endif
    tmp->copy = copy_one;

    return tmp;
}

/*
 * Not the pattern
 * !(pat)
 * nullable: not nullable(pat)
 * derivative: not der(pat)
 *
 */
glob *glob_not(glob const *pattern)
{
    glob *tmp = new_glob();

    tmp->type = NOT;
    tmp->nullable = nullable_not;
    tmp->derive = derive_not;

    tmp->first = copy_glob(pattern);
#ifdef _DEBUG_BRZ_
    tmp->print = print_not;
#endif
    tmp->copy = copy_one;

    return tmp;
}

/*
 * Sequence E F
 * nullable: nullable(E) nullable(F)
 * derivative: nullable(E): Or( Seq(der(E), F), der(F) )
 *                  else: Seq( der(E), F )
 * Optimization:
 * (1) Empty E -> Empty
 * (2) E Empty -> Empty
 * (3) Blank E -> E
 */
glob *glob_seq(glob const *first, glob const *second)
{
    if (first->type == EMPTY || second->type == EMPTY) {
        return glob_empty();
    } else if (first->type == BLANK) {
        return copy_glob(second);
    } else {
        glob *tmp = new_glob();
        tmp->type = SEQ;
        tmp->nullable = nullable_seq;
        tmp->derive = derive_seq;

        tmp->first = copy_glob(first);
        tmp->second = copy_glob(second);
#ifdef _DEBUG_BRZ_
        tmp->print = print_seq;
#endif
        tmp->copy = copy_two;
        return tmp;
    }
}

static int is_false(glob const *self)
{
    (void) (self);
    return 0;
}

static int is_true(glob const *self)
{
    (void) (self);
    return 1;
}

static int nullable_char(glob const *self)
{
    if (self->c == '?' || self->c == '*')
        return 1;
    else
        return 0;
}

static int nullable_or(glob const *self)
{
    glob const *first, *second;

    first = self->first;
    second = self->second;
    return first->nullable(first) || second->nullable(second);
}

static int nullable_seq(glob const *self)
{
    glob const *first, *second;

    first = self->first;
    second = self->second;
    return first->nullable(first) && second->nullable(second);
}

static int nullable_first(glob const *self)
{
    glob const *first;

    first = self->first;
    return first->nullable(first);
}

static int nullable_not(glob const *self)
{
    return !nullable_first(self);
}

static glob *derive_empty(glob const *self, char const chr)
{
    (void) (chr);
    (void) (self);
    return glob_empty();
}

static glob *derive_blank(glob const *self, char const chr)
{
    (void) (self);
    (void) (chr);
    return glob_empty();
}

static glob *derive_char(glob const *self, char const chr)
{
    if (self->c == chr || self->c == '?') {
        return glob_blank();
    } else if (self->c == '*') {
        glob *ret = self->copy(self);
        return ret;
    }
    return glob_empty();
}

static glob *derive_cset(glob const *self, char const chr)
{
    /* Character class */
    char *pat = self->cset;
    int match = 0, inverted = (*pat == '!');
    char const *class = pat + inverted;
    unsigned char a = *class++;

    /*
     * Iterate over each span in the character class.
     * A span is either a single character a, or a
     * range a-b.  The first span may begin with ']'.
     */
    do {
        unsigned char b = a;

        if (a == '\0') /* Malformed */
            return glob_empty();

        if (class[0] == '-' && class[1] != ']') {
            b = class[1];

            if (b == '\0') /* malformed */
                return glob_empty();

            class += 2;
        }
        if (a > b) {
            unsigned char tmp = a;
            a = b;
            b = tmp;
        }
        match |= (a <= chr && chr <= b);
    } while ((a = *class++) && !match);

    if (match == inverted)
        return glob_empty();
    else
        return glob_blank();
}

static glob *derive_and(glob const *self, char const chr)
{
    glob const *first, *second;

    first = self->first;
    second = self->second;
    glob *d1 = derive_glob(first, chr);
    glob *d2 = derive_glob(second, chr);
    glob *ret;
    ret = glob_and(d1, d2);
    free_glob(d1);
    free_glob(d2);
    return ret;
}

static glob *derive_or(glob const *self, char const chr)
{
    glob const *first, *second;

    first = self->first;
    second = self->second;
    glob *d1 = derive_glob(first, chr);
    glob *d2 = derive_glob(second, chr);
    glob *ret = glob_or(d1, d2);
    free_glob(d1);
    free_glob(d2);
    return ret;
}

static glob *derive_plus(glob const *self, char const chr)
{
    glob *ret;
    glob const *pat;

    pat = self->pattern;
    glob *d = derive_glob(pat, chr);
    glob *s = glob_star(pat);

    ret = glob_seq(d, s);
    free_glob(s);
    free_glob(d);
    return ret;
}

static glob *derive_seq(glob const *self, char const chr)
{
    glob *ret;
    glob const *first, *second;

    first = self->first;
    second = self->second;
    if (first->nullable(first)) { /* A nullable -> Or( Seq(d1, sec), d2) */
        glob *d1 = derive_glob(first, chr);
        glob *d2 = derive_glob(second, chr);
        glob *s = glob_seq(d1, second);
        ret = glob_or(s, d2);
        free_glob(s);
        free_glob(d1);
        free_glob(d2);
    } else { /* A not nullable */
        glob *derive = derive_glob(first, chr);
        ret = glob_seq(derive, second);
        free_glob(derive);
    }

    return ret;
}

static glob *derive_at(glob const *self, char const chr)
{
    glob const *pat;

    pat = self->pattern;
    glob *d = derive_glob(pat, chr);
    glob *ret;
    ret = glob_at(d);
    free_glob(d);
    return ret;
}

static glob *derive_not(glob const *self, char const chr)
{
    glob const *pat;

    pat = self->pattern;
    glob *d = derive_glob(pat, chr);
    glob *ret = glob_not(d);
    free_glob(d);
    return ret;
}

static glob *copy_zero(glob const *cpy)
{
    glob *tmp = new_glob();

    *tmp = *cpy;
    return tmp;
}

static glob *copy_one(glob const *cpy)
{
    glob const *first = cpy->first;
    glob *tmp = new_glob();

    *tmp = *cpy;
    if (cpy->type != CHARSET)
        tmp->first = copy_glob(first);
    else
        tmp->cset = strdup(cpy->cset);
    return tmp;
}

static glob *copy_two(glob const *cpy)
{
    glob const *first = cpy->first;
    glob const *sec = cpy->second;
    glob *tmp = new_glob();

    *tmp = *cpy;
    tmp->first = copy_glob(first);
    tmp->second = copy_glob(sec);
    return tmp;
}

#ifdef _DEBUG_BRZ_

static void print_empty(glob const *self)
{
    (void) (self);
    printf("Empty");
}

static void print_blank(glob const *self)
{
    (void) (self);
    printf("Blank");
}
static void print_char(glob const *self)
{
    printf("%c", self->c);
}

static void print_cset(glob const *self)
{
    printf("[%s]", self->cset);
}

static void print_or(glob const *self)
{
    glob const *first = self->first;
    glob const *sec = self->second;

    printf("(");
    print_glob(first);
    printf(" | ");
    print_glob(sec);
    printf(")");
}
static void print_and(glob const *self)
{
    glob const *first = self->first;
    glob const *sec = self->second;

    printf("(");
    print_glob(first);
    printf(" & ");
    print_glob(sec);
    printf(")");
}

static void print_seq(glob const *self)
{
    glob const *first = self->first;
    glob const *sec = self->second;

    print_glob(first);
    printf(" . ");
    print_glob(sec);
}

static void print_star(glob const *self)
{
    glob const *first = self->first;

    printf("*(");
    print_glob(first);
    printf(")");
}
static void print_plus(glob const *self)
{
    glob const *first = self->first;

    printf("+(");
    print_glob(first);
    printf(")");
}
static void print_at(glob const *self)
{
    glob const *first = self->first;

    printf("@(");
    print_glob(first);
    printf(")");
}
static void print_opt(glob const *self)
{
    glob const *first = self->first;

    printf("?(");
    print_glob(first);
    printf(")");
}
static void print_not(glob const *self)
{
    glob const *first = self->first;

    printf("!(");
    print_glob(first);
    printf(")");
}

#endif
