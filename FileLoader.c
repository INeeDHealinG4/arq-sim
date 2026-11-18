#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "ARQ.h"
#include "CRC.h"
#include "BSC.h"

void LoadingAndWriting(size_t L_payload, const char *filename, uint8_t crc_type, double ber){
    FILE* ReadAnyToBinary;
    FILE* WriteToBinary;
    ReadAnyToBinary = fopen(filename, "rb");
    if (ReadAnyToBinary == NULL) {
        printf("Error opening file\n");
    }
    else {
        printf("Plik zostal otwarty\n");
    }

    //rozmiar pliku i liczba bramek
    fseek(ReadAnyToBinary, 0, SEEK_END);
    long file_size = ftell(ReadAnyToBinary);
    rewind(ReadAnyToBinary);

    size_t frames = (file_size + L_payload - 1) / L_payload;

    printf("Rozmiar pliku: %ld bajtow\n", file_size);
    printf("Przewidywana liczba ramek: %zu\n\n", frames);

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
        FrameDynamic *Frame = Ramka(frame_seq, bytes_read, current_CRC, crc_type, buffer);

       //Kanał BSC
       size_t frame_size = sizeof(FrameDynamic) + bytes_read;
       bsc(Frame, frame_size, ber);
       uint8_t *bsc_payload = (uint8_t *)Frame + sizeof(FrameDynamic);

       //Sprawdzanie błędów
       int error = check_error(Frame, crc_type, bsc_payload, bytes_read);

       if (error) {
           printf("Ramka %d: Wykryto blad? TAK (NACK)\n", frame_seq);
       } else {
           printf("Ramka %d: Wykryto blad? NIE (ACK)\n", frame_seq);
       }

       fwrite(Frame, frame_size, 1, WriteToBinary);

       free(Frame);
       frame_seq++;
    }

    free(buffer);
    fclose(ReadAnyToBinary);
    fclose(WriteToBinary);

}
