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
#ifndef PIPE_H_
#define PIPE_H_

#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "log/logging.h"

struct pipe_handle;
typedef struct pipe_handle s_pipe_handle;

struct pipe_file_handle;
typedef struct pipe_file_handle s_pipe_file_handle;

enum pipe_end {
    PIPE_READ = 0,
    PIPE_WRITE = 1,
};

enum criterion_std_fd {
    CR_STDIN = 0,
    CR_STDOUT = 1,
    CR_STDERR = 2,
};

enum pipe_opt {
    PIPE_NOOPT = 0,
    PIPE_DUP = 1 << 0,
    PIPE_CLOSE = 1 << 1,
};

s_pipe_handle *stdpipe();
FILE *pipe_in(s_pipe_handle *p, enum pipe_opt opts);
FILE *pipe_out(s_pipe_handle *p, enum pipe_opt opts);

s_pipe_file_handle *pipe_out_handle(s_pipe_handle *p, enum pipe_opt opts);
s_pipe_file_handle *pipe_in_handle(s_pipe_handle *p, enum pipe_opt opts);

int stdpipe_options(s_pipe_handle *pipe, int id, int noblock);
void pipe_std_redirect(s_pipe_handle *pipe, enum criterion_std_fd fd);

int pipe_write(const void *buf, size_t size, s_pipe_file_handle *pipe);
int pipe_read(void *buf, size_t size, s_pipe_file_handle *pipe);

CR_INLINE FILE *get_std_file(enum criterion_std_fd fd_kind)
{
    switch (fd_kind) {
        case CR_STDIN:  return stdin;
        case CR_STDOUT: return stdout;
        case CR_STDERR: return stderr;
    }
    criterion_perror("get_std_file: invalid parameter.\n");
    abort();
}

extern s_pipe_handle *stdout_redir;
extern s_pipe_handle *stderr_redir;
extern s_pipe_handle *stdin_redir;

s_pipe_file_handle *pipe_file_open(const char *path);

void cri_silence_outputs(void);
void cri_restore_outputs(void);

#endif /* !PIPE_H_ */
