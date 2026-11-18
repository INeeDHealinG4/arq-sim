#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "DetectionDecoder.h"
#include "CRC.h"

bool verify_frame(const FrameDynamic *frame, uint8_t crc_type, size_t total_size) {
    if (frame == NULL || total_size < sizeof(FrameDynamic)) {
        return false;
    }

    // Zabezpieczenie przed przekłamaniem pola len w kanale
    if (sizeof(FrameDynamic) + frame->len != total_size) {
        return false;
    }

    FrameDynamic *temp_frame = (FrameDynamic *)malloc(total_size);
    if (temp_frame == NULL) {
        return false;
    }

    memcpy(temp_frame, frame, total_size);
    uint32_t received_crc = temp_frame->CRC;
    temp_frame->CRC = 0;

    uint32_t calculated_crc = 0;
    switch (crc_type) {
        case CRC_TYPE_8:
            calculated_crc = calculate_crc8((uint8_t *)temp_frame, total_size);
            break;
        case CRC_TYPE_16:
            calculated_crc = calculate_crc16((uint8_t *)temp_frame, total_size);
            break;
        case CRC_TYPE_32:
        default:
            calculated_crc = calculate_crc32((uint8_t *)temp_frame, total_size);
            break;
    }

    free(temp_frame);
    return (calculated_crc == received_crc);
}

FrameDynamic* receiver_process(const FrameDynamic *incoming, size_t total_size, uint16_t *expected_seq, FILE *out_file, uint8_t crc_type) {
    // 1. Sprawdzenie poprawności CRC całej ramki
    if (!verify_frame(incoming, crc_type, total_size)) {
        printf("  [ODBIORNIK] Blad CRC! Odsyłam NACK dla seq=%u\n", *expected_seq);
        return create_frame(FRAME_TYPE_NACK, *expected_seq, NULL, 0, crc_type);
    }

    // 2. Oczekiwana, poprawna ramka z danymi
    if (incoming->type == FRAME_TYPE_DATA && incoming->seq_number == *expected_seq) {
        if (out_file != NULL && incoming->len > 0) {
            fwrite(incoming->payload, 1, incoming->len, out_file);
        }
        printf("  [ODBIORNIK] Odebrano poprawnie seq=%u (%u B). Odsylam ACK.\n", incoming->seq_number, incoming->len);
        (*expected_seq)++;
        return create_frame(FRAME_TYPE_ACK, incoming->seq_number, NULL, 0, crc_type);
    }

    // 3. Duplikat ramki (gdy poprzedni ACK uległ przekłamaniu w kanale powrotnym)
    if (incoming->type == FRAME_TYPE_DATA && incoming->seq_number < *expected_seq) {
        printf("  [ODBIORNIK] Duplikat seq=%u (poprzedni ACK uszkodzony). Ponawiam ACK.\n", incoming->seq_number);
        return create_frame(FRAME_TYPE_ACK, incoming->seq_number, NULL, 0, crc_type);
    }

    return NULL;
}