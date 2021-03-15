/**
 * Provides pthread barrier support on macOS (and any other OS with pthreads
 * that lacks it). These are simply vimplemented using pthread conditions as
 * shown in the book and at:
 * https://blog.albertarmea.com/post/47089939939/using-pthreadbarrier-on-mac-os-x
 * 
 * Including this file on Linux on other systems just includes pthreads.h.
 */

#pragma once

#include <pthread.h>

#if !defined(PTHREAD_BARRIER_SERIAL_THREAD) && (!defined(_POSIX_BARRIERS) || _POSIX_BARRIERS < 0)

#include <errno.h>

#define PTHREAD_BARRIER_SERIAL_THREAD 1
#define _POSIX_BARRIERS 1

typedef int pthread_barrierattr_t;
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int trip_count;
} pthread_barrier_t;

// see https://linux.die.net/man/3/pthread_barrier_init, attr not supported
int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count) {
    if (attr != NULL || count == 0) { errno = EINVAL; return -1; }
    int err;
    if ((err = pthread_mutex_init(&barrier->mutex, 0)) < 0) { return err; }
    if ((err = pthread_cond_init(&barrier->cond, 0)) < 0) { pthread_mutex_destroy(&barrier->mutex); return err; }
    barrier->trip_count = count;
    barrier->count = 0;
    return 0;
}

// see https://linux.die.net/man/3/pthread_barrier_destroy
int pthread_barrier_destroy(pthread_barrier_t *barrier) {
    int err1 = pthread_cond_destroy(&barrier->cond);
    int err2 = pthread_mutex_destroy(&barrier->mutex);
    return err1 < 0 ? err1 : err2;
}

// see https://linux.die.net/man/3/pthread_barrier_wait
int pthread_barrier_wait(pthread_barrier_t *barrier) {
    int err, err2, retval = 0;
    if ((err = pthread_mutex_lock(&barrier->mutex)) < 0) { return err; }
    if (++barrier->count >= barrier->trip_count) {
        barrier->count = 0;
        retval = PTHREAD_BARRIER_SERIAL_THREAD;
        err = pthread_cond_broadcast(&barrier->cond);
    } else {
        err = pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }
    err2 = pthread_mutex_unlock(&barrier->mutex);
    return err < 0 ? err : err2 < 0 ? err2 : retval;
}

#endif
