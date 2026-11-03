//
// Created by Marcel on 03.11.2026.
//

#ifndef ARQ_SIM_ARQ_H
#define ARQ_SIM_ARQ_H
#include <stddef.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
    uint8_t preamble;
    uint8_t type;
    uint16_t seq;
    uint16_t len;
    uint32_t CRC;
    uint8_t crc_type;
} FrameDynamic;
#pragma pack(pop)

FrameDynamic* Ramka(int frame_seq, size_t bytes_read, uint32_t CRC,uint8_t crc_type);

#endif //ARQ_SIM_ARQ_H
