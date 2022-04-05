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
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <ffi.h>
#include <assert.h>
#include <limits.h>
#include "compat/setjmp.h"
#include "criterion/theories.h"
#include "protocol/protocol.h"
#include "protocol/messages.h"
#include "io/event.h"
#include "string/fmt.h"
#include "abort.h"

enum {
    MAX_ARGS = 128,
};

struct cri_theory_ctx {
    ffi_type *types[MAX_ARGS];
    void *args[MAX_ARGS];
    size_t nargs;
};

static void cri_append_arg(struct cri_theory_ctx *ctx, ffi_type *t, void *val)
{
    assert(ctx->nargs + 1 < MAX_ARGS);
    ctx->types[ctx->nargs] = t;
    ctx->args[ctx->nargs] = val;
    ctx->nargs++;
}

void cri_theory_push_arg(struct cri_theory_ctx *ctx, bool is_float, size_t size, void *ptr)
{
    ffi_type *float_ffi_types[] = {
        [sizeof (float)] = &ffi_type_float,
        [sizeof (double)] = &ffi_type_double,
        [sizeof (long double)] = &ffi_type_longdouble,
    };

    ffi_type *ffi_types[] = {
        [sizeof (uint8_t)] = &ffi_type_uint8,
        [sizeof (uint16_t)] = &ffi_type_uint16,
        [sizeof (uint32_t)] = &ffi_type_uint32,
        [sizeof (uint64_t)] = &ffi_type_uint64,
    };

    ffi_type *type = is_float ? float_ffi_types[size] : ffi_types[size];
    assert(type);
    cri_append_arg(ctx, type, ptr);
}

struct cri_theory_ctx *cr_theory_init(void)
{
    return calloc(1, sizeof (struct cri_theory_ctx));
}

void cr_theory_free(struct cri_theory_ctx *ctx)
{
    free(ctx);
}

static jmp_buf theory_jmp;

void cr_theory_abort(void)
{
    cri_longjmp(theory_jmp, 1);
}

void cr_theory_call(struct cri_theory_ctx *ctx, void (*fnptr)(void))
{
    ffi_cif cif;
    assert(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, ctx->nargs, &ffi_type_void, ctx->types) == FFI_OK);

    ffi_call(&cif, fnptr, NULL, ctx->args);
}

static bool contains_word(const char *str, const char *pattern, size_t sz)
{
    char *res = strstr(str, pattern);

    return res
           && (res == str || (res > str && res[-1] == ' '))
           && (!res[sz - 1] || res[sz - 1] == ' ');
}

