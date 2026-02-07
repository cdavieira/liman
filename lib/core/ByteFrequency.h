#pragma once

#include "utils/types/common.h"

typedef struct ByteFrequency ByteFrequency;
typedef struct InputBytes InputBytes;

ByteFrequency *byteFreq_from_filename(const char *filename);
ByteFrequency *byteFreq_destroy(ByteFrequency *f);

size_t byteFreq_get_total_size_in_bytes(ByteFrequency *bf);

unsigned byteFreq_get_byte(ByteFrequency *bf, unsigned ch);
size_t byteFreq_get_count(ByteFrequency *bf, unsigned ch);
