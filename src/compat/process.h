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
#ifndef COMPAT_PROCESS_H_
# define COMPAT_PROCESS_H_

# include "criterion/types.h"
# include "internal.h"

typedef void (*cr_worker_func)(struct criterion_test *, struct criterion_suite *);

struct proc_handle {
#ifdef VANILLA_WIN32
    HANDLE handle;
#else
    pid_t pid;
#endif
};

typedef struct proc_handle s_proc_handle;

struct worker_context {
    struct criterion_test *test;
    struct criterion_suite *suite;
    cr_worker_func func;
    struct pipe_handle *pipe;
    struct test_single_param *param;
};

extern struct worker_context g_worker_context;

int resume_child(void);

s_proc_handle *fork_process();
void wait_process(s_proc_handle *handle, int *status);

s_proc_handle *get_current_process();
bool is_current_process(s_proc_handle *proc);

unsigned long long get_process_id(void);
unsigned long long get_process_id_of(s_proc_handle *proc);

void init_proc_compat(void);
void free_proc_compat(void);

#endif /* !COMPAT_PROCESS_H_ */
