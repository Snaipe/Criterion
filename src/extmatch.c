#include <stddef.h>
#include <string.h>
#include <pcre.h>

#include <stdio.h>
#include "criterion/common.h"

struct context {
    int depth;
    char *dst;
    const char *src;
    char old, cur;
    int eos;
};

void transform_impl(struct context *ctx);

static inline void transform_rec(struct context *ctx) {
    struct context new_ctx = {
        .depth = ctx->depth + 1,
        .dst = ctx->dst,
        .src = ctx->src,
        .old = ctx->old,
        .eos = ctx->eos,
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

static int active()   { return 1; }
static int inactive() { return 0; }

static int is_eos(struct context *ctx) {
    return peek_char(ctx) == '\0';
}

static inline void handle_special(struct context *ctx, handler_arg strs[5]) {
    if (peek_char(ctx) == '(') {
        if ((strs[0].validator ?: inactive)(ctx))
            copy_str(ctx, strs[0].str);
        dup_char(ctx);
        if ((strs[1].validator ?: inactive)(ctx))
            copy_str(ctx, strs[1].str);

        transform_rec(ctx);

        if ((strs[2].validator ?: inactive)(ctx))
            copy_str(ctx,strs[2].str);
        copy_char(ctx, ')');
        if ((strs[3].validator ?: inactive)(ctx))
            copy_str(ctx, strs[3].str);
    } else if ((strs[4].validator ?: inactive)(ctx)) {
        copy_str(ctx, strs[4].str);
    }
}

# define Handler(Name, ...)                                             \
        static void Name(struct context *ctx, UNUSED char c) {          \
            handle_special(ctx, (handler_arg[5]) {                      \
                __VA_ARGS__                                             \
            });                                                         \
        }

# define _ active
Handler(handle_plus, [POSTSUFFIX] = {_, "+"}, [ELSESTR] = {_, "+" });
Handler(handle_star, [POSTSUFFIX] = {_, "*"}, [ELSESTR] = {_, ".*"});
Handler(handle_wild, [POSTSUFFIX] = {_, "?"}, [ELSESTR] = {_, "." });
Handler(handle_excl,
        [POSTPREFIX] = {_,      "?!"},
        [PRESUFFIX]  = {is_eos, "$" },
        [POSTSUFFIX] = {_,      ".*"},
        [ELSESTR]    = {_,      "!" }
    );
Handler(handle_at, [ELSESTR] = {_, "@"});
# undef _

static void handle_rbra(struct context *ctx, UNUSED char c) {
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

static void escape_pipe(struct context *ctx, UNUSED char c) {
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
    };
    for (char c = read_char(ctx); !ctx->eos; c = read_char(ctx)) {
        f_handler handler = handlers[(unsigned char) c];

        if (ctx->old == '\\')
            handler = copy_char;

        if (c == ')' && ctx->depth > 0)
            return;

        (handler ?: copy_char)(ctx, c);

        if (ctx->eos)
            return;
    }
    if (ctx->depth > 0) {
        puts("pattern error: mismatching parenthesis");
        exit(1);
    }
}

static void transform(const char *pattern, char *result) {
    struct context ctx = {
        .src = pattern,
        .dst = result,
    };
    copy_char(&ctx, '^');
    transform_impl(&ctx);
    copy_char(&ctx, '$');
    copy_char(&ctx, '\0');
}

int extmatch(const char *pattern, const char *string) {
    char regex[strlen(pattern) * 2];
    transform(pattern, regex);

    const char *errmsg;
    int erroffset;
    pcre *preg = pcre_compile(regex, 0, &errmsg, &erroffset, NULL);
    if (preg) {
        int res = pcre_exec(preg, NULL, string, strlen(string), 0, 0, NULL, 0);
        pcre_free(preg);
        return res;
    }
    printf("pattern error: %s\n", errmsg);
    exit(1);
}
