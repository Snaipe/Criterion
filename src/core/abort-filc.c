#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "abort.h"
#include "filc-simple.h"

jmp_buf g_pre_test;

static int g_failed;
static int g_skipped;

void cri_filc_reset_test_state(void)
{
    g_failed = 0;
    g_skipped = 0;
}

void cri_filc_mark_failed(void)
{
    g_failed = 1;
}

void cri_filc_mark_skipped(void)
{
    g_skipped = 1;
}

int cri_filc_test_failed(void)
{
    return g_failed;
}

int cri_filc_test_skipped(void)
{
    return g_skipped;
}

void criterion_abort_test(void)
{
    cri_filc_mark_failed();
    cri_longjmp(g_pre_test, 1);
}

void criterion_skip_test(const char *format, ...)
{
    if (format && *format) {
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        fputc('\n', stderr);
        va_end(args);
    }
    cri_filc_mark_skipped();
    cri_longjmp(g_pre_test, 1);
}

void criterion_continue_test(void)
{
}

void criterion_test_die(const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    fputc('\n', stderr);
    va_end(args);
    cri_filc_mark_failed();
    cri_longjmp(g_pre_test, 1);
}
