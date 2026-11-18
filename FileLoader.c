#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include "ARQ.h"

void LoadingAndWriting(size_t L_payload, const char *filename, uint8_t crc_type, double ber) {
    FILE *ReadAnyToBinary = fopen(filename, "rb");
    if (ReadAnyToBinary == NULL) {
        perror("Blad otwarcia pliku wejściowego");
        return;
    }

    fseek(ReadAnyToBinary, 0, SEEK_END);
    long file_size = ftell(ReadAnyToBinary);
    rewind(ReadAnyToBinary);

    size_t frames = (file_size + L_payload - 1) / L_payload;
    printf("Rozmiar pliku: %ld bajtow\n", file_size);
    printf("Przewidywana liczba ramek: %zu\n\n", frames);

    FILE *WriteToBinary = fopen("wyjscie.txt", "wb");
    if (WriteToBinary == NULL) {
        perror("Blad tworzenia pliku wyjsciowego");
        fclose(ReadAnyToBinary);
        return;
    }

    // Wywołanie protokołu bezpośrednio z modułu ARQ
    ArqStats stats = Stop_And_Wait(ReadAnyToBinary, WriteToBinary, L_payload, crc_type, ber);

    printf("\nPODSUMOWANIE TRANSMISJI (Stop-and-Wait)\n");
    printf("Wyslane ramki danych:       %zu\n", stats.frames_sent);
    printf("Retransmisje:               %zu\n", stats.retransmissions);
    printf("Uszkodzone ramki DATA:      %zu\n", stats.corrupted_data_frames);
    printf("Uszkodzone ramki ACK:       %zu\n", stats.corrupted_ack_frames);

    fclose(ReadAnyToBinary);
    fclose(WriteToBinary);
}