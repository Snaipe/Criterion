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
#ifndef POSIX_COMPAT_H_
# define POSIX_COMPAT_H_

#if defined(_WIN32) && !defined(__CYGWIN__)
# define VANILLA_WIN32
#endif

# if !defined(_POSIX_SOURCE)
#  define _POSIX_SOURCE 1
#  define TMP_POSIX
# endif
# include <stdio.h>
# ifdef TMP_POSIX
#  undef _POSIX_SOURCE
#  undef TMP_POSIX
# endif

# ifdef VANILLA_WIN32
#  define WEXITSTATUS(Status) (((Status) & 0xFF00) >> 8)
#  define WTERMSIG(Status)    ((Status) & 0x7F)
#  define WIFEXITED(Status)   (WTERMSIG(Status) == 0)
#  define WIFSIGNALED(Status) (((signed char) (WTERMSIG(Status) + 1) >> 1) > 0)
# else
#  include <sys/wait.h>
# endif

#include <criterion/types.h>

struct proc_handle;
typedef struct proc_handle s_proc_handle;

struct pipe_handle;
typedef struct pipe_handle s_pipe_handle;

struct worker_context {
    struct criterion_test *test;
    struct criterion_suite *suite;
    f_worker_func func;
    struct pipe_handle *pipe;
};

extern struct worker_context g_worker_context;

int resume_child(void);

s_pipe_handle *stdpipe();
FILE *pipe_in(s_pipe_handle *p);
FILE *pipe_out(s_pipe_handle *p);

s_proc_handle *fork_process();
void wait_process(s_proc_handle *handle, int *status);

s_proc_handle *get_current_process();
bool is_current_process(s_proc_handle *proc);

# ifdef _WIN32
void *get_win_section_start(const char *section);
void *get_win_section_end(const char *section);
#  define CR_STRINGIFY_(Param) #Param
#  define CR_STRINGIFY(Param) CR_STRINGIFY_(Param)
#  define GET_SECTION_START(Name) get_win_section_start(CR_STRINGIFY(Name))
#  define GET_SECTION_END(Name)   get_win_section_end(CR_STRINGIFY(Name))
# elif defined(__APPLE__)
void *get_osx_section_start(const char *section);
void *get_osx_section_end(const char *section);
#  define CR_STRINGIFY_(Param) #Param
#  define CR_STRINGIFY(Param) CR_STRINGIFY_(Param)
#  define GET_SECTION_START(Name) get_osx_section_start(CR_STRINGIFY(Name))
#  define GET_SECTION_END(Name)   get_osx_section_end(CR_STRINGIFY(Name))
# else
#  define GET_SECTION_START(Name) SECTION_START(Name)
#  define GET_SECTION_END(Name)   SECTION_END(Name)
# endif

const char *basename_compat(const char *str);

#endif /* !POSIX_COMPAT_H_ */
