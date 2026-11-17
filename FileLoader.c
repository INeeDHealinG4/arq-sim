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
    uint16_t frame_seq = 0;

    while ((bytes_read = fread(buffer, 1, L_payload, ReadAnyToBinary)) > 0) {


        FrameDynamic *Frame = create_frame(FRAME_TYPE_DATA,frame_seq, buffer, (uint16_t)bytes_read, crc_type);
        if (Frame == NULL) {
            printf("Błąd alokacji ramki nr %u\n", frame_seq);
            break;
        }


        size_t total_frame_size = sizeof(FrameDynamic) + bytes_read;

        /*payload jest alokowany dynamicznie, a sizeof nie dolicza
        payloadu jesli jest alokowany dynamicznie, więc jeślibysmy nie dodali tych bitów to by nam ciągle pokazywało ze
        total_frame_size wynosi równo 11 bitów*/

        fwrite(Frame, total_frame_size, 1, WriteToBinary);

        free(Frame);
        frame_seq++;
    }

    free(buffer);
    fclose(ReadAnyToBinary);
    fclose(WriteToBinary);

}
