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
#include <stdio.h>
#include <csptr/smalloc.h>

#include "criterion/assert.h"
#include "pipe-internal.h"

FILE *pipe_in(s_pipe_handle *p, enum pipe_opt opts)
{
#ifdef VANILLA_WIN32
    if (opts & PIPE_CLOSE)
        CloseHandle(p->fhs[1]);
    int fd = _open_osfhandle((intptr_t) p->fhs[0], _O_RDONLY);
    if (fd == -1)
        return NULL;
    if (opts & PIPE_DUP)
        fd = _dup(fd);
    FILE *in = _fdopen(fd, "r");
#else
    if (opts & PIPE_CLOSE)
        close(p->fds[1]);
    int fd = p->fds[0];
    if (opts & PIPE_DUP)
        fd = dup(fd);
    FILE *in = fdopen(fd, "r");
#endif
    if (!in)
        return NULL;

    setvbuf(in, NULL, _IONBF, 0);
    return in;
}

FILE *pipe_out(s_pipe_handle *p, enum pipe_opt opts)
{
#ifdef VANILLA_WIN32
    if (opts & PIPE_CLOSE)
        CloseHandle(p->fhs[0]);
    int fd = _open_osfhandle((intptr_t) p->fhs[1], _O_WRONLY);
    if (fd == -1)
        return NULL;
    if (opts & PIPE_DUP)
        fd = _dup(fd);
    FILE *out = _fdopen(fd, "w");
#else
    if (opts & PIPE_CLOSE)
        close(p->fds[0]);
    int fd = p->fds[1];
    if (opts & PIPE_DUP)
        fd = dup(fd);
    FILE *out = fdopen(fd, "w");
#endif
    if (!out)
        return NULL;

    setvbuf(out, NULL, _IONBF, 0);
    return out;
}

int stdpipe_stack(s_pipe_handle *out)
{
#ifdef VANILLA_WIN32
    HANDLE fhs[2];
    SECURITY_ATTRIBUTES attr = {
        .nLength        = sizeof (SECURITY_ATTRIBUTES),
        .bInheritHandle = TRUE
    };
    if (!CreatePipe(fhs, fhs + 1, &attr, 0))
        return -1;
    *out = (s_pipe_handle) { { fhs[0], fhs[1] } };
#else
    int fds[2] = { -1, -1 };
    if (pipe(fds) == -1)
        return -1;
    *out = (s_pipe_handle) { { fds[0], fds[1] } };
#endif
    return 0;
}

s_pipe_handle *stdpipe()
{
    s_pipe_handle *handle = smalloc(sizeof (s_pipe_handle));

    if (stdpipe_stack(handle) < 0)
        return NULL;
    return handle;
}

int stdpipe_options(s_pipe_handle *handle, int id, int noblock)
{
#ifdef VANILLA_WIN32
    HANDLE fhs[2];
    SECURITY_ATTRIBUTES attr = {
        .nLength        = sizeof (SECURITY_ATTRIBUTES),
        .bInheritHandle = TRUE
    };
    char pipe_name[256] = { 0 };
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

    *handle = (s_pipe_handle) { { fhs[0], fhs[1] } };
#else
    (void) id;

    int fds[2] = { -1, -1 };
    if (pipe(fds) == -1)
        return 0;

    if (noblock)
        for (int i = 0; i < 2; ++i)
            fcntl(fds[i], F_SETFL, fcntl(fds[i], F_GETFL) | O_NONBLOCK);

    *handle = (s_pipe_handle) { { fds[0], fds[1] } };
#endif
    return 1;
}

void file_std_redirect(s_pipe_file_handle *to, enum criterion_std_fd fd)
{
#ifdef VANILLA_WIN32
    int stdfd = _open_osfhandle((intptr_t) to->fh, fd == CR_STDIN ? _O_RDONLY : _O_WRONLY);
    if (stdfd == -1)
        cr_assert_fail("Could not redirect standard file descriptor.");

    _close(fd);
    _dup2(stdfd, fd);
    _close(stdfd);

    setvbuf(get_std_file(fd), NULL, _IONBF, 0);

    static int handles[] = {
        [CR_STDIN]  = STD_INPUT_HANDLE,
        [CR_STDOUT] = STD_OUTPUT_HANDLE,
        [CR_STDERR] = STD_ERROR_HANDLE,
    };
    SetStdHandle(handles[fd], to->fh);
#else
    close(fd);
    dup2(to->fd, fd);
    close(to->fd);
#endif
}

void pipe_std_redirect(s_pipe_handle *pipe, enum criterion_std_fd fd)
{
    enum pipe_end end = fd == CR_STDIN ? PIPE_READ : PIPE_WRITE;

    s_pipe_file_handle from;
#ifdef VANILLA_WIN32
    from.fh = pipe->fhs[end];
#else
    from.fd = pipe->fds[end];
#endif

    file_std_redirect(&from, fd);
}

void close_pipe_file_handle(void *ptr, CR_UNUSED void *meta)
{
    s_pipe_file_handle *h = ptr;

#ifdef VANILLA_WIN32
    CloseHandle(h->fh);
#else
    close(h->fd);
#endif
}

#ifdef VANILLA_WIN32
static HANDLE win_dup(HANDLE h)
{
    HANDLE dup;

    DuplicateHandle(GetCurrentProcess(),
            h,
            GetCurrentProcess(),
            &dup,
            0,
            TRUE,
            DUPLICATE_SAME_ACCESS);
    return dup;
}
#endif

