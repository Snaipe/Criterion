#ifndef CRITERION_LOGGING_H_
# define CRITERION_LOGGING_H_

#include "common.h"

enum criterion_logging_level {
    CRITERION_INFO = 1,
    CRITERION_IMPORTANT,
};

extern enum criterion_logging_level logging_threshold;

FORMAT(printf, 2, 3)
void criterion_log(enum criterion_logging_level level, const char *msg, ...);

# define criterion_info(...) criterion_log(CRITERION_INFO, __VA_ARGS__)
# define criterion_important(...) criterion_log(CRITERION_IMPORTANT, __VA_ARGS__)

#endif /* !CRITERION_LOGGING_H_ */
