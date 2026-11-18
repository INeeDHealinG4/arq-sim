#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ARQ.h"
#include "CRC.h"

//przekłamywanie bitów z szansą BER
void bsc(void *Frame, size_t frame_size, double ber) {
    uint8_t *bsc_frame = (uint8_t *)Frame;
    for (size_t i = 0; i < frame_size; i++) {
        for (int bit = 0; bit < 8; bit++) {
            double u = (double)rand() / (double)RAND_MAX;
            if (u < ber) {
                bsc_frame[i] ^= (1 << bit);
            }
        }
    }
}