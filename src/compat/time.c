#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>
#include "criterion/common.h"
#include "criterion/options.h"
#include "compat/time.h"
#include "compat/posix.h"

#define GIGA 1000000000

#if defined(__unix__) && !defined(__CYGWIN__)

# ifdef CLOCK_MONOTONIC_RAW
#  define CLOCK CLOCK_MONOTONIC_RAW
# else
#  define CLOCK CLOCK_MONOTONIC
# endif

extern __attribute__ ((weak)) int clock_gettime(clockid_t, struct timespec *);

#elif defined(__APPLE__)
# include <mach/clock.h>
# include <mach/mach.h>
# include <pthread.h>
#elif defined(_WIN32) || defined(__CYGWIN__)
# define VC_EXTRALEAN
# define WIN32_LEAN_AND_MEAN
# include <Windows.h>
#endif

bool can_measure_time(void) {
    if (!criterion_options.measure_time)
        return false;

#if defined(__unix__) && !defined(__CYGWIN__)
    return clock_gettime != NULL;
#else
    return true;
#endif
}

int gettime_compat(struct timespec_compat *ts) {
#if defined(__APPLE__)
    clock_serv_t cclock;
    mach_timespec_t mts;

    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    int res = clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);

    *ts = (struct timespec_compat) { mts.tv_sec, mts.tv_nsec };
    return res > 0 ? -1 : 0;
#elif defined(_WIN32) || defined(__CYGWIN__)
    LARGE_INTEGER freq, count;
    if (!QueryPerformanceFrequency(&freq)
        || !QueryPerformanceCounter(&count))
        return -1;

    int64_t sec  = count.QuadPart / freq.QuadPart;
    int64_t nano = (int64_t) ((double) count.QuadPart * GIGA / (double) freq.QuadPart) % GIGA;

    *ts = (struct timespec_compat) { sec, nano };
    return 0;
#elif defined(__unix__)
    if (!can_measure_time()) {
        errno = ENOTSUP;
        return -1;
    }

    struct timespec ts_;
    int res = clock_gettime(CLOCK, &ts_);

    *ts = (struct timespec_compat) { ts_.tv_sec, ts_.tv_nsec };
    return res;
#else
    return -1;
#endif
}

int timer_start(struct timespec_compat *state) {
    return gettime_compat(state) == -1 ? 0 : 1;
}

int timer_end(double *time, struct timespec_compat *state) {
    struct timespec_compat last;
    if (gettime_compat(&last) == -1)
        return 0;

    *time = (last.tv_sec - state->tv_sec) + (last.tv_nsec - state->tv_nsec) / (double) GIGA;
    return 1;
}

#if defined(_WIN32) || defined(__CYGWIN__)
DWORD WINAPI win_raise_timeout(LPVOID ptr) {
    uint64_t *nanos = (uint64_t*) ptr;
    Sleep(*nanos / 1000000);
    TerminateProcess(GetCurrentProcess(), CR_EXCEPTION_TIMEOUT);
    return 0;
}
#endif

#if defined(__APPLE__)
void *wait_and_raise(void *ptr) {
    uint64_t *nanos = ptr;
    struct timespec elapsed = {
        .tv_sec = *nanos / GIGA,
        .tv_nsec = *nanos % GIGA,
    };
    free(nanos);
    if (!nanosleep(&elapsed, NULL))
        raise(SIGPROF);
    return NULL;
}
#endif

int setup_timeout(uint64_t nanos) {
#if   defined(__APPLE__)
    uint64_t *nanos_copy = malloc(sizeof (uint64_t));
    *nanos_copy = nanos;

    pthread_t thread;
    int res = pthread_create(&thread, NULL, wait_and_raise, nanos_copy);

    return res ? -1 : 0;
#elif defined(_WIN32) || defined(__CYGWIN__)
    uint64_t *nanos_copy = malloc(sizeof (uint64_t));
    *nanos_copy = nanos;

    HANDLE thread = CreateThread(NULL, 0, win_raise_timeout, nanos_copy, 0, NULL);
    if (thread == NULL)
        return -1;
    CloseHandle(thread);
    return 0;
#elif defined(__unix__)
    timer_t timer;
    int res = timer_create(CLOCK_MONOTONIC, &(struct sigevent) {
            .sigev_notify = SIGEV_SIGNAL,
            .sigev_signo  = SIGPROF,
        }, &timer);

    if (res == -1)
        return res;

    struct itimerspec schedule = {
        .it_value = { .tv_sec = nanos / GIGA, .tv_nsec = (nanos % GIGA) }
    };

    return timer_settime(timer, 0, &schedule, NULL);
#else
    errno = ENOTSUP;
    return -1;
#endif
}
