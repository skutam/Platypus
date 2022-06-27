#include "covert_channel.h"

static void run_nop() {
    for (int i = 0; i < N; ++i) {
        asm volatile("nop");
    }
}

static void run_rdtscs() {
    volatile uint64_t a, d;

    for (int i = 0; i < N; ++i) {
        asm volatile("rdtsc": "=a"(a), "=d"(d));
    }
}

static void run_clflush() {
    volatile uint64_t r = 25;

    for (int i = 0; i < N; ++i) {
        asm volatile("clflush (%0)":: "r"(&r));
    }
}

static void run_fscale() {
    for (int i = 0; i < N; ++i) {
        asm volatile("fscale");
    }
}

static void run_mov() {
    volatile uint64_t src = 159;
    volatile uint64_t dst;

    for (int i = 0; i < N; ++i) {
        asm volatile("mov %1, %0": "=r"(dst): "r"(src));
    }
}

/**
 * Function that will measure each 5ms , so we can receive the data from sender
 * @param sig
 */
void timer_callback() {
    // Increase the number of measurements, 10000 is the default
    assert(measurement_index < MAX_MEASUREMENTS);
    measurements[measurement_index++] = take_measurement();
}

void static init_measurement_results() {
    for (int i = 0; i < MAX_MEASUREMENTS; ++i) {
        measurement_results[i].time = 0;
        measurement_results[i].joules = 0;
        measurement_results[i].count = 0;
    }
}

int start_covert_channel(const char *inst, uint64_t bps) {
    // Initialize timer
    if (init_timer(timer_callback) != 0) {
        return 1;
    }

    // Pointer to instruction function
    void (*inst_func) (void) = NULL;

    // Choose instruction for covert channel
    if (strcmp(inst, "nop") == 0) {
        inst_func = run_nop;
    } else if (strcmp(inst, "rdtsc") == 0) {
        inst_func = run_rdtscs;
    } else if (strcmp(inst, "clflush") == 0) {
        inst_func = run_clflush;
    } else if (strcmp(inst, "fscale") == 0) {
        inst_func = run_fscale;
    } else if (strcmp(inst, "mov") == 0) {
        inst_func = run_mov;
    }

    if (inst_func == NULL) {
        fprintf(stderr, "Invalid argument, choose on of these instructions [nop|time|clflush|fscale|mov].");
        return 1;
    }

    // Initialize measurement results
    init_measurement_results();

    size_t min_measurement_index = INT64_MAX;

    // Run it multiple times, so we have a mean value
    for (int i = 0; i < RUNS; ++i) {
        // Reset index of Measurement buffer
        measurement_index = 0;

        start_timer();
        for (int key_i = 0; key_i < KEY_LENGTH; ++key_i) {
            // Start doing power consuming operation
            volatile uint64_t run_until = (uint64_t)((double)rdtsc() + ((1.0f / (double)bps) * (double)get_rdtsc_scale()));
            if (key[key_i]) {
                while (rdtsc() < run_until) {
                    inst_func();
                }
                // Idle
            } else {
                while (rdtsc() < run_until) {
                    usleep((unsigned int)(1.0f / (double)bps) * SEC_TO_SI);
                }
            }
        }
        stop_timer();

        // Wait until the timer finishes
        usleep(10 * 1000);

        // Add taken measurements into the result array
        for (size_t j = 0; j < (measurement_index - 1); ++j) {
            append_calc_results(&(measurement_results[j]), &(measurements[j]), &(measurements[j + 1]));
        }

        min_measurement_index = min_measurement_index > (measurement_index - 1) ? measurement_index : min_measurement_index;
    }

    // Print to standard error output the average power we calculated
    for (size_t j = 0; j < min_measurement_index; ++j) {
        if (measurement_results[j].count == 0) {
            break;
        }
        fprintf(stderr, "%f\n", (measurement_results[j].joules / (double)measurement_results[j].count) / measurement_results[j].time);
    }
    return 0;
}
