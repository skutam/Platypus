#include "measurement.h"

/**
 * Return number of CPU cycles since last reset
 * @return Number of CPU cycles since last reset
 */
uint64_t rdtsc() {
    uint64_t a, d;
    // Wait until all load and store operations finish
    asm volatile("mfence");
    // Get current timestamp saved in 64 bit value, EAX[second half] EDX[first half]
    asm volatile("rdtsc" : "=a"(a), "=d"(d));
    a = (d << 32) | a;
    // Wait here until last instruction load and stores finishes
    asm volatile("mfence");
    return a;
}

uint64_t get_rdtsc_scale() {
    return rdtsc_scale;
}

/**
 * Read how many joules were consumed since the start of the system
 * @return Number of joules consumed since the start of the system
 */
static uint64_t read_rapl() {
    uint64_t data;
    pread(fd, &data, sizeof data, POWERCAP_OFFSET);
    return data;
}

/**
 * Run few timings to calibrate the ticks/s
 */
static void calibrate_rdtsc() {
    uint64_t start, end;
    for (int i = 0; i < RDTSC_ITTER; ++i) {
        start = rdtsc();
        sleep(1);
        end = rdtsc();
        rdtsc_scale += end - start;
    }

    rdtsc_scale /= RDTSC_ITTER;
}

/**
 * Open joules file and calibrate time timer
 */
void init_measurement() {
    // Open MSR file
    fd = open("/dev/cpu/0/msr", O_RDONLY);
    // Check if file is open
    assert(fd > 0);
    // Calibrate timer
    calibrate_rdtsc();
}

/**
 * Take measurement of power consumption and current time in ticks
 * @return The taken measurement
 */
Measurement take_measurement() {
    Measurement m;
    m.joules = read_rapl();
    m.time = rdtsc();
    return m;
}

/**
 * Calculate the difference between two taken measurements
 * @param start Measurement taken before executing command we wanted to take a measurement of
 * @param end Measurement taken after executing command we wanted to take a measurement of
 * @return Measurement result
 */
MeasurementResult calc_results(const Measurement *start, const Measurement *end) {
    // Create new structure
    MeasurementResult mr;
    // Calculate joules required
    mr.joules = (double)(end->joules - start->joules) / POWERCAP_RAPL_CONVERTER;
    // Time it took in seconds
    mr.time = (double)(end->time - start->time) / (double)rdtsc_scale;
    mr.count = 0;
    return mr;
}

/**
 * Add result from another Measurement Result
 * @param mr Target
 * @param mr_ Append from this value
 */
void append_results(MeasurementResult *mr, MeasurementResult *mr_) {
    mr->joules += mr_->joules;
    mr->time += mr_->time;
}

/**
 * Add the difference of Measurements to the Measurement Result
 * @param mr Target Measurement Result we will add to
 * @param start Measurement before energy consuming operation
 * @param end measurement after energy consuming operation
 */
void append_calc_results(MeasurementResult *mr, const Measurement *start, const Measurement *end) {
    MeasurementResult mr_tmp = calc_results(start, end);
    mr->joules += mr_tmp.joules;
    mr->time += mr_tmp.time;
    mr->count += 1;
}
