#ifndef CRITERION_OPTIONS_H_
# define CRITERION_OPTIONS_H_

# include "logging.h"

struct criterion_options {
    enum criterion_logging_level logging_threshold;
    bool enable_tap_format;
    bool no_early_exit;
    bool always_succeed;
};

extern struct criterion_options criterion_options;

#endif /*!CRITERION_OPTIONS_H_ */
