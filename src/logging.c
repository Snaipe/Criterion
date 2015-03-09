#include <stdio.h>
#include <stdarg.h>
#include "criterion/logging.h"

enum criterion_logging_level logging_threshold = CRITERION_IMPORTANT;

void criterion_log(enum criterion_logging_level level, const char *msg, ...) {
    va_list args;

    if (level < logging_threshold)
        return;

    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
}
