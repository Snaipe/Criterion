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
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <csptr/smalloc.h>
#include "core/worker.h"
#include "core/runner.h"
#include "protocol/protocol.h"
#include "protocol/messages.h"
#include "io/event.h"
#include "process.h"
#include "internal.h"
#include "pipe-internal.h"
#include "alloc.h"

#include <signal.h>

#ifdef VANILLA_WIN32
# include <tchar.h>
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

static int get_win_status(HANDLE handle) {
    DWORD exit_code;
    GetExitCodeProcess(handle, &exit_code);
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
    return sig ? sig : exit_code << 8;
}
#else
# include <pthread.h>
#endif

struct worker_context g_worker_context = {.test = NULL};

#ifdef VANILLA_WIN32
struct full_context {
    struct criterion_test test;
    struct criterion_test_extra_data test_data;
    struct criterion_suite suite;
    struct criterion_test_extra_data suite_data;
    cr_worker_func func;
    struct test_single_param param;
    HANDLE sync;
    DWORD extra_size;
};

static TCHAR g_mapping_name[] = TEXT("WinCriterionWorker_%lu");

static struct full_context local_ctx;
#endif

#if defined(__unix__) || defined(__APPLE__)
# ifndef __GNUC__
#  error Unsupported compiler. Use GCC or Clang under *nixes.
# endif

static void handle_sigchld_pump(void) {
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        int result = WIFEXITED(status)
            ? criterion_protocol_death_result_type_NORMAL
            : criterion_protocol_death_result_type_CRASH;
        int code = WIFEXITED(status)
            ? WEXITSTATUS(status)
            : WTERMSIG(status);

        criterion_protocol_msg msg = criterion_message(death,
                .result = result,
                .has_status = true,
                .status = code,
            );

        msg.id.pid = pid;
        cr_send_to_runner(&msg);
    }
}

/*
 * This child reaping logic is a dirty hack to prevent deadlocks
 * when the pipe is overflowing and a child terminates.
 *
 * (Windows doesn't have this issue as the waitpid logic is threaded by
 * RegisterWaitForSingleObject)
 *
 * REMOVE WHEN REFACTORING I/O LAYER
 */
static pthread_t child_pump;
static bool child_pump_running;

static void *chld_pump_thread_main(void *nil) {
    do {
        handle_sigchld_pump();
        usleep(1000);
    } while (child_pump_running);

    return nil;
}
#endif

void init_proc_compat(void) {
#ifndef VANILLA_WIN32
    child_pump_running = true;
    int err = pthread_create(&child_pump, NULL, chld_pump_thread_main, NULL);
    if (err) {
        perror(0);
        exit(1);
    }
#endif
}

void free_proc_compat(void) {
#ifndef VANILLA_WIN32
    child_pump_running = false;
    pthread_join(child_pump, NULL);
#endif
}

#ifdef VANILLA_WIN32
struct wait_context {
    HANDLE wait_handle;
    HANDLE proc_handle;
};

static void CALLBACK handle_child_terminated(PVOID lpParameter,
                                             CR_UNUSED BOOLEAN TimerOrWaitFired) {

    assert(!TimerOrWaitFired);

    struct wait_context *wctx = lpParameter;

    int status = get_win_status(wctx->proc_handle);

    int64_t pid = (int64_t) GetProcessId(wctx->proc_handle);

    int result = WIFEXITED(status)
        ? criterion_protocol_death_result_type_NORMAL
        : criterion_protocol_death_result_type_CRASH;
    int code = WIFEXITED(status)
        ? WEXITSTATUS(status)
        : WTERMSIG(status);

    criterion_protocol_msg msg = criterion_message(death,
            .result = result,
            .has_status = true,
            .status = code,
        );

    msg.id.pid = pid;
    cr_send_to_runner(&msg);

    HANDLE whandle = wctx->wait_handle;
    free(lpParameter);
    UnregisterWaitEx(whandle, NULL);
}
#endif

