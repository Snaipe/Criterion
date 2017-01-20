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
#include <string.h>
#include "abort.h"
#include "protocol/protocol.h"
#include "protocol/messages.h"
#include "criterion/internal/asprintf-compat.h"
#include "criterion/criterion.h"
#include "criterion/options.h"
#include "io/event.h"
#include "debugbreak.h"

jmp_buf g_pre_test;
size_t cri_nb_asserts;

void criterion_abort_test(void)
{
    if (criterion_options.crash)
        debug_break();

    cri_longjmp(g_pre_test, 1);
}

void criterion_skip_test(const char *format, ...)
{
    char *msg = NULL;

    if (*format) {
        va_list args;
        va_start(args, format);
        cr_vasprintf(&msg, format, args);
        va_end(args);
    }

    criterion_protocol_msg skip_msg = criterion_message(phase,
                    .phase = criterion_protocol_phase_kind_SKIP,
                    .name = (char *) criterion_current_test->name,
                    .message = msg,
                    );
    criterion_message_set_id(skip_msg);
    cr_send_to_runner(&skip_msg);
    free(msg);

    exit(0);
}

void criterion_continue_test(void)
{
    if (criterion_options.crash)
        debug_break();
}

void criterion_test_die(const char *msg, ...)
{
    va_list vl;

    va_start(vl, msg);
    char *formatted_msg = NULL;
    int res = cr_vasprintf(&formatted_msg, msg, vl);
    va_end(vl);

    if (res < 0)
        abort();

    criterion_protocol_msg abort_msg = criterion_message(phase,
                    .phase = criterion_protocol_phase_kind_ABORT,
                    .name = (char *) criterion_current_test->name,
                    .message = formatted_msg,
                    );
    criterion_message_set_id(abort_msg);
    cr_send_to_runner(&abort_msg);

    free(formatted_msg);

    exit(0);
}
