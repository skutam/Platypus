#ifndef PROJECT_TIMER_H
#define PROJECT_TIMER_H

#include <signal.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>

#define TIMER_FREQUENCY_NS 5*1000*1000llu // 5 ms

static timer_t timer_id;

int init_timer(void (*) (int));
void start_timer();
void stop_timer();

#endif //PROJECT_TIMER_H
