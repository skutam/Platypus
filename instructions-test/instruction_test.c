#include "instruction_test.h"

static double test_nop() {
    MeasurementResult all;
    for (size_t i = 0; i < RUNS; i++) {
        Measurement start = take_measurement();

        // Run instruction for N times
        for (int j = 0; j < N; ++j) {
            asm volatile("nop");
        }

        Measurement end = take_measurement();
        MeasurementResult mr = calc_results(&start, &end);
        fprintf(stderr, "%lf\n", mr.joules);
        append_results(&all, &mr);
    }

    // Get average consumption
    return all.joules / all.time / (double)RUNS;
}

static double test_rdtsc() {
    MeasurementResult all;
    for (size_t i = 0; i < RUNS; i++) {
        volatile uint64_t a, d;
        Measurement start = take_measurement();

        // Run instruction for N times
        for (int j = 0; j < N; ++j) {
            asm volatile("rdtsc": "=a"(a), "=d"(d));
        }

        Measurement end = take_measurement();
        MeasurementResult mr = calc_results(&start, &end);
        fprintf(stderr, "%lf\n", mr.joules);
        append_results(&all, &mr);
    }

    // Get average consumption
    return all.joules / all.time / (double)RUNS;
}

static double test_clflush() {
    MeasurementResult all;
    for (size_t i = 0; i < RUNS; i++) {
        volatile uint64_t r = 25;
        Measurement start = take_measurement();

        // Run instruction for N times
        for (int j = 0; j < N; ++j) {
            asm volatile("clflush (%0)":: "r"(&r));
        }

        Measurement end = take_measurement();
        MeasurementResult mr = calc_results(&start, &end);
        fprintf(stderr, "%lf\n", mr.joules);
        append_results(&all, &mr);
    }

    // Get average consumption
    return all.joules / all.time / (double)RUNS;
}

static double test_fscale() {
    MeasurementResult all;
    for (size_t i = 0; i < RUNS; i++) {
        Measurement start = take_measurement();

        // Run instruction for N times
        for (int j = 0; j < N; ++j) {
            asm volatile("fscale");
        }

        Measurement end = take_measurement();
        MeasurementResult mr = calc_results(&start, &end);
        fprintf(stderr, "%lf\n", mr.joules);
        append_results(&all, &mr);
    }

    // Get average consumption
    return all.joules / all.time / (double)RUNS;
}

static double test_mov() {
    MeasurementResult all;
    for (size_t i = 0; i < RUNS; i++) {
        volatile uint64_t src = 159;
        volatile uint64_t dst;
        Measurement start = take_measurement();

        // Run instruction for N times
        for (int j = 0; j < N; ++j) {
            asm volatile("mov %1, %0": "=r"(dst): "r"(src));
        }

        Measurement end = take_measurement();
        MeasurementResult mr = calc_results(&start, &end);
        fprintf(stderr, "%lf\n", mr.joules);
        append_results(&all, &mr);
    }

    // Get average consumption
    return all.joules / all.time / (double)RUNS;
}

/**
 * Based on given inst text, test instruction
 * @param inst Instruction to test
 * @return 1 on error, 0 on success
 */
int test_instructions(const char* inst) {
    // Based on argument choose which instruction to test
    if (strcmp(inst, "nop") == 0) {
        printf("NOP:\t\t%lf\n", test_nop());
    } else if (strcmp(inst, "rdtsc") == 0) {
        printf("RDTSC:\t\t%lf\n", test_rdtsc());
    } else if (strcmp(inst, "clflush") == 0) {
        printf("CLFLUSH:\t%lf\n", test_clflush());
    } else if (strcmp(inst, "fscale") == 0) {
        printf("FSCALE:\t\t%lf\n", test_fscale());
    } else if (strcmp(inst, "mov") == 0) {
        printf("MOV:\t\t%lf\n", test_mov());
    } else {
        fprintf(stderr, "Choose as second argument one of these instructions [nop|time|clflush|fscale|mov]");
        return 1;
    }

    return 0;
}
