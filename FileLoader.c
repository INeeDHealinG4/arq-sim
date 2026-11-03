#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARQ.h"
#include "CRC.h"

void LoadingAndWriting(size_t L_payload, const char *filename, uint8_t crc_type) {
    FILE* ReadAnyToBinary;
    FILE* WriteToBinary;
    ReadAnyToBinary = fopen(filename, "rb");
    if (ReadAnyToBinary == NULL) {
        printf("Error opening file\n");
    }
    WriteToBinary = fopen("filename.txt", "wb");
    if (WriteToBinary == NULL) {
        printf("File does not exist... creating one\n");
    }
    unsigned char *buffer = malloc(L_payload);
    if (buffer == NULL) {
        printf("Błąd alokacji pamięci");
        fclose(ReadAnyToBinary);
        return;
    }
    size_t bytes_read = 0;
    int frame_seq = 0;
    while ((bytes_read = fread(buffer, 1, L_payload, ReadAnyToBinary)) > 0) {
        uint32_t current_CRC = 0;

        // Wybór odpowiedniej funkcji CRC bazując na typie
        if (crc_type == 8) {
            current_CRC = calculate_crc8(buffer, bytes_read);
        } else if (crc_type == 16) {
            current_CRC = calculate_crc16(buffer, bytes_read);
        } else {
            current_CRC = calculate_crc32(buffer, bytes_read);
        }

        // Tworzenie ramki (zdefiniowanej w ARQ.h)
        FrameDynamic *Frame = Ramka(frame_seq, bytes_read, current_CRC, crc_type);

        fwrite(Frame, sizeof(FrameDynamic), 1, WriteToBinary);

        free(Frame);
        frame_seq++;
    }

    free(buffer);
    fclose(ReadAnyToBinary);
    fclose(WriteToBinary);

}
