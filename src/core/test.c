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
#include "criterion/internal/test.h"
#include "core/abort.h"
#include "core/stats.h"
#include "core/worker.h"
#include "core/report.h"
#include "compat/time.h"
#include "protocol/protocol.h"
#include "protocol/messages.h"
#include "io/event.h"

extern const struct criterion_test  *criterion_current_test;
extern const struct criterion_suite *criterion_current_suite;

static void send_event(int phase) {
    criterion_protocol_msg msg = criterion_message(phase,
            .phase = phase,
            .name = (char *) criterion_current_test->name,
        );
    criterion_message_set_id(msg);
    cr_send_to_runner(&msg);
}

static CR_INLINE void nothing(void) {}

void criterion_internal_test_setup(void) {
    const struct criterion_suite *suite = criterion_current_suite;
    const struct criterion_test *test = criterion_current_test;

    send_event(criterion_protocol_phase_kind_SETUP);
    if (suite->data)
        (suite->data->init ? suite->data->init : nothing)();
    (test->data->init ? test->data->init : nothing)();
}

void criterion_internal_test_main(void (*fn)(void)) {
    const struct criterion_test *test = criterion_current_test;

    send_event(criterion_protocol_phase_kind_MAIN);

    struct timespec_compat ts;
    if (!setjmp(g_pre_test)) {
        timer_start(&ts);
        if (!test->data->param_) {
            fn();
        } else {
            void(*param_test_func)(void *) = (void(*)(void*)) fn;
            param_test_func(g_worker_context.param->ptr);
        }
    }

    double elapsed_time;
    if (!timer_end(&elapsed_time, &ts))
        elapsed_time = -1;

    send_event(criterion_protocol_phase_kind_TEARDOWN);
}

void criterion_internal_test_teardown(void) {
    const struct criterion_suite *suite = criterion_current_suite;
    const struct criterion_test *test = criterion_current_test;

    (test->data->fini ? test->data->fini : nothing)();
    if (suite->data)
        (suite->data->fini ? suite->data->fini : nothing)();

    send_event(criterion_protocol_phase_kind_END);
}

