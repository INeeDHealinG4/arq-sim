#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ARQ.h"

FrameDynamic* Ramka(int frame_seq, size_t bytes_read, uint32_t CRC,uint8_t crc_type) {
    FrameDynamic *f1 = (FrameDynamic *)malloc(sizeof(FrameDynamic));

    if (f1 == NULL) {
        return NULL;
    }

    switch(crc_type) {
        case 8:
            f1->CRC = CRC & 0xFF;        // Zachowuje tylko 1 bajt
            break;
        case 16:
            f1->CRC = CRC & 0xFFFF;      // Zachowuje 2 bajty
            break;
        case 32:
        default:
            f1->CRC = CRC;               // Zachowuje całe 4 bajty
            break;
    }
    f1->seq = (uint16_t)frame_seq;
    f1->len = (uint16_t)bytes_read;
    f1->type = 0x01;
    f1->preamble = 0x7E;
    return f1;
}
