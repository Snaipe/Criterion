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
#include <stddef.h>
#include <string.h>
#include <pcre.h>
#include <setjmp.h>

#include <stdio.h>
#include "criterion/internal/common.h"
#include "common.h"

struct context {
    int depth;
    char *dst;
    const char *src;
    char old, cur;
    int eos;
    const char **errmsg;
    jmp_buf *jmp;
};

void transform_impl(struct context *ctx);

static inline void transform_rec(struct context *ctx) {
    struct context new_ctx = {
        .depth = ctx->depth + 1,
        .dst = ctx->dst,
        .src = ctx->src,
        .old = ctx->old,
        .eos = ctx->eos,
        .errmsg = ctx->errmsg,
        .jmp = ctx->jmp,
    };
    transform_impl(&new_ctx);
    ctx->dst = new_ctx.dst;
    ctx->src = new_ctx.src;
    ctx->old = new_ctx.old;
    ctx->eos = new_ctx.eos;
}

static inline char read_char(struct context *ctx) {
    char c = *ctx->src;
    ctx->old = ctx->cur;
    ctx->cur = c;
    if (c == '\0')
        ctx->eos = 1;
    ++ctx->src;
    return c;
}

static inline char peek_char(struct context *ctx) {
    return *ctx->src;
}

static inline void copy_char(struct context *ctx, char src) {
    *ctx->dst = src;
    ++ctx->dst;
}

static inline void dup_char(struct context *ctx) {
    copy_char(ctx, read_char(ctx));
}

static inline void copy_str(struct context *ctx, const char *src) {
    size_t len = strlen(src);
    strncpy(ctx->dst, src, len);
    ctx->dst += len;
}

#define PREPREFIX   0
#define POSTPREFIX  1
#define PRESUFFIX   2
#define POSTSUFFIX  3
#define ELSESTR     4

typedef struct {
    int (*validator)(struct context *ctx);
    char *str;
} handler_arg;

static int active(CR_UNUSED struct context *ctx)   { return 1; }
static int inactive(CR_UNUSED struct context *ctx) { return 0; }

static int is_eos(struct context *ctx) {
    return peek_char(ctx) == '\0';
}

static inline void handle_special(struct context *ctx, handler_arg strs[5]) {
    if (peek_char(ctx) == '(') {
        if (DEF(strs[0].validator, inactive)(ctx))
            copy_str(ctx, strs[0].str);
        dup_char(ctx);
        if (DEF(strs[1].validator, inactive)(ctx))
            copy_str(ctx, strs[1].str);

        transform_rec(ctx);

        if (DEF(strs[2].validator, inactive)(ctx))
            copy_str(ctx,strs[2].str);
        copy_char(ctx, ')');
        if (DEF(strs[3].validator, inactive)(ctx))
            copy_str(ctx, strs[3].str);
    } else if (DEF(strs[4].validator, inactive)(ctx)) {
        copy_str(ctx, strs[4].str);
    }
}

# define Handler(Name, ...)                                             \
        static void Name(struct context *ctx, CR_UNUSED char c) {          \
            handle_special(ctx, (handler_arg[5]) {                      \
                __VA_ARGS__                                             \
            });                                                         \
        }

# define _ active
Handler(handle_plus, [POSTSUFFIX] = {_, "+"}, [ELSESTR] = {_, "+" })
Handler(handle_star, [POSTSUFFIX] = {_, "*"}, [ELSESTR] = {_, ".*"})
Handler(handle_wild, [POSTSUFFIX] = {_, "?"}, [ELSESTR] = {_, "." })
Handler(handle_excl,
        [POSTPREFIX] = {_,      "?!"},
        [PRESUFFIX]  = {is_eos, "$" },
        [POSTSUFFIX] = {_,      ".*"},
        [ELSESTR]    = {_,      "!" }
    )
Handler(handle_at, [ELSESTR] = {_, "@"})
# undef _

static void handle_rbra(struct context *ctx, CR_UNUSED char c) {
    copy_char(ctx, c);
    if (peek_char(ctx) == '!') {
        read_char(ctx);
        copy_char(ctx, '^');
    }
}

static void escape_char(struct context *ctx, char c) {
    copy_char(ctx, '\\');
    copy_char(ctx, c);
}

static void escape_pipe(struct context *ctx, CR_UNUSED char c) {
    if (ctx->depth == 0)
        copy_char(ctx, '\\');
    copy_char(ctx, '|');
}

