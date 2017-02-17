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
#define _POSIX_SOURCE
#include "kill.h"
#include "internal.h"
#include "log/logging.h"
#include "core/report.h"
#include "core/stats.h"
#include "io/event.h"
#include "io/output.h"
#include <stdlib.h>
#include <assert.h>
#include <signal.h>

#ifdef VANILLA_WIN32
static HANDLE cr_job;
#else
static void handle_sigterm(CR_UNUSED int signum)
{
    assert(signum == SIGTERM);

    kill(-getpid(), SIGTERM);
    _exit(1);
}
#endif

void setup_parent_job(void)
{
#ifdef VANILLA_WIN32
    /* Put runner in its own job */
    cr_job = CreateJobObject(NULL, NULL);
    if (!AssignProcessToJobObject(cr_job, GetCurrentProcess()))
        abort();
#else
    /* Put runner in its own group */
    setpgid(0, 0);

    /* Setup termination handlers */
    sigset_t block_mask;
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGINT);
    sigaddset(&block_mask, SIGQUIT);
    sigaddset(&block_mask, SIGTSTP);

    struct sigaction sa = {
        .sa_handler = handle_sigterm,
        .sa_mask    = block_mask,
    };
    sigaction(SIGTERM, &sa, NULL);
#endif
}

void cr_killall(void)
{
    fflush(NULL);
#ifdef VANILLA_WIN32
    if (!TerminateJobObject(cr_job, 1))
        abort();
#else
    kill(getpid(), SIGTERM);
#endif
}

void cr_terminate(struct criterion_global_stats *stats)
{
    report(POST_ALL, stats);
    process_all_output(stats);
    log(post_all, stats);

    cr_killall();
}
