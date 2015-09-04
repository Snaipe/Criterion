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
#include <stdbool.h>
#include <stdarg.h>
#include <setjmp.h>
#include <dyncall.h>
#include <assert.h>
#include <limits.h>
#include "criterion/theories.h"
#include "abort.h"

struct criterion_theory_context {
    DCCallVM* vm;
};

void cr_theory_push_arg(struct criterion_theory_context *ctx, bool is_float, size_t size, void *ptr) {
    if (is_float) {
        if (size == sizeof (float)) {
            dcArgFloat(ctx->vm, *(float*)ptr);
        } else if (size == sizeof (double)) {
            dcArgDouble(ctx->vm, *(double*)ptr);
        } else if (size == sizeof (long double)) {
            dcArgDouble(ctx->vm, *(long double*)ptr);
        }
    } else {
        if (size == sizeof (char)) {
            dcArgChar(ctx->vm, *(char*)ptr);
        } else if (size == sizeof (short)) {
            dcArgShort(ctx->vm, *(short*)ptr);
        } else if (size == sizeof (int)) {
            dcArgInt(ctx->vm, *(int*)ptr);
        } else if (size == sizeof (bool)) {
            dcArgBool(ctx->vm, *(bool*)ptr);
#if INT_MAX < LONG_MAX
        } else if (size == sizeof (long)) {
            dcArgLong(ctx->vm, *(long*)ptr);
#endif
#if LONG_MAX < LLONG_MAX
        } else if (size == sizeof (long long)) {
            dcArgLongLong(ctx->vm, *(long long*)ptr);
#endif
        } else if (size == sizeof (void*)) {
            dcArgPointer(ctx->vm, *(void**)ptr);
        } else {
            dcArgPointer(ctx->vm, ptr);
        }
    }
}

struct criterion_theory_context* cr_theory_init(void) {
    struct criterion_theory_context* ctx = malloc(sizeof (struct criterion_theory_context));
    ctx->vm = dcNewCallVM(4096);
    dcMode(ctx->vm, DC_CALL_C_DEFAULT);
    return ctx;
}

void cr_theory_free(struct criterion_theory_context *ctx) {
    dcFree(ctx->vm);
}

static jmp_buf theory_jmp;

void cr_theory_abort(void) {
    longjmp(theory_jmp, 1);
}

int cr_theory_mark(void) {
    return setjmp(theory_jmp);
}

void cr_theory_reset(struct criterion_theory_context *ctx) {
    dcReset(ctx->vm);
}

void cr_theory_call(struct criterion_theory_context *ctx, void (*fnptr)(void)) {
    dcCallVoid(ctx->vm, (DCpointer) fnptr);
}

static bool contains_word(const char *str, const char *pattern, size_t sz) {
    char *res = strstr(str, pattern);

    return res
        && (res == str || (res > str && res[-1] == ' '))
        && (!res[sz - 1] || res[sz - 1] == ' ');
}

static bool is_string(const char *name) {
    return !strcmp(name, "char*")
        || !strcmp(name, "char *")
        || !strcmp(name, "const char*")
        || !strcmp(name, "const char *")
        || !strcmp(name, "char const *")
        || !strcmp(name, "char const*")
        || !strcmp(name, "char[]")
        || !strcmp(name, "char []")
        || !strcmp(name, "const char[]")
        || !strcmp(name, "const char []")
        || !strcmp(name, "char const[]")
        || !strcmp(name, "char const []");
}

static bool is_float(const char *name) {
    return contains_word(name, "float", sizeof ("float"))
        || contains_word(name, "double", sizeof ("double"));
}

static bool is_unsigned_int(const char *name) {
    return contains_word(name, "unsigned", sizeof ("unsigned"))
        || !strncmp(name, "uint", 4);
}

static bool is_bool(const char *name) {
    return contains_word(name, "bool", sizeof ("bool"))
        || contains_word(name, "_Bool", sizeof ("_Bool"));
}

