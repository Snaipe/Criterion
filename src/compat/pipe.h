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
#ifndef PIPE_H_
# define PIPE_H_

# include "common.h"

struct pipe_handle;
typedef struct pipe_handle s_pipe_handle;

enum pipe_end {
    PIPE_READ = 0,
    PIPE_WRITE = 1,
};

enum criterion_std_fd {
    CR_STDIN = 0,
    CR_STDOUT = 1,
    CR_STDERR = 2,
};

s_pipe_handle *stdpipe();
FILE *pipe_in(s_pipe_handle *p, int do_close);
FILE *pipe_out(s_pipe_handle *p, int do_close);

int stdpipe_options(s_pipe_handle *pipe, int id, int noblock);
void pipe_std_redirect(s_pipe_handle *pipe, enum criterion_std_fd fd);

INLINE FILE* get_std_file(int fd_kind) {
    switch (fd_kind) {
        case CR_STDIN:  return stdin;
        case CR_STDOUT: return stdout;
        case CR_STDERR: return stderr;
    }
    return NULL;
}

extern s_pipe_handle *stdout_redir;
extern s_pipe_handle *stderr_redir;
extern s_pipe_handle *stdin_redir;

#endif /* !PIPE_H_ */
