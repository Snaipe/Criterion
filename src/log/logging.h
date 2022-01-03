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
#ifndef LOGGING_H_
#define LOGGING_H_

#ifdef __cplusplus
# include <cstdarg>
using std::va_list;
#else
# include <stdbool.h>
# include <stdarg.h>
#endif
#include "criterion/logging.h"

enum criterion_logging_prefix {
    CRITERION_LOGGING_PREFIX_DASHES,
    CRITERION_LOGGING_PREFIX_EQUALS,
    CRITERION_LOGGING_PREFIX_RUN,
    CRITERION_LOGGING_PREFIX_SKIP,
    CRITERION_LOGGING_PREFIX_PASS,
    CRITERION_LOGGING_PREFIX_FAIL,
    CRITERION_LOGGING_PREFIX_ERR,
    CRITERION_LOGGING_PREFIX_WARN,
    CRITERION_LOGGING_PREFIX_DEBUG,
};

struct criterion_prefix_data {
    const char *prefix;
    const char *color;
};

#define CRIT_COLOR_NORMALIZE(Str)    (criterion_options.color ? Str : "")

#define CRIT_FG_BOLD     "\33[0;1m"
#define CRIT_FG_RED      "\33[0;31m"
#define CRIT_FG_GREEN    "\33[0;32m"
#define CRIT_FG_GOLD     "\33[0;33m"
#define CRIT_FG_BLUE     "\33[0;34m"
#define CRIT_FG_GRAY     "\33[1;30m"
#define CRIT_RESET       "\33[0m"

#define CR_FG_BOLD       CRIT_COLOR_NORMALIZE(CRIT_FG_BOLD)
#define CR_FG_RED        CRIT_COLOR_NORMALIZE(CRIT_FG_RED)
#define CR_FG_GREEN      CRIT_COLOR_NORMALIZE(CRIT_FG_GREEN)
#define CR_FG_GOLD       CRIT_COLOR_NORMALIZE(CRIT_FG_GOLD)
#define CR_FG_BLUE       CRIT_COLOR_NORMALIZE(CRIT_FG_BLUE)
#define CR_FG_GRAY       CRIT_COLOR_NORMALIZE(CRIT_FG_GRAY)
#define CR_RESET         CRIT_COLOR_NORMALIZE(CRIT_RESET)

extern const struct criterion_prefix_data g_criterion_logging_prefixes[];

#define CRITERION_PREFIX_DASHES    (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_DASHES])
#define CRITERION_PREFIX_EQUALS    (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_EQUALS])
#define CRITERION_PREFIX_RUN       (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_RUN])
#define CRITERION_PREFIX_SKIP      (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_SKIP])
#define CRITERION_PREFIX_PASS      (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_PASS])
#define CRITERION_PREFIX_FAIL      (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_FAIL])
#define CRITERION_PREFIX_ERR       (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_ERR])
#define CRITERION_PREFIX_WARN      (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_WARN])
#define CRITERION_PREFIX_DEBUG     (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_DEBUG])

#undef criterion_log
#undef criterion_info
#undef criterion_pinfo
#undef criterion_important
#undef criterion_pimportant
#undef criterion_perror

CR_API void criterion_vlog(enum criterion_logging_level level, const char *msg, va_list args);

CR_FORMAT(printf, 3, 4)
CR_API void criterion_plog(enum criterion_logging_level level, const struct criterion_prefix_data *prefix, const char *msg, ...);

CR_FORMAT(printf, 2, 3)
CR_API void criterion_log(enum criterion_logging_level level, const char *msg, ...);

#define criterion_info(...)          criterion_log(CRITERION_INFO, __VA_ARGS__)
#define criterion_important(...)     criterion_log(CRITERION_IMPORTANT, __VA_ARGS__)

#define criterion_pinfo(...)         criterion_plog(CRITERION_INFO, __VA_ARGS__)
#define criterion_pimportant(...)    criterion_plog(CRITERION_IMPORTANT, __VA_ARGS__)

#define criterion_perror(...)        criterion_plog(CRITERION_IMPORTANT, CRITERION_PREFIX_ERR, __VA_ARGS__)

void cr_log_noformat(enum criterion_severity severity, const char *out);
void criterion_log_noformat(enum criterion_severity severity, const char *msg);

#endif /* !LOGGING_H_ */