static void format_arg(char (*arg)[1024], struct criterion_datapoints *dp, void *data) {
    if (is_float(dp->name)) {
        if (dp->size == sizeof (float)) {
            snprintf(*arg, sizeof (*arg) - 1, "%gf", *(float*) data);
        } else if (dp->size == sizeof (double)) {
            snprintf(*arg, sizeof (*arg) - 1, "%g", *(double*) data);
        } else if (dp->size == sizeof (long double)) {
            snprintf(*arg, sizeof (*arg) - 1, "%gl", (double) *(long double*) data);
        }
    } else {
        if (is_string(dp->name)) {
            snprintf(*arg, sizeof (*arg) - 1, "\"%s\"", *(char**) data);
        } else if (dp->size == sizeof (bool) && is_bool(dp->name)) {
            snprintf(*arg, sizeof (*arg) - 1, "%s", (*(bool*) data) ? "true" : "false");
        } else if (dp->size == sizeof (char)) {
            snprintf(*arg, sizeof (*arg) - 1, "'%c'", *(char*) data);
        } else if (dp->size == sizeof (short)) {
            const char *fmt = is_unsigned_int(dp->name) ? "%hu" : "%hd";
            snprintf(*arg, sizeof (*arg) - 1, fmt, *(short*) data);
        } else if (dp->size == sizeof (int)) {
            const char *fmt = is_unsigned_int(dp->name) ? "%u" : "%d";
            snprintf(*arg, sizeof (*arg) - 1, fmt, *(int*) data);
        } else if (dp->size == sizeof (void*) && strstr(dp->name, "*")) {
            snprintf(*arg, sizeof (*arg) - 1, "%p", *(void**) data);
#if INT_MAX < LONG_MAX
        } else if (dp->size == sizeof (long)) {
            const char *fmt = is_unsigned_int(dp->name) ? "%lulu" : "%ldl";
            snprintf(*arg, sizeof (*arg) - 1, fmt, *(long*) data);
#endif
#if LONG_MAX < LLONG_MAX
        } else if (dp->size == sizeof (long long)) {
            const char *fmt = is_unsigned_int(dp->name) ? "%llullu" : "%lldll";
            snprintf(*arg, sizeof (*arg) - 1, fmt, *(long long*) data);
#endif
        } else {
            snprintf(*arg, sizeof (*arg) - 1, "%s", "<np>");
        }
    }
}

static void concat_arg(char (*msg)[4096], struct criterion_datapoints *dps, size_t *indices, size_t i) {
    void *data = ((char*) dps[i].arr) + dps[i].size * indices[i];

    char arg[1024];
    format_arg(&arg, dps + i, data);
    strncat(*msg, arg, sizeof (*msg) - 1);
}

void cr_theory_main(struct criterion_datapoints *dps, size_t datapoints, void (*fnptr)(void)) {
    struct criterion_theory_context *ctx = cr_theory_init();

    size_t indices[datapoints];
    memset(indices, 0, datapoints * sizeof (size_t));

    bool has_next = true;
    while (has_next) {
        if (!cr_theory_mark()) {
            cr_theory_reset(ctx);
            for (size_t i = 0; i < datapoints; ++i) {

                bool is_float = contains_word(dps[i].name, "float", sizeof ("float"))
                    || contains_word(dps[i].name, "double", sizeof ("double"));

                cr_theory_push_arg(ctx,
                        is_float,
                        dps[i].size,
                        ((char*) dps[i].arr) + dps[i].size * indices[i]);
            }

            jmp_buf backup;
            memcpy(backup, g_pre_test, sizeof (jmp_buf));
            if (!setjmp(g_pre_test)) {
                cr_theory_call(ctx, fnptr);
            } else {
                struct {
                    size_t len;
                    char msg[4096];
                } result = { .len = 0 };

                for (size_t i = 0; i < datapoints - 1; ++i) {
                    concat_arg(&result.msg, dps, indices, i);
                    strncat(result.msg, ", ", sizeof (result.msg) - 1);
                }
                concat_arg(&result.msg, dps, indices, datapoints - 1);
                result.len = strlen(result.msg) + 1;

                send_event(THEORY_FAIL, &result, result.len + sizeof (size_t));
            }
            memcpy(g_pre_test, backup, sizeof (jmp_buf));
        }

        for (size_t i = 0; i < datapoints; ++i) {
            if (indices[i] == dps[i].len - 1) {
                indices[i] = 0;
                has_next = i != (datapoints - 1);
            } else {
                ++indices[i];
                break;
            }
        }
    }

    cr_theory_free(ctx);
}
