#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ARQ.h"

#include <string.h>

#include "CRC.h"

FrameDynamic* create_frame(uint8_t type, uint16_t seq_number, const uint8_t *data, uint16_t len, uint8_t crc_type) {
    FrameDynamic *f1 = (FrameDynamic *)malloc(sizeof(FrameDynamic) + len);
    if (f1 == NULL) {
        return NULL;
    }
    f1->CRC = 0;
    f1->seq_number = (uint16_t)seq_number;
    f1->len = (uint16_t)len;
    f1->type = 0x01;
    f1->preamble = 0x7E;

    if (data != NULL && len > 0) {
        memcpy(f1->payload, data, len);
    }

    // Liczymy CRC z calej ramki (nagłówek + payload (jesli jest (patrz DetectionDecoder.c)))
    switch(crc_type) {
        case 8:  f1->CRC = calculate_crc8((uint8_t*)f1, sizeof(FrameDynamic) + len);  break;
        case 16: f1->CRC = calculate_crc16((uint8_t*)f1, sizeof(FrameDynamic) + len); break;
        default: f1->CRC = calculate_crc32((uint8_t*)f1, sizeof(FrameDynamic) + len); break;

    }

    return f1;
}
void Stop_And_Wait() {

}
