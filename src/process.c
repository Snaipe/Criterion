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
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <csptr/smart_ptr.h>

#include "criterion/criterion.h"
#include "criterion/options.h"
#include "process.h"
#include "event.h"

struct process {
    pid_t pid;
    int in;
};

static pid_t g_runner_pid;

void set_runner_pid(void) {
    g_runner_pid = getpid();
}

bool is_runner(void) {
    return g_runner_pid == getpid();
}

static void close_process(void *ptr, UNUSED void *meta) {
    close(((struct process *) ptr)->in);
}

struct event *worker_read_event(struct process *proc) {
    return read_event(proc->in);
}

struct process *spawn_test_worker(struct criterion_test *test, void (*func)(struct criterion_test *)) {
    int fds[2];
    if (pipe(fds) == -1)
        abort();

    pid_t pid = fork();
    if (pid == -1) {
        return NULL;
    } else if (!pid) {
        close(STDIN_FILENO);
        close(fds[0]);
        EVENT_PIPE = fds[1];

        func(test);
        close(fds[1]);
        if (!criterion_options.no_early_exit)
            return NULL;
        else
            _exit(0);
    }

    close(fds[1]);
    return unique_ptr(struct process, ({ .pid = pid, .in = fds[0] }), close_process);
}

struct process_status wait_proc(struct process *proc) {
    int status;
    waitpid(proc->pid, &status, 0);

    if (WIFEXITED(status))
        return (struct process_status) { .kind = EXIT_STATUS, .status = WEXITSTATUS(status) };

    if (WIFSIGNALED(status))
        return (struct process_status) { .kind = SIGNAL, .status = WTERMSIG(status) };

    return (struct process_status) { .kind = STOPPED };
}
