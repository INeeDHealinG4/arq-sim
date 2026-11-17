#ifndef ARQ_SIM_ARQ_H
#define ARQ_SIM_ARQ_H

#include <stddef.h>
#include <stdint.h>

#define FRAME_PREAMBLE 0xAA

typedef enum {
    FRAME_TYPE_DATA = 0x01,
    FRAME_TYPE_ACK  = 0x02,
    FRAME_TYPE_NACK = 0x03
} FrameType;

typedef enum {
    CRC_TYPE_8  = 1,
    CRC_TYPE_16 = 2,
    CRC_TYPE_32 = 4
} CrcType;

#pragma pack(push, 1)
typedef struct {
    uint8_t  preamble;   // preambula
    uint8_t  type;       // DATA / ACK / NACK
    uint16_t len;        // Długość pola payload w bajtach
    uint16_t seq_number; // Numer ramki//
    uint32_t CRC;        // Obliczona suma kontrolna (dla CRC8/16 starsze bajty to 0)
    uint8_t  payload[];  // Dynamiczne dane o rozmiarze 'len'
} FrameDynamic;
#pragma pack(pop)

// Tworzy ramkę w pamięci, kopiuje dane i od razu oblicza odpowiednie CRC
FrameDynamic* create_frame(uint8_t type, uint16_t seq_number, const uint8_t *data, uint16_t len, uint8_t crc_type);


#endif //ARQ_SIM_ARQ_H