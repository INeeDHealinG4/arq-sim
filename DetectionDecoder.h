//
// Created by Marcel on 17.11.2026.
//

#ifndef ARQ_SIM_DETECTIONDECODER_H
#define ARQ_SIM_DETECTIONDECODER_H
#include <stdbool.h>
#include "ARQ.h"


bool verify_frame(FrameDynamic *frame, uint8_t crc_type);

#endif //ARQ_SIM_DETECTIONDECODER_H

