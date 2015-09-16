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
#include <csptr/smalloc.h>
#include "process.h"
#include "internal.h"

#ifdef VANILLA_WIN32
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
#endif

struct proc_handle {
#ifdef VANILLA_WIN32
    HANDLE handle;
#else
    pid_t pid;
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
