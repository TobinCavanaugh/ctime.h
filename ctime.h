#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef CTIME
#define CTIME

uint16_t ctime_active_timers = 0;

#define CTIME_ARR_LEN 3
#define CTIME_THREAD_ARR_IND 0
#define CTIME_MS_ARR_IND 1
#define CTIME_FUN_ARR_IND 2

/// This is our internal function for our pthread to call.
/// If you're reading this, probably dont mess with this
/// \param rawArgs
void internal_start_timer(void *rawArgs) {
    //Read our void * rawArgs as 8 byte values
    const uint64_t *args = (uint64_t *) rawArgs;

    //Get our thread id
    pthread_t thread = args[CTIME_THREAD_ARR_IND];

    //Get our wait duration
    uint64_t ms = args[CTIME_MS_ARR_IND];

    //Get our function pointer
    void (*timerDone)(void) = (void (*)(void)) args[CTIME_FUN_ARR_IND];

    //Wait for necessary time
    usleep(ms * 1000);

    //Complete stuff
    timerDone();
    ctime_active_timers--;
    pthread_join(thread, NULL);

    //We have to free this
    free(rawArgs);
}

/// Create a timer that calls function when the elapsed time (ms) finishes
/// \param ms The time in milliseconds to wait before the function will be invoked
/// \param function The function to be invoked
void create_timer(int ms, void (*function)(void)) {

    uint64_t *data = malloc(sizeof(uint64_t) * CTIME_ARR_LEN);

    //TODO Some weirdness here with pthread...
    pthread_t thread_id;

    //Load in our data
    data[CTIME_THREAD_ARR_IND] = (uint64_t) thread_id;
    data[CTIME_MS_ARR_IND] = ms;
    data[CTIME_FUN_ARR_IND] = (uint64_t) *function;

    ctime_active_timers++;

    //Start our thread
    pthread_create(&thread_id, NULL, (void *(*)(void *)) internal_start_timer, data);
    data[CTIME_THREAD_ARR_IND] = thread_id;
}

#endif