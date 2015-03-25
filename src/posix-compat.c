#include "posix-compat.h"

#ifdef _WIN32
# define VC_EXTRALEAN
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <io.h>
# include <fcntl.h>
# include <winnt.h>
# include <setjmp.h>
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

# ifdef _WIN64
#  define Register(Reg, Context) ((Context).R ## Reg)
# else
#  define Register(Reg, Context) ((Context).E ## Reg)
# endif
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

#ifdef _WIN32
# define CONTEXT_INIT { .ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS | CONTEXT_FLOATING_POINT }
struct stack_info {
    char *ptr;
    char *base;
    size_t size;
};

static inline void get_stack_info(struct stack_info *stack) {
    CONTEXT context = CONTEXT_INIT;
    MEMORY_BASIC_INFORMATION mbi;

    GetThreadContext(GetCurrentThread(), &context);
    stack->ptr = (char *) Register(sp, context);

    VirtualQuery(stack->ptr, &mbi, sizeof (mbi));
    stack->base = mbi.BaseAddress;
    stack->size = mbi.RegionSize;
}

static jmp_buf g_jmp;

static void resume_child(void) {
    longjmp(g_jmp, 1);
}
#endif

s_proc_handle *fork_process() {
#ifdef _WIN32
    PROCESS_INFORMATION info;
    STARTUPINFOW si = { .cb = sizeof (STARTUPINFOW) };

    ZeroMemory(&info, sizeof (info));

    CONTEXT context = CONTEXT_INIT;

    // Initialize longjmp buffer
    if (setjmp(g_jmp))
        return NULL; // we are the child, return

    // Create the suspended child process
    wchar_t filename[MAX_PATH];
    GetModuleFileNameW(NULL, filename, MAX_PATH);

    if (!CREATE_SUSPENDED_(filename, GetCommandLineW(), si, info))
        return NULL;

    // Set child's instruction pointer to resume_child
    GetThreadContext(info.hThread, &context);
    Register(ip, context) = (intptr_t) resume_child;
    SetThreadContext(info.hThread, &context);

    // Copy stack
    struct stack_info stack;
    get_stack_info(&stack);

    VirtualAllocEx(info.hProcess, stack.base, stack.size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    WriteProcessMemory(info.hProcess, stack.ptr, stack.ptr, stack.base + stack.size - stack.ptr, NULL);

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
    if (!CreatePipe(fhs, fhs + 1, NULL, 0))
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
