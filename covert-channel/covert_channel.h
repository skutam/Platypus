#ifndef PROJECT_COVERT_CHANNEL_H
#define PROJECT_COVERT_CHANNEL_H

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>

#include "timer.h"
#include "../src/measurement.h"

#define MAX_MEASUREMENTS 10000
#define KEY_LENGTH 11
#define N 200000
#define SEC_TO_SI 1000000
#define RUNS 200

// Define key we want to send
const static char key[] = {1,0,0,1,0,0,1,0, 1,1,0};

static Measurement measurements[MAX_MEASUREMENTS];
static MeasurementResult measurement_results[MAX_MEASUREMENTS];
static size_t measurement_index = 0;

int start_covert_channel(const char *, uint64_t);

#endif //PROJECT_COVERT_CHANNEL_H
