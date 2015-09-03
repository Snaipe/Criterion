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

# include <stdbool.h>
# include <stdarg.h>
# include "common.h"
# include "ordered-set.h"
# include "stats.h"

enum criterion_logging_level {
    CRITERION_INFO = 1,
    CRITERION_IMPORTANT,
};

enum criterion_logging_prefix {
    CRITERION_LOGGING_PREFIX_DASHES,
    CRITERION_LOGGING_PREFIX_EQUALS,
    CRITERION_LOGGING_PREFIX_RUN,
    CRITERION_LOGGING_PREFIX_SKIP,
    CRITERION_LOGGING_PREFIX_PASS,
    CRITERION_LOGGING_PREFIX_FAIL,
};

struct criterion_prefix_data {
    const char *prefix;
    const char *color;
};

# ifdef CRITERION_LOGGING_COLORS
#  define CRIT_COLOR_NORMALIZE(Str) (criterion_options.use_ascii ? "" : Str)

#  define CRIT_FG_BOLD  "\e[0;1m"
#  define CRIT_FG_RED   "\e[0;31m"
#  define CRIT_FG_GREEN "\e[0;32m"
#  define CRIT_FG_GOLD  "\e[0;33m"
#  define CRIT_FG_BLUE  "\e[0;34m"
#  define CRIT_RESET    "\e[0m"

#  define FG_BOLD  CRIT_COLOR_NORMALIZE(CRIT_FG_BOLD)
#  define FG_RED   CRIT_COLOR_NORMALIZE(CRIT_FG_RED)
#  define FG_GREEN CRIT_COLOR_NORMALIZE(CRIT_FG_GREEN)
#  define FG_GOLD  CRIT_COLOR_NORMALIZE(CRIT_FG_GOLD)
#  define FG_BLUE  CRIT_COLOR_NORMALIZE(CRIT_FG_BLUE)
#  define RESET    CRIT_COLOR_NORMALIZE(CRIT_RESET)
# endif

extern const struct criterion_prefix_data g_criterion_logging_prefixes[];

# define CRITERION_PREFIX_DASHES (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_DASHES])
# define CRITERION_PREFIX_EQUALS (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_EQUALS])
# define CRITERION_PREFIX_RUN    (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_RUN   ])
# define CRITERION_PREFIX_SKIP   (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_SKIP  ])
# define CRITERION_PREFIX_PASS   (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_PASS  ])
# define CRITERION_PREFIX_FAIL   (&g_criterion_logging_prefixes[CRITERION_LOGGING_PREFIX_FAIL  ])

void criterion_vlog(enum criterion_logging_level level, const char *msg, va_list args);

FORMAT(printf, 3, 4)
void criterion_plog(enum criterion_logging_level level, const struct criterion_prefix_data *prefix, const char *msg, ...);

FORMAT(printf, 2, 3)
void criterion_log(enum criterion_logging_level level, const char *msg, ...);

# define criterion_info(...) criterion_log(CRITERION_INFO, __VA_ARGS__)
# define criterion_important(...) criterion_log(CRITERION_IMPORTANT, __VA_ARGS__)

# define criterion_pinfo(...) criterion_plog(CRITERION_INFO, __VA_ARGS__)
# define criterion_pimportant(...) criterion_plog(CRITERION_IMPORTANT, __VA_ARGS__)

struct criterion_output_provider {
    void (*log_pre_all    )(struct criterion_test_set *set);
    void (*log_pre_suite  )(struct criterion_suite_set *set);
    void (*log_pre_init   )(struct criterion_test *test);
    void (*log_pre_test   )(struct criterion_test *test);
    void (*log_assert     )(struct criterion_assert_stats *stats);
    void (*log_theory_fail)(struct criterion_theory_stats *stats);
    void (*log_test_crash )(struct criterion_test_stats *stats);
    void (*log_other_crash)(struct criterion_test_stats *stats);
    void (*log_post_test  )(struct criterion_test_stats *stats);
    void (*log_post_fini  )(struct criterion_test_stats *stats);
    void (*log_post_suite )(struct criterion_suite_stats *stats);
    void (*log_post_all   )(struct criterion_global_stats *stats);
};

extern struct criterion_output_provider normal_logging;
extern struct criterion_output_provider tap_logging;

#define NORMAL_LOGGING (&normal_logging)
#define TAP_LOGGING    (&tap_logging)

#endif /* !CRITERION_LOGGING_H_ */
