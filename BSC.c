#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "ARQ.h"
#include "CRC.h"

//przekłamywanie bitów szansą BER
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

//Sprawdzenie czy wykryto błąd
int check_error(const FrameDynamic *received_frame, uint8_t crc_type, const uint8_t *payload_buffer, size_t payload_len) {
    uint32_t new_crc = 0;

    if (crc_type == 8) {
        new_crc = calculate_crc8(payload_buffer, payload_len);
    } else if (crc_type == 16) {
        new_crc = calculate_crc16(payload_buffer, payload_len);
    } else {
        new_crc = calculate_crc32(payload_buffer, payload_len);
    }

    if (new_crc != received_frame->CRC) {
        return 1; //Wykryto błąd
    } else {
        return 0; //Nie wykryto błędu
    }
}