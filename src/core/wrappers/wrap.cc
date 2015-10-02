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
#include <exception>
#include "criterion/assert.h"
#include "criterion/event.h"

extern "C" {

#include "core/abort.h"
#include "core/report.h"
#include "core/worker.h"
#include "compat/time.h"
#include "wrap.h"
#include "common.h"

static INLINE void nothing(void) {}

void cpp_wrap(struct criterion_test *test, struct criterion_suite *suite) {

    send_event(PRE_INIT, NULL, 0);
    try {
        if (suite->data)
            (suite->data->init ? suite->data->init : nothing)();
        (test->data->init ? test->data->init : nothing)();
    } catch (const std::exception &e) {
        criterion_test_die("Caught an unexpected exception during the test initialization: %s.", e.what());
    } catch (...) {
        criterion_test_die("Caught some unexpected exception during the test initialization.");
    }
    send_event(PRE_TEST, NULL, 0);

    struct timespec_compat ts;
    if (!setjmp(g_pre_test)) {
        timer_start(&ts);
        if (test->test) {
            try {
                if (!test->data->param_) {
                    test->test();
                } else {
                    void(*param_test_func)(void *) = (void(*)(void*)) test->test;
                    param_test_func(g_worker_context.param->ptr);
                }
            } catch (const std::exception &e) {
                criterion_test_die("Caught an unexpected exception during the test execution: %s.", e.what());
            } catch (...) {
                criterion_test_die("Caught some unexpected exception during the test execution.");
            }
        }
    }

    double elapsed_time;
    if (!timer_end(&elapsed_time, &ts))
        elapsed_time = -1;

    send_event(POST_TEST, &elapsed_time, sizeof (double));
    try {
        (test->data->fini ? test->data->fini : nothing)();
        if (suite->data)
            (suite->data->fini ? suite->data->fini : nothing)();
    } catch (const std::exception &e) {
        criterion_test_die("Caught an unexpected exception during the test finalization: %s.", e.what());
    } catch (...) {
        criterion_test_die("Caught some unexpected exception during the test finalization.");
    }
    send_event(POST_FINI, NULL, 0);

}

}
