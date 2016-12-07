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
#include <errno.h>

#include "mutex.h"

#ifdef HAVE_WIN32_SYNCHAPI
# include <synchapi.h>
#endif

#ifdef HAVE_WIN32_THREADS
BOOL CALLBACK init_once(PINIT_ONCE once, PVOID param, PVOID *ctx)
{
    (void) once;
    (void) ctx;
    return !cri_mutex_init(param);
}
#endif

int cri_mutex_init_once(struct cri_mutex *mutex)
{
#ifdef HAVE_WIN32_THREADS
    static INIT_ONCE once;
    BOOL rc = InitOnceExecuteOnce(&once, init_once, mutex, NULL);
    return rc ? 0 : -EINVAL;
#elif HAVE_PTHREADS
    static pthread_mutex_t once = PTHREAD_MUTEX_INITIALIZER;

    int rc = -pthread_mutex_lock(&once);
    if (rc)
        return rc;

    if (mutex->init)
        goto end;

    rc = cri_mutex_init(mutex);

end:
    pthread_mutex_unlock(&once);
    return rc;
#else
    return -ENOTSUP;
#endif
}

int cri_mutex_init(struct cri_mutex *mutex)
{
#ifdef HAVE_WIN32_THREADS
    InitializeCriticalSection(&mutex->mutex);
    return 0;
#elif HAVE_PTHREADS
    int rc = pthread_mutex_init(&mutex->mutex, NULL);
    if (!rc)
        mutex->init = 1;
    return -rc;
#else
    return -ENOTSUP;
#endif
}

int cri_mutex_lock(struct cri_mutex *mutex)
{
#ifdef HAVE_WIN32_THREADS
    EnterCriticalSection(&mutex->mutex);
    return 0;
#elif HAVE_PTHREADS
    return -pthread_mutex_lock(&mutex->mutex);
#else
    return -ENOTSUP;
#endif
}

int cri_mutex_unlock(struct cri_mutex *mutex)
{
#ifdef HAVE_WIN32_THREADS
    LeaveCriticalSection(&mutex->mutex);
    return 0;
#elif HAVE_PTHREADS
    return -pthread_mutex_unlock(&mutex->mutex);
#else
    return -ENOTSUP;
#endif
}
