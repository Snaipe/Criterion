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
#ifndef PROCESS_H_
# define PROCESS_H_

# include <stdbool.h>
# include "criterion/types.h"
# include "compat/process.h"
# include "compat/pipe.h"

struct test_single_param {
    size_t size;
    void *ptr;
};

struct execution_context {
    bool test_started;
    bool normal_finish;
    bool cleaned_up;
    bool aborted;
    struct criterion_global_stats *stats;
    struct criterion_test *test;
    struct criterion_test_stats *test_stats;
    struct criterion_suite *suite;
    struct criterion_suite_stats *suite_stats;
    struct test_single_param *param;
};

struct worker {
    int active;
    s_proc_handle *proc;
    s_pipe_file_handle *in;
    struct execution_context ctx;
};

enum status_kind {
    EXIT_STATUS,
    STOPPED,
    SIGNAL,
};

struct process_status {
    enum status_kind kind;
    int status;
};

struct worker_status {
    s_proc_handle proc;
    struct process_status status;
};

struct worker_set {
    struct worker **workers;
    size_t max_workers;
};

extern s_pipe_handle *g_worker_pipe;

void run_worker(struct worker_context *ctx);
void set_runner_process(void);
void unset_runner_process(void);
bool is_runner(void);
struct process_status wait_proc(struct worker *proc);
struct process_status get_status(int status);
struct worker *spawn_test_worker(struct execution_context *ctx,
                                  cr_worker_func func,
                                  s_pipe_handle *pipe);
struct event *worker_read_event(struct worker_set *workers, s_pipe_file_handle *pipe);

#endif /* !PROCESS_H_ */