s_pipe_file_handle *pipe_out_handle(s_pipe_handle *p, enum pipe_opt opts)
{
#ifdef VANILLA_WIN32
    if (opts & PIPE_CLOSE)
        CloseHandle(p->fhs[0]);
    HANDLE fh = p->fhs[1];
    if (opts & PIPE_DUP)
        fh = win_dup(fh);

    s_pipe_file_handle *h = smalloc(
        .size = sizeof (s_pipe_file_handle),
        .dtor = close_pipe_file_handle);

    h->fh = fh;
    return h;
#else
    if (opts & PIPE_CLOSE)
        close(p->fds[0]);
    int fd = p->fds[1];
    if (opts & PIPE_DUP)
        fd = dup(fd);

    s_pipe_file_handle *h = smalloc(
        .size = sizeof (s_pipe_file_handle),
        .dtor = close_pipe_file_handle);

    h->fd = fd;
    return h;
#endif
}

s_pipe_file_handle *pipe_in_handle(s_pipe_handle *p, enum pipe_opt opts)
{
#ifdef VANILLA_WIN32
    if (opts & PIPE_CLOSE)
        CloseHandle(p->fhs[1]);
    HANDLE fh = p->fhs[0];
    if (opts & PIPE_DUP)
        fh = win_dup(fh);

    s_pipe_file_handle *h = smalloc(
        .size = sizeof (s_pipe_file_handle),
        .dtor = close_pipe_file_handle);

    h->fh = fh;
    return h;
#else
    if (opts & PIPE_CLOSE)
        close(p->fds[1]);
    int fd = p->fds[0];
    if (opts & PIPE_DUP)
        fd = dup(fd);

    s_pipe_file_handle *h = smalloc(
        .size = sizeof (s_pipe_file_handle),
        .dtor = close_pipe_file_handle);

    h->fd = fd;
    return h;
#endif
}

int pipe_write(const void *buf, size_t size, s_pipe_file_handle *pipe)
{
#ifdef VANILLA_WIN32
    DWORD written = 0;
    size_t off = 0;
    while (size > 0) {
        if (!WriteFile(pipe->fh, (const char *) buf + off, size, &written, NULL))
            return -1;
        size -= written;
        off += written;
    }
    if (off > 0)
        return 1;
    return 0;
#else
    ssize_t res = write(pipe->fd, buf, size);
    if (res < 0)
        return -1;
    if (res > 0)
        return 1;
    return 0;
#endif
}

int pipe_read(void *buf, size_t size, s_pipe_file_handle *pipe)
{
#ifdef VANILLA_WIN32
    DWORD read = 0;
    size_t off = 0;
    while (size > 0) {
        if (!ReadFile(pipe->fh, (char *) buf + off, size, &read, NULL))
            return -1;
        size -= read;
        off += read;
    }
    if (off > 0)
        return 1;
    return 0;
#else
    ssize_t res = read(pipe->fd, buf, size);
    if (res < 0)
        return -1;
    if (res > 0)
        return 1;
    return 0;
#endif
}

static s_pipe_handle stdout_redir_;
static s_pipe_handle stderr_redir_;
static s_pipe_handle stdin_redir_;

s_pipe_handle *stdout_redir = &stdout_redir_;
s_pipe_handle *stderr_redir = &stderr_redir_;
s_pipe_handle *stdin_redir  = &stdin_redir_;

static void file_open(s_pipe_file_handle *h, const char *path)
{
#ifdef VANILLA_WIN32
    if (!path)
        path = "nul";
    h->fh = CreateFile(path,
                    GENERIC_READ | GENERIC_WRITE,
                    0,
                    NULL,
                    OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL,
                    NULL);
#else
    if (!path)
        path = "/dev/null";
    h->fd = open(path, O_RDWR);
#endif
}

s_pipe_file_handle *pipe_file_open(const char *path)
{
    s_pipe_file_handle *h = smalloc(
        .size = sizeof (s_pipe_file_handle),
        .dtor = close_pipe_file_handle);

    file_open(h, path);
    return h;
}

static int orig_stdout_fd = -1;
static int orig_stderr_fd = -1;

#ifdef VANILLA_WIN32
static HANDLE orig_stdout_winhandle = INVALID_HANDLE_VALUE;
static HANDLE orig_stderr_winhandle = INVALID_HANDLE_VALUE;
#endif

void cri_silence_outputs(void)
{
#ifdef VANILLA_WIN32
    orig_stdout_fd = _dup(1);
    orig_stderr_fd = _dup(2);

    orig_stdout_winhandle = win_dup(GetStdHandle(STD_OUTPUT_HANDLE));
    orig_stderr_winhandle = win_dup(GetStdHandle(STD_ERROR_HANDLE));
#else
    orig_stdout_fd = dup(1);
    orig_stderr_fd = dup(2);
#endif

    s_pipe_file_handle fh;
    file_open(&fh, NULL);
    file_std_redirect(&fh, CR_STDOUT);

    file_open(&fh, NULL);
    file_std_redirect(&fh, CR_STDERR);
}

void cri_restore_outputs(void)
{
#ifdef VANILLA_WIN32
    _dup2(orig_stdout_fd, 1);
    _close(orig_stdout_fd);
    _dup2(orig_stderr_fd, 2);
    _close(orig_stderr_fd);

    SetStdHandle(STD_OUTPUT_HANDLE, orig_stdout_winhandle);
    SetStdHandle(STD_ERROR_HANDLE, orig_stderr_winhandle);
#else
    dup2(orig_stdout_fd, 1);
    close(orig_stdout_fd);
    dup2(orig_stderr_fd, 2);
    close(orig_stderr_fd);
#endif
}
