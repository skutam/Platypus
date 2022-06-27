#include <stdio.h>
#include "../src/measurement.h"
#include "instruction_test.h"
#include <unistd.h>

int main(int argc, char *argv[]) {
    // When user is not sudo, print error
    if (getuid()) {
        fprintf(stderr, "Please make sure to run this program with root permission.\n");
        return 1;
    }

    if (argc != 2) {
        fprintf(stderr, "Choose as second argument one of these instructions [nop|time|clflush|fscale|mov]");
        return 1;
    }

    // Initialize library and calibrate the timing function
    init_measurement();

    // Test instructions to get the highest average consumption
    return test_instructions(argv[1]);
}
