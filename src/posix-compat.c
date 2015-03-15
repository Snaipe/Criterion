#include "posix-compat.h"

#ifdef _WIN32
# include <windows.h>
#else
# include <unistd.h>
# include <sys/wait.h>
# include <sys/signal.h>
# include <sys/fcntl.h>
#endif

#include <csptr/smart_ptr.h>

struct proc_handle {
#ifdef _WIN32
    // TODO
#else
    pid_t pid;
#endif
};

struct pipe_handle {
#ifdef _WIN32
    // TODO
#else
    int fds[2];
#endif
};

s_proc_handle *fork_process() {
#ifdef _WIN32
    // TODO
#else
    pid_t pid = fork();
    if (pid == -1)
        return (void *) -1;
    if (pid == 0)
        return NULL;
    return unique_ptr(s_proc_handle, { pid });
#endif
}

void wait_process(s_proc_handle *handle, int *status) {
#ifdef _WIN32
    // TODO
#else
    waitpid(handle->pid, status, 0);
#endif
}

FILE *pipe_in(s_pipe_handle *p) {
    close(p->fds[1]);
    FILE *in = fdopen(p->fds[0], "r");
    setvbuf(in, NULL, _IONBF, 0);
    return in;
}

FILE *pipe_out(s_pipe_handle *p) {
    close(p->fds[0]);
    FILE *out = fdopen(p->fds[1], "w");
    setvbuf(out, NULL, _IONBF, 0);
    return out;
}

s_pipe_handle *stdpipe() {
#ifdef _WIN32
    // TODO
#else
    int fds[2] = { -1, -1 };
    if (pipe(fds) == -1)
        return NULL;
    return unique_ptr(s_pipe_handle, {{ fds[0], fds[1] }});
#endif
}

s_proc_handle *get_current_process() {
    return unique_ptr(s_proc_handle, { getpid() });
}

bool is_current_process(s_proc_handle *proc) {
    return proc->pid == getpid();
}
