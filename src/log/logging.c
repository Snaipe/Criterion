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
#define CRITERION_LOGGING_COLORS
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "criterion/logging.h"
#include "criterion/options.h"
#include "i18n.h"

#define LOG_FORMAT "[%1$s%2$s%3$s] %4$s"

const struct criterion_prefix_data g_criterion_logging_prefixes[] = {
    [CRITERION_LOGGING_PREFIX_DASHES]   = { "----", CRIT_FG_BLUE  },
    [CRITERION_LOGGING_PREFIX_EQUALS]   = { "====", CRIT_FG_BLUE  },
    [CRITERION_LOGGING_PREFIX_RUN]      = { "RUN ", CRIT_FG_BLUE  },
    [CRITERION_LOGGING_PREFIX_SKIP]     = { "SKIP", CRIT_FG_GOLD  },
    [CRITERION_LOGGING_PREFIX_PASS]     = { "PASS", CRIT_FG_GREEN },
    [CRITERION_LOGGING_PREFIX_FAIL]     = { "FAIL", CRIT_FG_RED   },
    { NULL, NULL }
};

void criterion_plog(enum criterion_logging_level level, const struct criterion_prefix_data *prefix, const char *msg, ...) {
    va_list args;

    if (level < criterion_options.logging_threshold)
        return;

    char formatted_msg[1024];
    va_start(args, msg);
    vsnprintf(formatted_msg, sizeof formatted_msg, msg, args);
    va_end(args);

    fprintf(stderr, _(LOG_FORMAT),
            CRIT_COLOR_NORMALIZE(prefix->color),
            prefix->prefix,
            RESET,
            formatted_msg);
}

void criterion_log(enum criterion_logging_level level, const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    criterion_vlog(level, msg, args);
    va_end(args);
}

void criterion_vlog(enum criterion_logging_level level, const char *msg, va_list args) {
    if (level < criterion_options.logging_threshold)
        return;

    vfprintf(stderr, msg, args);
}
