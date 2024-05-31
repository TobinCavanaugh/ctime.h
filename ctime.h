#ifndef CTIME
#define CTIME

#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>

/// Macro for defining function pointers
#define ctime_func_ptr(a) void(*a)(void)

/// Mutex for ctime_active_timers_count
static pthread_mutex_t ctime_active_timers_mutex = PTHREAD_MUTEX_INITIALIZER;

/// How many timers are actively running. DO NOT MODIFY
static uint16_t ctime_active_timers_count = 0;


/// Args for ctime_internal_start_timer
typedef struct {
    /// The amount of time in ms to wait
    uint64_t ms;

    /// The function to be called when the timer ends
    ctime_func_ptr(function);
} ctime_internal_args;

/// This is our internal function for our pthread to call.
/// If you're reading this, probably dont mess with this
/// \param rawArgs The ctime_internal_args to read the data from
void ctime_internal_start_timer(void *rawArgs) {
    //Read our void * rawArgs as 8 byte values
    //We could do implicit casting by setting the argument
    //to be of this type, but that feels kinda icky... idk
    ctime_internal_args *args = (ctime_internal_args *) rawArgs;

    //Get our wait duration
    uint64_t ms = args->ms;

    //Get our function pointer
    ctime_func_ptr(timerDone) = args->function;

    //Wait for necessary time
    usleep((useconds_t) (ms * 1000LL));

    timerDone();

    //Decrease our active timers, we gotta mutex this or the world ends
    pthread_mutex_lock(&ctime_active_timers_mutex);
    ctime_active_timers_count--;
    pthread_mutex_unlock(&ctime_active_timers_mutex);

    //We have to free this
    free(rawArgs);

    pthread_detach(pthread_self());
}

/// Create a timer that calls function when the elapsed time (ms) finishes
/// \param ms The time in milliseconds to wait before the function will be invoked.
/// The actual wait time may be +/- up to 15ms due to thread creation delays
/// \param function The function to be invoked
void ctime_start(int ms, ctime_func_ptr(function)) {

    //Initialize our arguments
    ctime_internal_args *args = malloc(sizeof(ctime_internal_args));
    args->ms = ms;
    args->function = function;

    ctime_active_timers_count++;

    //Start our thread
    pthread_t p;
    int err = pthread_create(&p, NULL, (void *(*)(void *)) &ctime_internal_start_timer, (void *) args);

    if (err != 0) {
        printf("CTIME ERROR: FAILED TO CREATE THREAD: %d, Thread ID: %llu\n", err, p);
        fflush(stdout);
        ctime_active_timers_count--;
        free(args);
    }
}

/// This will allow you to prevent the program from closing before all timers are
/// complete. Like so: ```while (!ctime_timers_stopped()) {}```
/// \return 1 if all timers are stopped.
uint8_t ctime_timers_stopped() {
    //Not sure if this is strictly necessary, because we're reading the value...
    pthread_mutex_unlock(&ctime_active_timers_mutex);
    int val = ctime_active_timers_count;
    if (val < 0) {
        ctime_active_timers_count = 0;
    }
    pthread_mutex_lock(&ctime_active_timers_mutex);

    return val <= 0;
}

#endif