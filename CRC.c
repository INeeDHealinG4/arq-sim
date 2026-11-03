#include "CRC.h"

#define CRC32_POLYNOMIAL 0xEDB88320UL //rozne wielomiany
#define CRC16_POLYNOMIAL 0x8408
#define CRC8_POLYNOMIAL  0x8C

// Osobne tablice dla każdego rodzaju CRC
static uint32_t crc32_table[256];
static uint16_t crc16_table[256];
static uint8_t  crc8_table[256];

static int crc32_init = 0;
static int crc16_init = 0;
static int crc8_init = 0;

void init_crc32_table(void) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
            else crc >>= 1;
        }
        crc32_table[i] = crc;
    }
    crc32_init = 1;
}

uint32_t calculate_crc32(const uint8_t *data, size_t length) {
    if (crc32_init==0) {
        init_crc32_table();
    }
    uint32_t crc = 0xFFFFFFFFUL;
    for (size_t i = 0; i < length; i++) {
        uint8_t index = (crc ^ data[i]) & 0xFF;
        crc = (crc >> 8) ^ crc32_table[index];
    }
    return ~crc;
}

void init_crc16_table(void) {
    for (uint16_t i = 0; i < 256; i++) {
        uint16_t crc = i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) crc = (crc >> 1) ^ CRC16_POLYNOMIAL;
            else crc >>= 1;
        }
        crc16_table[i] = crc;
    }
    crc16_init = 1;
}

uint16_t calculate_crc16(const uint8_t *data, size_t length) {
    if (crc16_init==0) {
        init_crc16_table();
    }
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < length; i++) {
        uint8_t index = (crc ^ data[i]) & 0xFF;
        crc = (crc >> 8) ^ crc16_table[index];
    }
    return crc;
}

void init_crc8_table(void) {
    for (int i = 0; i < 256; i++) {
        uint8_t crc = (uint8_t)i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) crc = (crc >> 1) ^ CRC8_POLYNOMIAL;
            else crc >>= 1;
        }
        crc8_table[i] = crc;
    }
    crc8_init = 1;
}

uint8_t calculate_crc8(const uint8_t *data, size_t length) {
    if (crc8_init==0) {
        init_crc8_table();
    }
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < length; i++) {
        crc = crc8_table[crc ^ data[i]];
    }
    return crc;
}