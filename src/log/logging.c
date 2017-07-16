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
#define CRITERION_LOGGING_COLORS
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "criterion/criterion.h"
#include "criterion/options.h"
#include "compat/process.h"
#include "core/runner.h"
#include "core/report.h"
#include "log/logging.h"
#include "protocol/protocol.h"
#include "protocol/messages.h"
#include "string/fmt.h"
#include "string/i18n.h"

#ifdef ENABLE_NLS
# define LOG_FORMAT      "[%1$s%2$s%3$s] %4$s"
# define ERROR_FORMAT    "[%1$s%2$s%3$s] %4$s%5$s%6$s%7$s"
#else
# define LOG_FORMAT      "[%s%s%s] %s"
# define ERROR_FORMAT    "[%s%s%s] %s%s%s%s"
#endif

const struct criterion_prefix_data g_criterion_logging_prefixes[] = {
    [CRITERION_LOGGING_PREFIX_DASHES]   = { "----", CRIT_FG_BLUE  },
    [CRITERION_LOGGING_PREFIX_EQUALS]   = { "====", CRIT_FG_BLUE  },
    [CRITERION_LOGGING_PREFIX_RUN]      = { "RUN ", CRIT_FG_BLUE  },
    [CRITERION_LOGGING_PREFIX_SKIP]     = { "SKIP", CRIT_FG_GOLD  },
    [CRITERION_LOGGING_PREFIX_PASS]     = { "PASS", CRIT_FG_GREEN },
    [CRITERION_LOGGING_PREFIX_FAIL]     = { "FAIL", CRIT_FG_RED   },
    [CRITERION_LOGGING_PREFIX_ERR]      = { "ERR ", CRIT_FG_RED   },
    [CRITERION_LOGGING_PREFIX_WARN]     = { "WARN", CRIT_FG_GOLD  },
    [CRITERION_LOGGING_PREFIX_DEBUG]    = { "----", CRIT_FG_GRAY  },
                                          { NULL,   NULL          }
};

void criterion_log_noformat(enum criterion_severity severity, const char *msg)
{
    static const struct criterion_prefix_data *prefixes[] = {
        [CR_LOG_INFO]       = CRITERION_PREFIX_DASHES,
        [CR_LOG_WARNING]    = CRITERION_PREFIX_WARN,
        [CR_LOG_ERROR]      = CRITERION_PREFIX_ERR,
    };

    static enum criterion_logging_level severity_to_level[] = {
        [CR_LOG_INFO]       = CRITERION_INFO,
        [CR_LOG_WARNING]    = CRITERION_IMPORTANT,
        [CR_LOG_ERROR]      = CRITERION_IMPORTANT,
    };

    if (severity_to_level[severity] < criterion_options.logging_threshold)
        return;

    const struct criterion_prefix_data *prefix = prefixes[severity];
    if (severity == CR_LOG_ERROR) {
        fprintf(stderr, _(ERROR_FORMAT),
                CRIT_COLOR_NORMALIZE(prefix->color),
                prefix->prefix,
                CR_RESET,
                CR_FG_RED,
                CR_FG_BOLD,
                msg,
                CR_RESET);
    } else {
        fprintf(stderr, _(LOG_FORMAT),
                CRIT_COLOR_NORMALIZE(prefix->color),
                prefix->prefix,
                CR_RESET,
                msg);
    }
    fprintf(stderr, "\n");
}

void criterion_plog(enum criterion_logging_level level, const struct criterion_prefix_data *prefix, const char *msg, ...)
{
    va_list args;

    if (level < criterion_options.logging_threshold)
        return;

    char formatted_msg[1024];
    va_start(args, msg);
    vsnprintf(formatted_msg, sizeof formatted_msg, msg, args);
    va_end(args);

    if (prefix == &g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_ERR]) {
        fprintf(stderr, _(ERROR_FORMAT),
                CRIT_COLOR_NORMALIZE(prefix->color),
                prefix->prefix,
                CR_RESET,
                CR_FG_RED,
                CR_FG_BOLD,
                formatted_msg,
                CR_RESET);
    } else {
        fprintf(stderr, _(LOG_FORMAT),
                CRIT_COLOR_NORMALIZE(prefix->color),
                prefix->prefix,
                CR_RESET,
                formatted_msg);
    }
}

void criterion_log(enum criterion_logging_level level, const char *msg, ...)
{
    va_list args;

    va_start(args, msg);
    criterion_vlog(level, msg, args);
    va_end(args);
}

void criterion_vlog(enum criterion_logging_level level, const char *msg, va_list args)
{
    if (level < criterion_options.logging_threshold)
        return;

    vfprintf(stderr, msg, args);
}

void cr_log_noformat(enum criterion_severity severity, const char *out)
{
    if (cri_is_runner) {
        log(message, severity, out);
    } else {
        criterion_protocol_msg msg = criterion_message(message,
                        .severity = (criterion_protocol_log_level) severity,
                        .message = (char *) out);

        criterion_message_set_id(msg);
        cr_send_to_runner(&msg);
    }
}

void cr_log(enum criterion_severity severity, const char *msg, ...)
{
    va_list args;
    char *out = NULL;

    va_start(args, msg);
    int res = cr_vasprintf(&out, msg, args);
    va_end(args);

    if (res == -1) {
        cr_log_noformat(CR_LOG_ERROR, "Could not format log message");
        abort();
    }

    cr_log_noformat(severity, out);
    free(out);
}
