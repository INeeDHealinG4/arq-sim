#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ARQ.h"

FrameDynamic* Ramka(int frame_seq, size_t bytes_read, uint32_t CRC,uint8_t crc_type, const uint8_t *payload) {
    FrameDynamic *f1 = (FrameDynamic *)malloc(sizeof(FrameDynamic) + bytes_read);

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

    //Kopiowanie payload za nagłówek
    if (payload != NULL && bytes_read > 0) {
        uint8_t *frame_payload = (uint8_t *)f1 + sizeof(FrameDynamic);
        for (size_t i = 0; i < bytes_read; i++) {
            frame_payload[i] = payload[i];
        }
    }

    return f1;
}
