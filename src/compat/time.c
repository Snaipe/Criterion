/*
 * The MIT License (MIT)
 *
 * Copyright © 2016 Franklin "Snaipe" Mathieu <http://snai.pe/>
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
#include <errno.h>

#include "config.h"
#include "time.h"

#define KILO    1000ull
#define MEGA    (KILO * 1000ull)
#define GIGA    (MEGA * 1000ull)

#if defined (HAVE_GETTIMEOFDAY)
# include <sys/time.h>
#endif

#if defined (HAVE_CLOCK_GETTIME)
# include <time.h>
#endif

#if defined (__APPLE__)
# include <mach/clock.h>
# include <mach/mach.h>
# include <pthread.h>
#endif

#if defined (_WIN32) || defined (__CYGWIN__)
# include <windows.h>
#endif

#if defined (HAVE_CLOCK_GETTIME)
# if defined (HAVE_CLOCK_MONOTONIC_RAW)
#  define CLOCK    CLOCK_MONOTONIC_RAW
# else
#  define CLOCK    CLOCK_MONOTONIC
# endif
#endif

uint64_t cri_timestamp_monotonic(void)
{
#if defined (HAVE_CLOCK_GETTIME)
    struct timespec now;
    clock_gettime(CLOCK, &now);
    return now.tv_sec * GIGA + now.tv_nsec;
#elif defined (__APPLE__)
    clock_serv_t cclock;
    mach_timespec_t mts;

    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    int res = clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);

    return mts.tv_sec * GIGA + mts.tv_nsec;
#elif defined (_WIN32) || defined (__CYGWIN__)
    LARGE_INTEGER freq, count;
    if (!QueryPerformanceFrequency(&freq)
            || !QueryPerformanceCounter(&count))
        return -1;

    uint64_t sec  = count.QuadPart / freq.QuadPart;
    uint64_t nano = ((count.QuadPart * GIGA) / freq.QuadPart) % GIGA;
    return sec * GIGA + nano;
#else
    errno = ENOTSUP;
    return (uint64_t) -1;
#endif
}