static bool is_string(const char *name)
{
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

static bool is_float(const char *name)
{
    return contains_word(name, "float", sizeof ("float"))
           || contains_word(name, "double", sizeof ("double"));
}

static bool is_long_double(const char *name)
{
    return !strcmp(name, "long double");
}

static bool is_unsigned_int(const char *name)
{
    return contains_word(name, "unsigned", sizeof ("unsigned"))
           || !strncmp(name, "uint", 4);
}

static bool is_bool(const char *name)
{
    return contains_word(name, "bool", sizeof ("bool"))
           || contains_word(name, "_Bool", sizeof ("_Bool"));
}

static void format_arg(char (*arg)[1024], struct criterion_datapoints *dp, void *data)
{
    if (is_float(dp->name)) {
        if (dp->size == sizeof (float))
            snprintf(*arg, sizeof (*arg) - 1, "%gf", *(float *) data);
        else if (dp->size == sizeof (long double) && is_long_double(dp->name))
            snprintf(*arg, sizeof (*arg) - 1, "%gl", (double) *(long double *) data);
        else if (dp->size == sizeof (double))
            snprintf(*arg, sizeof (*arg) - 1, "%g", *(double *) data);
    } else {
        if (is_string(dp->name)) {
            snprintf(*arg, sizeof (*arg) - 1, "\"%s\"", *(char **) data);
        } else if (dp->size == sizeof (bool) && is_bool(dp->name)) {
            snprintf(*arg, sizeof (*arg) - 1, "%s", (*(bool *) data) ? "true" : "false");
        } else if (dp->size == sizeof (char)) {
            snprintf(*arg, sizeof (*arg) - 1, "'%c'", *(char *) data);
        } else if (dp->size == sizeof (short)) {
            const char *fmt = is_unsigned_int(dp->name) ? "%hu" : "%hd";
            snprintf(*arg, sizeof (*arg) - 1, fmt, *(short *) data);
        } else if (dp->size == sizeof (int)) {
            const char *fmt = is_unsigned_int(dp->name) ? "%u" : "%d";
            snprintf(*arg, sizeof (*arg) - 1, fmt, *(int *) data);
        } else if (dp->size == sizeof (void *) && strstr(dp->name, "*")) {
            snprintf(*arg, sizeof (*arg) - 1, "%p", *(void **) data);
#if INT_MAX < LONG_MAX
        } else if (dp->size == sizeof (long)) {
            const char *fmt = is_unsigned_int(dp->name) ? "%lulu" : "%ldl";
            snprintf(*arg, sizeof (*arg) - 1, fmt, *(long *) data);
#endif
#if LONG_MAX < LLONG_MAX
        } else if (dp->size == sizeof (long long)) {
            const char *fmt = is_unsigned_int(dp->name) ? "%llullu" : "%lldll";
            snprintf(*arg, sizeof (*arg) - 1, fmt, *(long long *) data);
#endif
        } else {
            snprintf(*arg, sizeof (*arg) - 1, "%s", "<np>");
        }
    }
}

#define BUFSIZE    4096

static void concat_arg(char (*msg)[BUFSIZE], struct criterion_datapoints *dps, size_t *indices, size_t i)
{
    void *data = ((char *) dps[i].arr) + dps[i].size * indices[i];

    char arg[1024];

    format_arg(&arg, dps + i, data);
    strncat(*msg, arg, BUFSIZE - 1);
}

int try_call_theory(struct cri_theory_ctx *ctx, void (*fnptr)(void))
{
    if (!cri_setjmp(g_pre_test)) {
        cr_theory_call(ctx, fnptr);
        return 1;
    }
    return 0;
}

void cr_theory_main(struct criterion_datapoints *dps, size_t datapoints, void (*fnptr)(void))
{
    struct cri_theory_ctx *ctx = cr_theory_init();

    size_t *indices = malloc(sizeof (size_t) * datapoints);

    memset(indices, 0, datapoints * sizeof (size_t));

    volatile int round = 1;
    volatile bool has_next = true;
    while (has_next) {
        char *name = NULL;
        cr_asprintf(&name, "%s::%d", criterion_current_test->name, round);

        criterion_protocol_msg setup_msg = criterion_message(phase,
                        .phase = criterion_protocol_phase_kind_SETUP,
                        .name = name,
                        );
        criterion_message_set_id(setup_msg);
        cr_send_to_runner(&setup_msg);

        criterion_protocol_msg main_msg = criterion_message(phase,
                        .phase = criterion_protocol_phase_kind_MAIN,
                        .name = name,
                        );
        criterion_message_set_id(main_msg);
        cr_send_to_runner(&main_msg);

        int theory_aborted = 0;
        if (!cri_setjmp(theory_jmp)) {
            ctx->nargs = 0;

            for (size_t i = 0; i < datapoints; ++i) {
                bool is_float = contains_word(dps[i].name, "float", sizeof ("float"))
                        || contains_word(dps[i].name, "double", sizeof ("double"));

                cri_theory_push_arg(ctx,
                        is_float,
                        dps[i].size,
                        ((char *) dps[i].arr) + dps[i].size * indices[i]);
            }

            if (!try_call_theory(ctx, fnptr)) {
                theory_aborted = 1;

                struct {
                    size_t len;
                    char msg[BUFSIZE];
                } result = { .len = 0 };

                for (size_t i = 0; i < datapoints - 1; ++i) {
                    concat_arg(&result.msg, dps, indices, i);
                    strncat(result.msg, ", ", sizeof (result.msg) - 1);
                }
                concat_arg(&result.msg, dps, indices, datapoints - 1);
                result.len = strlen(result.msg) + 1;
                result.msg[result.len] = '\0';

                criterion_protocol_msg msg = criterion_message(phase,
                                .phase = criterion_protocol_phase_kind_ABORT,
                                .name = name,
                                .message = result.msg
                                );
                criterion_message_set_id(msg);
                cr_send_to_runner(&msg);
            }
        }

        if (!theory_aborted) {
            criterion_protocol_msg teardown_msg = criterion_message(phase,
                            .phase = criterion_protocol_phase_kind_TEARDOWN,
                            .name = name,
                            );
            criterion_message_set_id(teardown_msg);
            cr_send_to_runner(&teardown_msg);

            criterion_protocol_msg end_msg = criterion_message(phase,
                            .phase = criterion_protocol_phase_kind_END,
                            .name = name,
                            );
            criterion_message_set_id(end_msg);
            cr_send_to_runner(&end_msg);
        }
        free(name);

        for (size_t i = 0; i < datapoints; ++i) {
            if (indices[i] == dps[i].len - 1) {
                indices[i] = 0;
                has_next = i != (datapoints - 1);
            } else {
                ++indices[i];
                break;
            }
        }
        ++round;
    }

    free(indices);
    cr_theory_free(ctx);
}
