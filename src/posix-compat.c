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

# include <signal.h>
# ifndef SIGALRM
#  define SIGALRM 14
# endif

#else
# include <unistd.h>
# include <sys/wait.h>
# include <sys/signal.h>
# include <sys/fcntl.h>
#endif

#include <csptr/smalloc.h>

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
    struct criterion_test test;
    struct criterion_test_extra_data test_data;
    struct criterion_suite suite;
    struct criterion_test_extra_data suite_data;
    f_worker_func func;
    struct pipe_handle pipe;
    volatile int resumed;
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
    g_worker_context = (struct worker_context) {
        &local_ctx.test,
        &local_ctx.suite,
        local_ctx.func,
        &local_ctx.pipe
    };

    local_ctx.test.data  = &local_ctx.test_data;
    local_ctx.suite.data = &local_ctx.suite_data;

    ctx->resumed = 1;

    UnmapViewOfFile(ctx);
    CloseHandle(sharedMem);

    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);

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
            g_mapping_name);

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

    *ctx = (struct full_context) {
        .test      = *g_worker_context.test,
        .test_data = *g_worker_context.test->data,
        .suite     = *g_worker_context.suite,
        .func      = g_worker_context.func,
        .pipe      = *g_worker_context.pipe,
        .resumed   = 0,
    };

    if (g_worker_context.suite->data)
        ctx->suite_data = *g_worker_context.suite->data;

    ResumeThread(info.hThread);
    CloseHandle(info.hThread);

    while (!ctx->resumed); // wait until the child has initialized itself

    UnmapViewOfFile(ctx);
    CloseHandle(sharedMem);

    s_proc_handle *handle = smalloc(sizeof (s_proc_handle));
    *handle = (s_proc_handle) { info.hProcess };
    return handle;
#else
    pid_t pid = fork();
    if (pid == -1)
        return (void *) -1;
    if (pid == 0)
        return NULL;

    s_proc_handle *handle = smalloc(sizeof (s_proc_handle));
    *handle = (s_proc_handle) { pid };
    return handle;
#endif
}

void wait_process(s_proc_handle *handle, int *status) {
#ifdef VANILLA_WIN32
    WaitForSingleObject(handle->handle, INFINITE);
    DWORD exit_code;
    GetExitCodeProcess(handle->handle, &exit_code);
    CloseHandle(handle->handle);

    unsigned int sig = 0;
    switch (exit_code) {
        case STATUS_FLOAT_DENORMAL_OPERAND:
        case STATUS_FLOAT_DIVIDE_BY_ZERO:
        case STATUS_FLOAT_INEXACT_RESULT:
        case STATUS_FLOAT_INVALID_OPERATION:
        case STATUS_FLOAT_OVERFLOW:
        case STATUS_FLOAT_STACK_CHECK:
        case STATUS_FLOAT_UNDERFLOW:
        case STATUS_INTEGER_DIVIDE_BY_ZERO:
        case STATUS_INTEGER_OVERFLOW:           sig = SIGFPE; break;

        case STATUS_ILLEGAL_INSTRUCTION:
        case STATUS_PRIVILEGED_INSTRUCTION:
        case STATUS_NONCONTINUABLE_EXCEPTION:   sig = SIGILL; break;

        case STATUS_TIMEOUT:                    sig = SIGALRM; break;

        case STATUS_ACCESS_VIOLATION:
        case STATUS_DATATYPE_MISALIGNMENT:
        case STATUS_ARRAY_BOUNDS_EXCEEDED:
        case STATUS_GUARD_PAGE_VIOLATION:
        case STATUS_IN_PAGE_ERROR:
        case STATUS_NO_MEMORY:
        case STATUS_INVALID_DISPOSITION:
        case STATUS_STACK_OVERFLOW:             sig = SIGSEGV; break;

        case STATUS_CONTROL_C_EXIT:             sig = SIGINT; break;

        default: break;
    }

    *status = sig ? sig : exit_code << 8;
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
    s_pipe_handle *handle = smalloc(sizeof (s_pipe_handle));
#ifdef VANILLA_WIN32
    HANDLE fhs[2];
    SECURITY_ATTRIBUTES attr = {
        .nLength = sizeof (SECURITY_ATTRIBUTES),
        .bInheritHandle = TRUE
    };
    if (!CreatePipe(fhs, fhs + 1, &attr, 0))
        return NULL;
    *handle = (s_pipe_handle) {{ fhs[0], fhs[1] }};
    return handle;
#else
    int fds[2] = { -1, -1 };
    if (pipe(fds) == -1)
        return NULL;
    *handle = (s_pipe_handle) {{ fds[0], fds[1] }};
    return handle;
#endif
}

s_proc_handle *get_current_process() {
    s_proc_handle *handle = smalloc(sizeof (s_proc_handle));
#ifdef VANILLA_WIN32
    *handle = (s_proc_handle) { GetCurrentProcess() };
#else
    *handle = (s_proc_handle) { getpid() };
#endif
    return handle;
}

bool is_current_process(s_proc_handle *proc) {
#ifdef VANILLA_WIN32
    return GetProcessId(proc->handle) == GetProcessId(GetCurrentProcess());
#else
    return proc->pid == getpid();
#endif
}

#ifdef _WIN32
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

#ifdef __APPLE__
# include <mach-o/getsect.h>
# include <mach-o/dyld.h>

# define BASE_IMAGE_INDEX 0

static inline void *get_real_address(void *addr) {
    if (!addr)
        return NULL;

    // We need to slide the section address to get a valid pointer
    // because ASLR will shift the image by a random offset
    return addr + _dyld_get_image_vmaddr_slide(BASE_IMAGE_INDEX);
}

void *get_osx_section_start(const char *section) {
    unsigned long secsize;
    return get_real_address(getsectdata("__DATA", section, &secsize));
}

void *get_osx_section_end(const char *section) {
    unsigned long secsize;
    char *section_start = getsectdata("__DATA", section, &secsize);
    return get_real_address(section_start) + secsize;
}
#endif

const char *basename_compat(const char *str) {
    const char *start = str;
    for (const char *c = str; *c; ++c)
        if ((*c == '/' || *c == '\\') && c[1])
            start = c + 1;
    return start;
}
