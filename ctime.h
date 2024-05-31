
#ifndef CTIME
#define CTIME

#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>


static pthread_mutex_t ctime_active_timers_mutex = PTHREAD_MUTEX_INITIALIZER;
static uint16_t ctime_active_timers_count = 0;

#define ctime_func_ptr(a) void(*a)(void)

///Total size 24B -> 32B
typedef struct {
    uint64_t ms;

    ctime_func_ptr(function);

} internal_ctime_args;

//Function prototypes
void internal_start_timer(void *rawArgs);

void create_timer(int ms, void (*function)(void));


/// This is our internal function for our pthread to call.
/// If you're reading this, probably dont mess with this
/// \param rawArgs
void internal_start_timer(void *rawArgs) {
    //Read our void * rawArgs as 8 byte values
    internal_ctime_args *args = (internal_ctime_args *) rawArgs;

//    printf("%llu\n", args->function);
//    printf("%llu\n", *args->function);

    //Get our wait duration
    uint64_t ms = args->ms;

//    printf("%d,", ctime_active_timers_count);

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
/// \param ms The time in milliseconds to wait before the function will be invoked
/// \param function The function to be invoked
void create_timer(int ms, ctime_func_ptr(function)) {

//    uint64_t *data = malloc(sizeof(uint64_t) * CTIME_ARR_LEN);
    internal_ctime_args *args = calloc(1, sizeof(internal_ctime_args));

    //Load in our data
    args->ms = ms;
    args->function = function;

    ctime_active_timers_count++;

    //Start our thread
    pthread_t p;
    int err = pthread_create(&p, NULL, &internal_start_timer, (void *) args);

    if (err != 0) {
        printf("CTIME ERROR: FAILED TO CREATE THREAD: %d\n", err);
        fflush(stdout);
        ctime_active_timers_count--;
    }
}

int all_timers_stopped() {
    int val = 1;
    pthread_mutex_unlock(&ctime_active_timers_mutex);
    val = ctime_active_timers_count;
//    printf("%d~\n", val);
    pthread_mutex_lock(&ctime_active_timers_mutex);

    return val <= 0;
}

#endif