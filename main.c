#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

void LoadingAndWriting(size_t L_payload, const char *filename, uint8_t crc_type, double ber);

int main(void) {
    srand((unsigned int)time(NULL));

    printf("Start\n");

    //Parametry
    size_t payload_size = 16; //Rozmiar paczki danych w bajtach
    const char *input_file = "dane.bin"; //Nazwa pliku wejściowego
    uint8_t crc_type = 16; //Wybór typu CRC
    double ber = 0.001; //szans na błąd w kanale BSC

    LoadingAndWriting(payload_size, input_file, crc_type, ber);

    return 0;
}

// $bytes = New-Object byte[] (500 * 1024); (New-Object Random).NextBytes($bytes); [IO.File]::WriteAllBytes("dane.bin", $bytes)

//ta komenda w terminalu generuje nowy plik o nazwie dane.bin (zeby zadzialalo, to trzeba wrzucic do folderu "cmake-build-debug")