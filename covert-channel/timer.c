#include "timer.h"

/**
 * Start the calling of callback function every 5 ms
 */
void start_timer() {
    // Set timer to signalled every 5 ms
    struct itimerspec timer_time;
    memset(&timer_time, 0, sizeof(struct itimerspec));
    timer_time.it_interval.tv_nsec = TIMER_FREQUENCY_NS;
    timer_time.it_value.tv_nsec = TIMER_FREQUENCY_NS;
    assert(timer_settime(timer_id, 0, &timer_time, NULL) == 0);
}

/**
 * Stop the calling of callback function every 5 ms
 */
void stop_timer() {
    // Set timer to 0 to disable
    struct itimerspec timer_time = {0};
    assert(timer_settime(timer_id, 0, &timer_time, NULL) == 0);
}

/**
 * Initialize timer that will call given callback function
 * @param timer_callback Callback function that will be called every 5 ms
 * @return 1 on error, 0 on success
 */
int init_timer(void (*timer_callback) (int)) {
    // Error setting signal alarm callback
    if(signal(SIGALRM, timer_callback) == SIG_ERR) {
        perror("SIGALRM: ");
        return 1;
    }

    // Define sigevent
    struct sigevent clock_sig_event;
    memset(&clock_sig_event, 0, sizeof( struct sigevent));
    clock_sig_event.sigev_notify = SIGEV_SIGNAL;
    clock_sig_event.sigev_signo = SIGALRM;
    clock_sig_event.sigev_notify_attributes = NULL;

    // Create timer
    if (timer_create(CLOCK_REALTIME, &clock_sig_event, &timer_id) != 0) {
        perror("Timer create: ");
        return 1;
    }

    return 0;
}