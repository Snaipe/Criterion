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
#include "criterion/assert.h"
#include "criterion/redirect.h"

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

        case CR_EXCEPTION_TIMEOUT:              sig = SIGPROF; break;

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

FILE *pipe_in(s_pipe_handle *p, int do_close) {
#ifdef VANILLA_WIN32
    if (do_close)
        CloseHandle(p->fhs[1]);
    int fd = _open_osfhandle((intptr_t) p->fhs[0], _O_RDONLY);
    if (fd == -1)
        return NULL;
    FILE *in = _fdopen(fd, "r");
#else
    if (do_close)
        close(p->fds[1]);
    FILE *in = fdopen(p->fds[0], "r");
#endif
    if (!in)
        return NULL;

    setvbuf(in, NULL, _IONBF, 0);
    return in;
}

FILE *pipe_out(s_pipe_handle *p, int do_close) {
#ifdef VANILLA_WIN32
    if (do_close)
        CloseHandle(p->fhs[0]);
    int fd = _open_osfhandle((intptr_t) p->fhs[1], _O_WRONLY);
    if (fd == -1)
        return NULL;
    FILE *out = _fdopen(fd, "w");
#else
    if (do_close)
        close(p->fds[0]);
    FILE *out = fdopen(p->fds[1], "w");
#endif
    if (!out)
        return NULL;

    setvbuf(out, NULL, _IONBF, 0);
    return out;
}

int stdpipe_stack(s_pipe_handle *out) {
#ifdef VANILLA_WIN32
    HANDLE fhs[2];
    SECURITY_ATTRIBUTES attr = {
        .nLength = sizeof (SECURITY_ATTRIBUTES),
        .bInheritHandle = TRUE
    };
    if (!CreatePipe(fhs, fhs + 1, &attr, 0))
        return -1;
    *out = (s_pipe_handle) {{ fhs[0], fhs[1] }};
#else
    int fds[2] = { -1, -1 };
    if (pipe(fds) == -1)
        return -1;
    *out = (s_pipe_handle) {{ fds[0], fds[1] }};
#endif
    return 0;
}

