#ifndef TIMER_H_
# define TIMER_H_

# include <time.h>
# include <inttypes.h>
# include <stdbool.h>

struct timespec_compat {
    int64_t tv_sec;
    int64_t tv_nsec;
};

bool can_measure_time(void);
int timer_start(struct timespec_compat *state);
int timer_end(double *time, struct timespec_compat *state);

#endif /* !TIMER_H_ */
