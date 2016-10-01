/*
 * The MIT License (MIT)
 *
 * Copyright © 2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#include <string.h>
#include "err.h"
#include "unwind.h"

#ifdef HAVE_GNU_UNWINDER

struct unwind_ctx {
    struct cri_unwind_jmp_buf *jmp;
    int val;
};

static _Unwind_Reason_Code unwind_stack(int ver, _Unwind_Action action,
        uint64_t class,
        struct _Unwind_Exception *exc,
        struct _Unwind_Context *ctx,
        void *stop_param)
{
    (void) ver;
    (void) action;
    (void) class;
    (void) exc;

    struct unwind_ctx *uctx = stop_param;
    void *frame = (void *) _Unwind_GetCFA(ctx);

    if (action & _UA_END_OF_STACK)
        return _URC_END_OF_STACK;
    if (frame >= uctx->jmp->frame) {
        struct unwind_ctx nctx = *uctx;
        free(exc);
        free(uctx);
        longjmp(nctx.jmp->buf, nctx.val);
    }

    return _URC_NO_REASON;
}

struct Unwind_Exception {
    uint64_t exception_class;
    _Unwind_Exception_Cleanup_Fn exception_cleanup;
    uint64_t private_1;
    uint64_t private_2;
};

CR_NORETURN void cri_unwind_longjmp(struct cri_unwind_jmp_buf *jmp, int val)
{
    struct Unwind_Exception *exc = calloc(1, sizeof (struct Unwind_Exception));

    exc->exception_class = *(uint64_t *) "cr\0\0asrt";

    volatile struct unwind_ctx *ctx = malloc(sizeof (struct unwind_ctx));
    *ctx = (struct unwind_ctx) { jmp, val };

    _Unwind_ForcedUnwind((struct _Unwind_Exception *) exc, unwind_stack, (void *) ctx);

    /* In cases where the unwinding cannot go further (e.g. with dyncall'ed
       functions) we need to longjmp anyway */
    struct unwind_ctx nctx = *ctx;
    free(exc);
    free((void *) ctx);
    longjmp(nctx.jmp->buf, nctx.val);
}

#else

/* Fallback to a non-unwinding longjmp */
CR_NORETURN void cri_unwind_longjmp(struct cri_unwind_jmp_buf *jmp, int val)
{
    longjmp(jmp->buf, val);
}

#endif
