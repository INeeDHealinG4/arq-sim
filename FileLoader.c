#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARQ.h"

void LoadingAndWriting(size_t L_payload, const char *filename, uint32_t CRC) {
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
    size_t bytes_read;
    int frame_seq = 0;
    uint8_t crc_type = 0;
    while ((bytes_read = fread(buffer, 1, L_payload, ReadAnyToBinary)) > 0) {

        FrameDynamic *Frame = Ramka(frame_seq, bytes_read, CRC, crc_type);

        free(Frame);
        frame_seq++;
    }

    free(buffer);
    fclose(ReadAnyToBinary);
    fclose(WriteToBinary);
    free(ReadAnyToBinary);
    free(WriteToBinary);
}
