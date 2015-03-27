#include "posix-compat.h"
#include "process.h"

#ifdef _WIN32
# define VC_EXTRALEAN
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <io.h>
# include <fcntl.h>
# include <winnt.h>
# include <stdint.h>

# define CREATE_SUSPENDED_(Filename, CmdLine, StartupInfo, Info)    \
    CreateProcessW(Filename,                                        \
            CmdLine,                                                \
            NULL,                                                   \
            NULL,                                                   \
            TRUE,                                                   \
            CREATE_SUSPENDED,                                       \
            NULL,                                                   \
            NULL,                                                   \
            &(StartupInfo),                                         \
            &(Info))

#else
# include <unistd.h>
# include <sys/wait.h>
# include <sys/signal.h>
# include <sys/fcntl.h>
#endif

#include <csptr/smart_ptr.h>

struct proc_handle {
#ifdef _WIN32
    HANDLE handle;
#else
    pid_t pid;
#endif
};

struct pipe_handle {
#ifdef _WIN32
    HANDLE fhs[2];
#else
    int fds[2];
#endif
};

struct worker_context g_worker_context = {.test = NULL};

#ifdef _WIN32
static struct criterion_test            child_test;
static struct criterion_test_extra_data child_test_data;
static struct criterion_suite           child_suite;
static struct criterion_test_extra_data child_suite_data;
static struct pipe_handle               child_pipe;
#endif

int resume_child(void) {
    if (g_worker_context.test) {
        run_worker(&g_worker_context);
        return 1;
    }
    return 0;
}

s_proc_handle *fork_process() {
#ifdef _WIN32
    PROCESS_INFORMATION info;
    STARTUPINFOW si = { .cb = sizeof (STARTUPINFOW) };

    ZeroMemory(&info, sizeof (info));

    // Create the suspended child process
    wchar_t filename[MAX_PATH];
    GetModuleFileNameW(NULL, filename, MAX_PATH);

    if (!CREATE_SUSPENDED_(filename, GetCommandLineW(), si, info))
        return (void *) -1;

    // Copy context over
    f_worker_func child_func = g_worker_context.func;

    child_test      = *g_worker_context.test;
    child_test_data = *g_worker_context.test->data;
    child_suite     = *g_worker_context.suite;
    child_pipe      = *g_worker_context.pipe;

    g_worker_context = (struct worker_context) { &child_test, &child_suite, child_func, &child_pipe };
    child_test.data  = &child_test_data;

    if (g_worker_context.suite->data) {
        child_suite_data = *g_worker_context.suite->data;
        child_suite.data = &child_suite_data;
        WriteProcessMemory(info.hProcess, &child_suite_data, &child_suite_data, sizeof (child_suite_data), NULL);
    }

    WriteProcessMemory(info.hProcess, &child_test, &child_test, sizeof (child_test), NULL);
    WriteProcessMemory(info.hProcess, &child_test_data, &child_test_data, sizeof (child_test_data), NULL);
    WriteProcessMemory(info.hProcess, &child_suite, &child_suite, sizeof (child_suite), NULL);
    WriteProcessMemory(info.hProcess, &child_pipe, &child_pipe, sizeof (child_pipe), NULL);
    WriteProcessMemory(info.hProcess, &g_worker_context, &g_worker_context, sizeof (struct worker_context), NULL);

    ResumeThread(info.hThread);
    CloseHandle(info.hThread);

    return unique_ptr(s_proc_handle, { info.hProcess });
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
    WaitForSingleObject(handle->handle, INFINITE);
    DWORD exit_code;
    GetExitCodeProcess(handle->handle, &exit_code);
    CloseHandle(handle->handle);
    *status = exit_code << 8;
#else
    waitpid(handle->pid, status, 0);
#endif
}

FILE *pipe_in(s_pipe_handle *p) {
#ifdef _WIN32
    CloseHandle(p->fhs[1]);
    int fd = _open_osfhandle((intptr_t) p->fhs[0], _O_RDONLY);
    if (fd == -1)
        return NULL;
    FILE *in = _fdopen(fd, "r");
#else
    close(p->fds[1]);
    FILE *in = fdopen(p->fds[0], "r");
#endif
    if (!in)
        return NULL;

    setvbuf(in, NULL, _IONBF, 0);
    return in;
}

FILE *pipe_out(s_pipe_handle *p) {
#ifdef _WIN32
    CloseHandle(p->fhs[0]);
    int fd = _open_osfhandle((intptr_t) p->fhs[1], _O_WRONLY);
    if (fd == -1)
        return NULL;
    FILE *out = _fdopen(fd, "w");
#else
    close(p->fds[0]);
    FILE *out = fdopen(p->fds[1], "w");
#endif
    if (!out)
        return NULL;

    setvbuf(out, NULL, _IONBF, 0);
    return out;
}

s_pipe_handle *stdpipe() {
#ifdef _WIN32
    HANDLE fhs[2];
    SECURITY_ATTRIBUTES attr = { .nLength = sizeof (SECURITY_ATTRIBUTES), .bInheritHandle = TRUE }; 
    if (!CreatePipe(fhs, fhs + 1, &attr, 0))
        return NULL;
    return unique_ptr(s_pipe_handle, {{ fhs[0], fhs[1] }});
#else
    int fds[2] = { -1, -1 };
    if (pipe(fds) == -1)
        return NULL;
    return unique_ptr(s_pipe_handle, {{ fds[0], fds[1] }});
#endif
}

s_proc_handle *get_current_process() {
#ifdef _WIN32
    return unique_ptr(s_proc_handle, { GetCurrentProcess() });
#else
    return unique_ptr(s_proc_handle, { getpid() });
#endif
}

bool is_current_process(s_proc_handle *proc) {
#ifdef _WIN32
    return GetProcessId(proc->handle) == GetProcessId(GetCurrentProcess());
#else
    return proc->pid == getpid();
#endif
}
