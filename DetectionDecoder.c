#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ARQ.h"
#include "CRC.h"


bool verify_frame(const FrameDynamic *frame, uint8_t crc_type) {
    uint32_t calculated_crc = 0;
    switch (crc_type) {
        case 8:  calculated_crc = calculate_crc8(frame->payload, frame->len);  break;
        case 16: calculated_crc = calculate_crc16(frame->payload, frame->len); break;
        case 32:
        default: calculated_crc = calculate_crc32(frame->payload, frame->len); break;
    }

    return (calculated_crc == frame->CRC);
}

void send_back_to_transmitter(const FrameDynamic *frame, uint8_t crc_type) {

    //trzeba dodac funkcje wysylania zawartosci ramki przez kanal do nadajnika
create_frame(FRAME_TYPE_NACK, frame->seq_number, NULL, 0, crc_type);

}
