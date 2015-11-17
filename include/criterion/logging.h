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
#ifndef CRITERION_LOGGING_H_
# define CRITERION_LOGGING_H_

# ifdef __cplusplus
#  include <cstdarg>
using std::va_list;
# else
#  include <stdbool.h>
#  include <stdarg.h>
# endif
# include "internal/common.h"
# include "internal/ordered-set.h"
# include "stats.h"

enum criterion_logging_level {
    CRITERION_INFO = 1,
    CRITERION_IMPORTANT,

    CRITERION_LOG_LEVEL_QUIET = 1 << 30,
};

enum criterion_logging_prefix {
    CRITERION_LOGGING_PREFIX_DASHES,
    CRITERION_LOGGING_PREFIX_EQUALS,
    CRITERION_LOGGING_PREFIX_RUN,
    CRITERION_LOGGING_PREFIX_SKIP,
    CRITERION_LOGGING_PREFIX_PASS,
    CRITERION_LOGGING_PREFIX_FAIL,
    CRITERION_LOGGING_PREFIX_ERR,
};

struct criterion_prefix_data {
    const char *prefix;
    const char *color;
};

# ifdef CRITERION_LOGGING_COLORS
#  define CRIT_COLOR_NORMALIZE(Str) (criterion_options.use_ascii ? "" : Str)

#  define CRIT_FG_BOLD  "\33[0;1m"
#  define CRIT_FG_RED   "\33[0;31m"
#  define CRIT_FG_GREEN "\33[0;32m"
#  define CRIT_FG_GOLD  "\33[0;33m"
#  define CRIT_FG_BLUE  "\33[0;34m"
#  define CRIT_RESET    "\33[0m"

#  define CR_FG_BOLD  CRIT_COLOR_NORMALIZE(CRIT_FG_BOLD)
#  define CR_FG_RED   CRIT_COLOR_NORMALIZE(CRIT_FG_RED)
#  define CR_FG_GREEN CRIT_COLOR_NORMALIZE(CRIT_FG_GREEN)
#  define CR_FG_GOLD  CRIT_COLOR_NORMALIZE(CRIT_FG_GOLD)
#  define CR_FG_BLUE  CRIT_COLOR_NORMALIZE(CRIT_FG_BLUE)
#  define CR_RESET    CRIT_COLOR_NORMALIZE(CRIT_RESET)
# endif

CR_BEGIN_C_API

extern const struct criterion_prefix_data g_criterion_logging_prefixes[];

# define CRITERION_PREFIX_DASHES (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_DASHES])
# define CRITERION_PREFIX_EQUALS (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_EQUALS])
# define CRITERION_PREFIX_RUN    (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_RUN   ])
# define CRITERION_PREFIX_SKIP   (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_SKIP  ])
# define CRITERION_PREFIX_PASS   (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_PASS  ])
# define CRITERION_PREFIX_FAIL   (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_FAIL  ])
# define CRITERION_PREFIX_ERR    (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_ERR   ])

CR_API void criterion_vlog(enum criterion_logging_level level, const char *msg, va_list args);

CR_FORMAT(printf, 3, 4)
CR_API void criterion_plog(enum criterion_logging_level level, const struct criterion_prefix_data *prefix, const char *msg, ...);

CR_FORMAT(printf, 2, 3)
CR_API void criterion_log(enum criterion_logging_level level, const char *msg, ...);

# define criterion_info(...) criterion_log(CRITERION_INFO, __VA_ARGS__)
# define criterion_important(...) criterion_log(CRITERION_IMPORTANT, __VA_ARGS__)

# define criterion_pinfo(...) criterion_plog(CRITERION_INFO, __VA_ARGS__)
# define criterion_pimportant(...) criterion_plog(CRITERION_IMPORTANT, __VA_ARGS__)

# define criterion_perror(...) criterion_plog(CRITERION_IMPORTANT, CRITERION_PREFIX_ERR, __VA_ARGS__)

struct criterion_logger {
    void (*log_pre_all      )(struct criterion_test_set *set);
    void (*log_pre_suite    )(struct criterion_suite_set *set);
    void (*log_pre_init     )(struct criterion_test *test);
    void (*log_pre_test     )(struct criterion_test *test);
    void (*log_assert       )(struct criterion_assert_stats *stats);
    void (*log_theory_fail  )(struct criterion_theory_stats *stats);
    void (*log_test_timeout )(struct criterion_test_stats *stats);
    void (*log_test_crash   )(struct criterion_test_stats *stats);
    void (*log_test_abort   )(struct criterion_test_stats *stats, const char *msg);
    void (*log_other_crash  )(struct criterion_test_stats *stats);
    void (*log_abnormal_exit)(struct criterion_test_stats *stats);
    void (*log_post_test    )(struct criterion_test_stats *stats);
    void (*log_post_fini    )(struct criterion_test_stats *stats);
    void (*log_post_suite   )(struct criterion_suite_stats *stats);
    void (*log_post_all     )(struct criterion_global_stats *stats);
};

extern struct criterion_logger normal_logging;

CR_END_C_API

#define CR_NORMAL_LOGGING (&normal_logging)

#endif /* !CRITERION_LOGGING_H_ */
