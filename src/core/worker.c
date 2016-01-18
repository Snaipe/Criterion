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
#include <stdbool.h>
#include <errno.h>
#include <csptr/smalloc.h>
#include <nanomsg/nn.h>

#include "criterion/types.h"
#include "criterion/options.h"
#include "criterion/redirect.h"
#include "protocol/protocol.h"
#include "protocol/messages.h"
#include "io/event.h"
#include "compat/posix.h"
#include "worker.h"
#include "protocol/connect.h"

static s_proc_handle *g_current_proc;

void set_runner_process(void) {
    g_current_proc = get_current_process();
}

void unset_runner_process(void) {
    sfree(g_current_proc);
}

bool is_runner(void) {
    return is_current_process(g_current_proc);
}

static void close_process(void *ptr, CR_UNUSED void *meta) {
    struct worker *proc = ptr;
    sfree(proc->ctx.suite_stats);
    sfree(proc->ctx.test_stats);
    sfree(proc->ctx.stats);
    sfree(proc->proc);
}

void run_worker(struct worker_context *ctx) {
    ctx->func(ctx->test, ctx->suite);
}

struct worker *spawn_test_worker(struct execution_context *ctx,
                                  cr_worker_func func) {
    g_worker_context = (struct worker_context) {
        .test = ctx->test,
        .suite = ctx->suite,
        .func = func,
        .param = ctx->param,
    };

    struct worker *ptr = NULL;

    s_proc_handle *proc = fork_process();
    if (proc == (void *) -1) {
        criterion_perror("Could not fork the current process and start a worker: %s.\n", strerror(errno));
        abort();
    } else if (proc == NULL) {
        run_worker(&g_worker_context);
        sfree(ctx->test_stats);
        sfree(ctx->suite_stats);
        sfree(ctx->stats);
        return NULL;
    }

    ptr = smalloc(
            .size = sizeof (struct worker),
            .kind = SHARED,
            .dtor = close_process);

    *ptr = (struct worker) {
        .proc = proc,
        .ctx = *ctx,
    };
    return ptr;
}
