#include "../src/measurement.h"
#include "covert_channel.h"

#include <stdio.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    // When user is not sudo, print error
    if (getuid()) {
        fprintf(stderr, "Please make sure to run this program with root permission.\n");
        return 1;
    }

    if (argc != 3) {
        fprintf(stderr, "Choose as second argument one of these instructions [nop|time|clflush|fscale|mov]\n");
        fprintf(stderr, "and as third argument the number of bits per second, max is 20.\n");
        return 1;
    }

    // Initialize
    init_measurement();

    uint64_t bps = (argv[2], NULL, 10);

    if (bps < 1 || bps > 20) {
        fprintf(stderr, "Third argument must be in range <1,20>!\n");
        return 1;
    }

    return start_covert_channel(argv[1], bps);
}