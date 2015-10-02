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
#include <string.h>
#include "abort.h"
#include "criterion/asprintf-compat.h"
#include "io/event.h"

jmp_buf g_pre_test;

void criterion_abort_test(void) {
    longjmp(g_pre_test, 1);
}

void criterion_test_die(const char *msg, ...) {
    va_list vl;
    va_start(vl, msg);
    char *formatted_msg = NULL;
    int res = cr_vasprintf(&formatted_msg, msg, vl);
    va_end(vl);

    if (res < 0)
        abort();

    size_t *buf = malloc(sizeof (size_t) + res + 1);
    *buf = res + 1;
    memcpy(buf + 1, formatted_msg, res + 1);

    criterion_send_event(TEST_ABORT, buf, sizeof(size_t) + res + 1);
    free(buf);
    free(formatted_msg);

    exit(0);
}
