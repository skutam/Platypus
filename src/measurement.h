#ifndef PROJECT_MEASUREMENT_H
#define PROJECT_MEASUREMENT_H

#include <fcntl.h>          // open
#include <assert.h>         // assert
#include <unistd.h>         // sleep, pread
#include <stdint-gcc.h>     // uint64_t
#include <stdlib.h>         // strtol
#include <stdio.h>          // fprintf

#define RDTSC_ITTER 3
#define POWERCAP_OFFSET 0x611
#define POWERCAP_RAPL_CONVERTER 1000000.0f

uint64_t rdtsc();
uint64_t get_rdtsc_scale();

static int fd = 0;
static uint64_t rdtsc_scale = 0;

/**
 * Structure to hold joules and time
 */
typedef struct Measurement {
    uint64_t joules;
    uint64_t time;
} Measurement;

/**
 * Structure to hold joules time and power
 */
typedef struct MeasurementResult {
    double joules;          // joules
    double time;            // seconds
    uint64_t count;         // Number of added results
} MeasurementResult;

void init_measurement();
Measurement take_measurement();
MeasurementResult calc_results(const Measurement *, const Measurement *);
void append_calc_results(MeasurementResult *, const Measurement *, const Measurement *);
void append_results(MeasurementResult *mr, MeasurementResult *mr_);

#endif //PROJECT_MEASUREMENT_H
