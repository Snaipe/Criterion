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
/**
 * @file
 * @brief Logging functions
 *****************************************************************************/
#ifndef CRITERION_LOGGING_H_
#define CRITERION_LOGGING_H_

#include "internal/common.h"
#include "internal/ordered-set.h"
#include "internal/deprecation.h"
#include "stats.h"

CR_BEGIN_C_API

enum criterion_logging_level {
    CRITERION_INFO = 1,
    CRITERION_IMPORTANT,

    CRITERION_LOG_LEVEL_QUIET = 1 << 30,
};

enum criterion_severity {
    CR_LOG_INFO,
    CR_LOG_WARNING,
    CR_LOG_ERROR,
};

/**
 * Prints a log message
 *
 * One can use this function, but it is more convenient to use the 3 macros.
 *
 * @param[in] severity Severity level of the log message
 * @param[in] msg printf like format string
 * @param[in] ... Additional arguments depending on msg
 *
 *****************************************************************************/
CR_FORMAT(printf, 2, 3)
CR_API void cr_log(enum criterion_severity severity, const char *msg, ...);

/**
 * Prints a info message
 *
 * Call: cr_log_info(msg, ...)\n
 * Prints a info message defined by msg and the following optional parameters
 *
 * @param[in] msg printf like format string
 * @param[in] ... Additional arguments depending on msg
 *
 *****************************************************************************/
#define cr_log_info(...)    cr_log(CR_LOG_INFO, __VA_ARGS__)

/**
 * Prints a warning message
 *
 * Call: cr_log_warn(msg, ...)\n
 * Prints a warning message defined by msg and the following optional parameters
 *
 * @param[in] msg printf like format string
 * @param[in] ... Additional arguments depending on msg
 *
 *****************************************************************************/
#define cr_log_warn(...)    cr_log(CR_LOG_WARNING, __VA_ARGS__)

/**
 * Prints a error message
 *
 * Call: cr_log_error(msg, ...)\n
 * Prints a error message defined by msg and the following optional parameters
 *
 * @param[in] msg printf like format string
 * @param[in] ... Additional arguments depending on msg
 *
 *****************************************************************************/
#define cr_log_error(...)    cr_log(CR_LOG_ERROR, __VA_ARGS__)

struct criterion_logger {
    void (*log_pre_all      )(struct criterion_test_set *set);
    void (*log_pre_suite    )(struct criterion_suite_set *set);
    void (*log_pre_init     )(struct criterion_suite *suite, struct criterion_test *test);
    void (*log_pre_test     )(struct criterion_suite *suite, struct criterion_test *test);
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
    void (*log_message      )(enum criterion_severity, const char *msg);
};

extern struct criterion_logger normal_logging;

CR_END_C_API

#define CR_NORMAL_LOGGING    (&normal_logging)

#ifdef __cplusplus
# include <sstream>

namespace criterion
{ namespace logging
  {
  static void(*const log)(enum criterion_severity, const char *, ...) = cr_log;

  class streambuf : public std::stringbuf {
  public:
      streambuf(enum criterion_severity severity__)
          : std::stringbuf(), severity__(severity__)
      {}

      virtual int sync() override
      {
          criterion::logging::log(severity__, "%s", str().c_str());
          str(std::string());
          return 0;
      }
  private:
      enum criterion_severity severity__;
  };

  class stream : public std::ostream {
  public:
      stream(enum criterion_severity severity__)
          : std::ostream(&buf), buf(severity__)
      {}
  private:
      streambuf buf;
  };

  static stream info { CR_LOG_INFO };
  static stream warn { CR_LOG_WARNING };
  static stream error { CR_LOG_ERROR };
  } }
#endif

/* Deprecated old logging system, schedule removal for 3.0 */
#ifndef CRITERION_NO_COMPAT

# define criterion_log(_, ...)           CR_DEPRECATED("criterion_log is deprecated, please use cr_log instead.") cr_log_info(__VA_ARGS__)
# define criterion_info(...)             CR_DEPRECATED("criterion_info is deprecated, please use cr_log_info instead.") cr_log_info(__VA_ARGS__)
# define criterion_pinfo(_, ...)         CR_DEPRECATED("criterion_pinfo is deprecated, please use cr_log_info instead.") cr_log_info(__VA_ARGS__)
# define criterion_important(...)        CR_DEPRECATED("criterion_important is deprecated, please use cr_log_info instead.") cr_log_info(__VA_ARGS__)
# define criterion_pimportant(_, ...)    CR_DEPRECATED("criterion_pimportant is deprecated, please use cr_log_info instead.") cr_log_info(__VA_ARGS__)
# define criterion_perror(...)           CR_DEPRECATED("criterion_perror is deprecated, please use cr_log_error instead.") cr_log_error(__VA_ARGS__)

#endif /* !CRITERION_NO_COMPAT */

#endif /* !CRITERION_LOGGING_H_ */