typedef void (*f_handler)(struct context *, char);

void transform_impl(struct context *ctx) {
    static f_handler handlers[] = {
        ['+'] = handle_plus,
        ['*'] = handle_star,
        ['?'] = handle_wild,
        ['!'] = handle_excl,
        ['['] = handle_rbra,
        ['@'] = handle_at,

        ['.'] = escape_char,
        ['('] = escape_char,
        [')'] = escape_char,
        ['|'] = escape_pipe,

        [255] = NULL,
    };
    for (char c = read_char(ctx); !ctx->eos; c = read_char(ctx)) {
        f_handler handler = handlers[(unsigned char) c];

        if (ctx->old == '\\')
            handler = copy_char;

        if (c == ')' && ctx->depth > 0)
            return;

        (handler ? handler : copy_char)(ctx, c);

        if (ctx->eos)
            return;
    }
    if (ctx->depth > 0) {
        *ctx->errmsg = "mismatching parenthesis";
        longjmp(*ctx->jmp, -1); // abort operation
    }
}

static int transform(const char *pattern, char *result, const char **errmsg) {
    jmp_buf jmp;
    struct context ctx = {
        .src = pattern,
        .dst = result,
        .errmsg = errmsg,
        .jmp = &jmp,
    };
    if (!setjmp(*ctx.jmp)) {
        copy_char(&ctx, '^');
        transform_impl(&ctx);
        copy_char(&ctx, '$');
        copy_char(&ctx, '\0');
        return 0;
    }
    return -1;
}

/*
 * let T be the transformation function,
 * let diff be the function that yields the greatest character difference
 * before and after its parameter has been transformed by T.
 *
 * T('*')   = '.*';                     diff('*') = 1
 * T('!()') = '(?!).*' or '(?!$).*';    diff('!()') = 4
 * T('@')   = '' or '@';                diff('@') = 0
 * T('|')   = '|' or '\|';              diff('|') = 1
 * T('.')   = '\.';                     diff('.') = 1
 * T('(')   = '\(';                     diff('(') = 1
 * T(')')   = '\)';                     diff(')') = 1
 * for every other 1 character string s, we have T(s) = s; hence diff(s) = 0
 *
 * let num be the function that yields the number of occurences of a string.
 * let spec be the set {(s, num(s)) | ∀s}
 * ∀s, length(T(s)) = length(s) + Σ((c_i, n_i) ∈ spec, n_i * diff(c_i))
 *
 * let S = {'*', '!()', '|', '.', '(', ')'}.
 * since ∀s ∉ S, diff(s) = 0, we can simplify the above equation as:
 *
 * ∀s, length(T(s)) = length(s) + num('*') + num('|') + num('.')
 *                              + num('(') + num(')') + 4 * num('!()').
 *
 * We must now find the maximal length L such as ∀s, L >= length(T(s))
 *
 * It is immediately apparent that the largest string will depend on the number
 * of occurrences of '!()'. Hence, ∀s, let u(s) be a string that is a repeating
 * sequence of '!()' padded by at most two '.', such as length(u(s)) == length(s),
 *
 * let N = floor(length(u(s)) / 3),
 * let Q = length(u(s)) mod 3,
 * hence num('!()') = N.
 *
 * ∀s | length(s) = length(u(s)),
 *     length(T(s)) <= length(T(u(s)))
 *                  <= length(u(s))         | the original length
 *                      + 4 * N             | the expansion of all '!()'
 *                      + Q * diff('.')     | the expansion of Q '.'
 *                  <= 3 * N + Q + 4 * N + Q
 *                  <= 7 * N + 4
 *                  <= 7 * floor(length(u(s)) / 3) + 4
 *                  <= 7 * floor(length(s) / 3) + 4
 *
 */
static inline size_t max_length(size_t len) {
    return 7 * len / 3 + 4;
}

int extmatch(const char *pattern, const char *string, const char **errmsg) {
    char regex[max_length(strlen(pattern)) + 1];
    if (transform(pattern, regex, errmsg) != -1) {
        int erroffset;
        pcre *preg = pcre_compile(regex, 0, errmsg, &erroffset, NULL);
        if (preg) {
            int res = pcre_exec(preg, NULL, string, strlen(string), 0, 0, NULL, 0);
            pcre_free(preg);
            return res;
        }
    }
    return -10;
}
