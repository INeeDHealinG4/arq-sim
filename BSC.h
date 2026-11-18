#ifndef BSC_H
#define BSC_H

#include <stddef.h>
#include <stdint.h>
#include "ARQ.h"

void bsc(void *Frame, size_t frame_size, double ber);
int check_error(const FrameDynamic *received_frame, uint8_t crc_type, size_t total_size);


#endif