s_pipe_handle *stdpipe() {
    s_pipe_handle *handle = smalloc(sizeof (s_pipe_handle));
    if (stdpipe_stack(handle) < 0)
        return NULL;
    return handle;
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

#ifdef VANILLA_WIN32
typedef DWORD cr_std_fd;
#else
typedef int cr_std_fd;
#endif

static s_pipe_handle stdout_redir;
static s_pipe_handle stderr_redir;
static s_pipe_handle stdin_redir;

enum criterion_std_fd {
    CR_STDIN = 0,
    CR_STDOUT = 1,
    CR_STDERR = 2,
};

enum criterion_pipe_end {
    PIPE_READ = 0,
    PIPE_WRITE = 1,
};

cr_std_fd get_std_fd(int fd_kind) {
    static int kinds[] = {
#ifdef VANILLA_WIN32
        [CR_STDIN]  = STD_INPUT_HANDLE,
        [CR_STDOUT] = STD_OUTPUT_HANDLE,
        [CR_STDERR] = STD_ERROR_HANDLE,
#else
        [CR_STDIN]  = STDIN_FILENO,
        [CR_STDOUT] = STDOUT_FILENO,
        [CR_STDERR] = STDERR_FILENO,
#endif
    };

    return kinds[fd_kind];
}

FILE* get_std_file(int fd_kind) {
    switch (fd_kind) {
        case CR_STDIN:  return stdin;
        case CR_STDOUT: return stdout;
        case CR_STDERR: return stderr;
    }
    return NULL;
}

int make_redirect_pipe(s_pipe_handle *handle, int id, int noblock) {
#ifdef VANILLA_WIN32
    HANDLE fhs[2];
    SECURITY_ATTRIBUTES attr = {
        .nLength = sizeof (SECURITY_ATTRIBUTES),
        .bInheritHandle = TRUE
    };
    char pipe_name[256] = {0};
    snprintf(pipe_name, sizeof (pipe_name),
        "\\\\.\\pipe\\criterion_%lu_%d", GetCurrentProcessId(), id);
    fhs[0] = CreateNamedPipe(pipe_name,
            PIPE_ACCESS_INBOUND,
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE
                           | (noblock ? PIPE_NOWAIT : PIPE_WAIT),
            1,
            4096 * 4,
            4096 * 4,
            0,
            &attr);

    if (fhs[0] == INVALID_HANDLE_VALUE)
        return 0;

    fhs[1] = CreateFile(pipe_name,
            GENERIC_WRITE,
            0,
            &attr,
            OPEN_EXISTING,
            0,
            NULL);

    if (fhs[1] == INVALID_HANDLE_VALUE) {
        CloseHandle(fhs[0]);
        return 0;
    }

    *handle = (s_pipe_handle) {{ fhs[0], fhs[1] }};
#else
    (void) id;

    int fds[2] = { -1, -1 };
    if (pipe(fds) == -1)
        return 0;

    if (noblock)
        for (int i = 0; i < 2; ++i)
            fcntl(fds[i], F_SETFL, fcntl(fds[i], F_GETFL) | O_NONBLOCK);

    *handle = (s_pipe_handle) {{ fds[0], fds[1] }};
#endif
    return 1;
}

void cr_redirect(int fd_kind, s_pipe_handle *pipe, int fd_index, int noblock) {
    fflush(get_std_file(fd_kind));
    if (!make_redirect_pipe(pipe, fd_kind, noblock))
        cr_assert_fail("Could not redirect standard file descriptor.");

    cr_std_fd fd = get_std_fd(fd_kind);
#ifdef VANILLA_WIN32
    int stdfd = _open_osfhandle((intptr_t) pipe->fhs[fd_index], fd_kind == 0 ? _O_RDONLY : _O_WRONLY);
    if (stdfd == -1)
        cr_assert_fail("Could not redirect standard file descriptor.");

    fflush(get_std_file(fd_kind));

    _close(fd_kind);
    SetStdHandle(fd, pipe->fhs[fd_index]);

    _dup2(stdfd, fd_kind);
    _close(stdfd);

    setvbuf(get_std_file(fd_kind), NULL, _IONBF, 0);
#else
    close(fd);
    dup2(pipe->fds[fd_index], fd);
    close(pipe->fds[fd_index]);
#endif
}

void cr_redirect_stdout(void) {
    cr_redirect(CR_STDOUT, &stdout_redir, PIPE_WRITE, 1);
}

void cr_redirect_stderr(void) {
    cr_redirect(CR_STDERR, &stderr_redir, PIPE_WRITE, 1);
}

void cr_redirect_stdin(void) {
    cr_redirect(CR_STDIN, &stdin_redir, PIPE_READ, 0);
}

FILE* cr_get_redirected_stdout(void) {
    static FILE *f;
    if (!f) {
        f = pipe_in(&stdout_redir, 0);
        if (!f)
            cr_assert_fail("Could not get redirected stdout read end.");
    }
    return f;
}

FILE* cr_get_redirected_stderr(void) {
    static FILE *f;
    if (!f) {
        f = pipe_in(&stderr_redir, 0);
        if (!f)
            cr_assert_fail("Could not get redirected stderr read end.");
    }
    return f;
}

FILE* cr_get_redirected_stdin(void) {
    static FILE *f;
    if (!f) {
        f = pipe_out(&stdin_redir, 0);
        if (!f)
            cr_assert_fail("Could not get redirected stdin write end.");
    }
    return f;
}

int cr_file_match_str(FILE* f, const char *str) {
    size_t len = strlen(str);

    char buf[512];
    size_t read;
    int matches = 0;
    while ((read = fread(buf, 1, sizeof (buf), f)) > 0) {
        matches = !strncmp(buf, str, read);
        if (!matches || read > len) {
            matches = 0;
            break;
        }

        len -= read;
        str += read;
    }

    // consume the rest of what's available
    while (fread(buf, 1, sizeof (buf), f) > 0);

    return matches;
}