int resume_child(void) {
#ifdef VANILLA_WIN32
    TCHAR mapping_name[128];
    _sntprintf(mapping_name, 128, g_mapping_name, GetCurrentProcessId());

    HANDLE sharedMem = OpenFileMapping(
           FILE_MAP_ALL_ACCESS,
           FALSE,
           mapping_name);

    if (sharedMem == NULL) {
        init_inheritable_heap();
        return 0;
    }

    struct full_context *ctx = (struct full_context *) MapViewOfFile(sharedMem,
           FILE_MAP_ALL_ACCESS,
           0,
           0,
           sizeof (struct full_context));

    if (ctx == NULL) {
        CloseHandle(sharedMem);
        exit(-1);
    }

    local_ctx = *ctx;
    UnmapViewOfFile(ctx);

    struct test_single_param *param = NULL;
    if (local_ctx.param.size != 0) {
        ctx = (struct full_context*) MapViewOfFile(sharedMem,
               FILE_MAP_ALL_ACCESS,
               0,
               0,
               sizeof (struct full_context) + local_ctx.extra_size);

        if (ctx == NULL) {
            CloseHandle(sharedMem);
            exit(-1);
        }

        param = malloc(sizeof (struct test_single_param) + local_ctx.param.size);
        *param = (struct test_single_param) {
            .size = local_ctx.param.size,
            .ptr = param + 1,
        };
        memcpy(param + 1, ctx + 1, param->size);
        UnmapViewOfFile(ctx);
    }

    CloseHandle(sharedMem);

    g_worker_context = (struct worker_context) {
        .test = &local_ctx.test,
        .suite = &local_ctx.suite,
        .func = local_ctx.func,
        .param = param,
    };

    local_ctx.test.data  = &local_ctx.test_data;
    local_ctx.suite.data = &local_ctx.suite_data;

    SetEvent(local_ctx.sync);

    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);

    run_worker(&g_worker_context);
    free(param);
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

    SECURITY_ATTRIBUTES inherit_handle = {
        .nLength = sizeof (SECURITY_ATTRIBUTES),
        .bInheritHandle = TRUE
    };

    // Create the synchronization event
    HANDLE sync = CreateEvent(&inherit_handle, FALSE, FALSE, NULL);
    if (sync == NULL)
        return (void *) -1;

    // Create the suspended child process
    wchar_t filename[MAX_PATH];
    GetModuleFileNameW(NULL, filename, MAX_PATH);

    if (!CREATE_SUSPENDED_(filename, GetCommandLineW(), si, info))
        return (void *) -1;

    // Copy context over
    TCHAR mapping_name[128];
    _sntprintf(mapping_name, 128, g_mapping_name, info.dwProcessId);

    DWORD mapping_size = sizeof (struct full_context);
    if (g_worker_context.param)
        mapping_size += g_worker_context.param->size;

    HANDLE sharedMem = CreateFileMapping(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_READWRITE,
            0,
            mapping_size,
            mapping_name);

    if (sharedMem == NULL || GetLastError() == ERROR_ALREADY_EXISTS)
        return (void *) -1;

    struct full_context *ctx = (struct full_context *) MapViewOfFile(sharedMem,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            mapping_size);

    if (ctx == NULL) {
        CloseHandle(sharedMem);
        return (void *) -1;
    }

    *ctx = (struct full_context) {
        .test      = *g_worker_context.test,
        .test_data = *g_worker_context.test->data,
        .suite     = *g_worker_context.suite,
        .func      = g_worker_context.func,
        .sync      = sync,
    };

    if (g_worker_context.param) {
        ctx->extra_size = g_worker_context.param->size;
        ctx->param = *g_worker_context.param;
        memcpy(ctx + 1, g_worker_context.param->ptr, g_worker_context.param->size);
    }

    if (g_worker_context.suite->data)
        ctx->suite_data = *g_worker_context.suite->data;

    inherit_heap(info.hProcess);

    if (ResumeThread(info.hThread) == (DWORD) -1)
        goto failure;

    // wait until the child has initialized itself
    HANDLE handles[] = { info.hProcess, sync };
    DWORD wres = WaitForMultipleObjects(sizeof (handles) / sizeof (HANDLE), handles, FALSE, INFINITE);
    if (wres == WAIT_OBJECT_0)
        goto failure;

    CloseHandle(info.hThread);

    UnmapViewOfFile(ctx);
    CloseHandle(sharedMem);

    struct wait_context *wctx = malloc(sizeof (struct wait_context));
    *wctx = (struct wait_context) {
        .proc_handle = info.hProcess,
    };

    RegisterWaitForSingleObject(
            &wctx->wait_handle,
            info.hProcess,
            handle_child_terminated,
            wctx,
            INFINITE,
            WT_EXECUTELONGFUNCTION | WT_EXECUTEONLYONCE);

    s_proc_handle *handle = smalloc(sizeof (s_proc_handle));
    *handle = (s_proc_handle) { info.hProcess };
    return handle;

failure:
    UnmapViewOfFile(ctx);
    CloseHandle(sharedMem);
    CloseHandle(info.hThread);
    CloseHandle(info.hProcess);
    return (void *) -1;

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

unsigned long long get_process_id(void) {
#ifdef VANILLA_WIN32
    return (unsigned long long) GetCurrentProcessId();
#else
    return (unsigned long long) getpid();
#endif
}

unsigned long long get_process_id_of(s_proc_handle *proc) {
#ifdef VANILLA_WIN32
    return (unsigned long long) GetProcessId(proc->handle);
#else
    return (unsigned long long) proc->pid;
#endif
}
