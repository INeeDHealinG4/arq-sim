#ifndef ARQ_SIM_DETECTIONDECODER_H
#define ARQ_SIM_DETECTIONDECODER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include "ARQ.h"

bool verify_frame(const FrameDynamic *frame, uint8_t crc_type, size_t total_size);
FrameDynamic* receiver_process(const FrameDynamic *incoming, size_t total_size, uint16_t *expected_seq, FILE *out_file, uint8_t crc_type);

#endif // ARQ_SIM_DETECTIONDECODER_H