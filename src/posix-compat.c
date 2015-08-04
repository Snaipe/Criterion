#include <assert.h>
#include "posix-compat.h"
#include "process.h"

#ifdef VANILLA_WIN32
# define VC_EXTRALEAN
# define WIN32_LEAN_AND_MEAN
# undef _WIN32_WINNT
# define _WIN32_WINNT 0x0502
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

# define WRITE_PROCESS_(Proc, What, Size)                           \
        WriteProcessMemory(Proc, &What, &What, Size, NULL);

#else
# include <unistd.h>
# include <sys/wait.h>
# include <sys/signal.h>
# include <sys/fcntl.h>
#endif

#include <csptr/smart_ptr.h>

struct proc_handle {
#ifdef VANILLA_WIN32
    HANDLE handle;
#else
    pid_t pid;
#endif
};

struct pipe_handle {
#ifdef VANILLA_WIN32
    HANDLE fhs[2];
#else
    int fds[2];
#endif
};

struct worker_context g_worker_context = {.test = NULL};

#ifdef VANILLA_WIN32
struct full_context {
    struct worker_context ctx;
    struct criterion_test test;
    struct criterion_test_extra_data test_data;
    struct criterion_suite suite;
    struct criterion_test_extra_data suite_data;
    struct pipe_handle pipe;
    int resumed;
};

static TCHAR g_mapping_name[] = TEXT("WinCriterionWorker");
#define MAPPING_SIZE sizeof (struct full_context)

static struct full_context local_ctx;
#endif

int resume_child(void) {
#ifdef VANILLA_WIN32
    HANDLE sharedMem = OpenFileMapping(
           FILE_MAP_ALL_ACCESS,
           FALSE,
           g_mapping_name);

    if (sharedMem == NULL)
        return 0;

    struct full_context *ctx = (struct full_context *) MapViewOfFile(sharedMem,
           FILE_MAP_ALL_ACCESS,
           0,
           0,
           MAPPING_SIZE);

    if (ctx == NULL)
        exit(-1);

    local_ctx = *ctx;
    g_worker_context = local_ctx.worker_context;
    ctx.resumed = 1;

    UmapViewOfFile(ctx);
    CloseHandle(sharedMem);

    run_worker(&g_worker_context);
    return 1;
#else
    return 0;
#endif
}

s_proc_handle *fork_process() {
#ifdef VANILLA_WIN32
    PROCESS_INFORMATION info;
    STARTUPINFOW si = { .cb = sizeof (STARTUPINFOW) };

    ZeroMemory(&info, sizeof (info));

    // Create the suspended child process
    wchar_t filename[MAX_PATH];
    GetModuleFileNameW(NULL, filename, MAX_PATH);

    if (!CREATE_SUSPENDED_(filename, GetCommandLineW(), si, info))
        return (void *) -1;

    // Copy context over
    HANDLE sharedMem = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            MAPPING_SIZE,
            g_mapping_name)

    if (sharedMem == NULL)
        return (void *) -1;

    struct full_context *ctx = (struct full_context *) MapViewOfFile(sharedMem,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            MAPPING_SIZE);

    if (ctx == NULL) {
        CloseHandle(sharedMem);
        return (void *) -1;
    }

    *ctx = (struc full_context) {
        .test      = *g_worker_context.test;
        .test_data = *g_worker_context.test->data;
        .suite     = *g_worker_context.suite;
        .pipe      = *g_worker_context.pipe;
    };

    ctx.ctx = (struct worker_context) {
        &ctx.test,
        &ctx.suite,
        g_worker_context.func,
        &ctx.pipe
    };

    ctx.test.data = &ctx.test_data;

    if (g_worker_context.suite->data) {
        ctx.suite_data = *g_worker_context.suite->data;
        ctx.suite.data = &ctx.suite_data;
    }

    ResumeThread(info.hThread);
    CloseHandle(info.hThread);

    while (!ctx.resumed); // wait until the child has initialized itself

    UmapViewOfFile(ctx);
    CloseHandle(sharedMem);

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
#ifdef VANILLA_WIN32
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
#ifdef VANILLA_WIN32
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
#ifdef VANILLA_WIN32
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
#ifdef VANILLA_WIN32
    HANDLE fhs[2];
    SECURITY_ATTRIBUTES attr = {
        .nLength = sizeof (SECURITY_ATTRIBUTES),
        .bInheritHandle = TRUE
    };
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
#ifdef VANILLA_WIN32
    return unique_ptr(s_proc_handle, { GetCurrentProcess() });
#else
    return unique_ptr(s_proc_handle, { getpid() });
#endif
}

bool is_current_process(s_proc_handle *proc) {
#ifdef VANILLA_WIN32
    return GetProcessId(proc->handle) == GetProcessId(GetCurrentProcess());
#else
    return proc->pid == getpid();
#endif
}

#ifdef VANILLA_WIN32
void *get_win_section_start(const char *section) {
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER) GetModuleHandle(NULL);
    PIMAGE_NT_HEADERS ntHeader = ntHeader = (PIMAGE_NT_HEADERS) ((DWORD)(dosHeader) + (dosHeader->e_lfanew));

    assert(dosHeader->e_magic == IMAGE_DOS_SIGNATURE);
    assert(ntHeader->Signature == IMAGE_NT_SIGNATURE);

    PIMAGE_SECTION_HEADER pSecHeader = IMAGE_FIRST_SECTION(ntHeader);
    for(int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++, pSecHeader++) {
        if (!strncmp((char*) pSecHeader->Name, section, 8)) {
            return (char*) dosHeader + pSecHeader->VirtualAddress;
        }
    }
    return NULL;
}

void *get_win_section_end(const char *section) {
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER) GetModuleHandle(NULL);
    PIMAGE_NT_HEADERS ntHeader = ntHeader = (PIMAGE_NT_HEADERS) ((DWORD)(dosHeader) + (dosHeader->e_lfanew));

    assert(dosHeader->e_magic == IMAGE_DOS_SIGNATURE);
    assert(ntHeader->Signature == IMAGE_NT_SIGNATURE);

    PIMAGE_SECTION_HEADER pSecHeader = IMAGE_FIRST_SECTION(ntHeader);
    for(int i = 0; i < ntHeader->FileHeader.NumberOfSections; i++, pSecHeader++) {
        if (!strncmp((char*) pSecHeader->Name, section, 8)) {
            return (char*) dosHeader + (size_t) pSecHeader->VirtualAddress + pSecHeader->SizeOfRawData;
        }
    }
    return NULL;
}
#endif
