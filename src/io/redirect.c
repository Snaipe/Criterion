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
#include "criterion/assert.h"
#include "criterion/redirect.h"
#include "compat/pipe.h"

void cr_redirect(enum criterion_std_fd fd_kind, s_pipe_handle *pipe)
{
    fflush(get_std_file(fd_kind));
    if (!stdpipe_options(pipe, fd_kind, fd_kind == CR_STDIN ? 0 : 1))
        cr_assert_fail("Could not redirect standard file descriptor.");

    pipe_std_redirect(pipe, fd_kind);
}

void cr_redirect_stdout(void)
{
    cr_redirect(CR_STDOUT, stdout_redir);
}

void cr_redirect_stderr(void)
{
    cr_redirect(CR_STDERR, stderr_redir);
}

void cr_redirect_stdin(void)
{
    cr_redirect(CR_STDIN, stdin_redir);
}

FILE *cr_get_redirected_stdout(void)
{
    static FILE *f;

    if (!f) {
        f = pipe_in(stdout_redir, PIPE_NOOPT);
        if (!f)
            cr_assert_fail("Could not get redirected stdout read end.");
    }
    return f;
}

FILE *cr_get_redirected_stderr(void)
{
    static FILE *f;

    if (!f) {
        f = pipe_in(stderr_redir, PIPE_NOOPT);
        if (!f)
            cr_assert_fail("Could not get redirected stderr read end.");
    }
    return f;
}

FILE *cr_get_redirected_stdin(void)
{
    static FILE *f;

    if (!f) {
        f = pipe_out(stdin_redir, PIPE_NOOPT);
        if (!f)
            cr_assert_fail("Could not get redirected stdin write end.");
    }
    return f;
}
