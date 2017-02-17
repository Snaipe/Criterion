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
#ifndef MUTEX_H_
#define MUTEX_H_

#include "config.h"

#ifdef HAVE_WIN32_THREADS
# include <windows.h>

struct cri_mutex {
    CRITICAL_SECTION mutex;
};
#elif HAVE_PTHREADS
# include <pthread.h>

struct cri_mutex {
    pthread_mutex_t mutex;
    int init;
};
#endif

int cri_mutex_init_once(struct cri_mutex *mutex);
int cri_mutex_init(struct cri_mutex *mutex);
int cri_mutex_lock(struct cri_mutex *mutex);
int cri_mutex_unlock(struct cri_mutex *mutex);

#endif /* !MUTEX_H_ */
