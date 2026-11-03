#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stddef.h>


uint32_t calculate_crc32(const uint8_t *data, size_t length);
uint16_t calculate_crc16(const uint8_t *data, size_t length);
uint8_t  calculate_crc8(const uint8_t *data, size_t length);

#endif