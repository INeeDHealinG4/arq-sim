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

//Sprawdzenie czy wykryto błąd
int check_error(const FrameDynamic *received_frame, uint8_t crc_type, size_t total_size) {
    FrameDynamic *temp_frame = malloc(total_size);
    if (temp_frame == NULL) {
        return 1; //W razie błędu alokacji - ramka uszkodzoną
    }
    memcpy(temp_frame, received_frame, total_size);

    uint32_t received_crc = temp_frame->CRC;
    temp_frame->CRC = 0; //Wyzerowanie  CRC

    uint32_t new_crc = 0;

    //CRC z całej ramki (nagłówek + payload)
    switch(crc_type) {
        case 8: new_crc = calculate_crc8((uint8_t*)temp_frame, total_size);
            break;
        case 16: new_crc = calculate_crc16((uint8_t*)temp_frame, total_size);
            break;
        default: new_crc = calculate_crc32((uint8_t*)temp_frame, total_size);
            break;
    }

    free(temp_frame);

    if (new_crc != received_frame->CRC) {
        return 1; //Wykryto błąd
    } else {
        return 0; //Nie wykryto błędu
    }
}