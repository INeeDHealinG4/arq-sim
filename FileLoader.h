#ifndef FILE_PROCESSOR_H
#define FILE_PROCESSOR_H

#include <stddef.h>
#include <stdint.h>
#include "ARQ.h"

void LoadingAndWriting(size_t L_payload, const char *filename, uint8_t crc_type);

#endif