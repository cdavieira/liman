#pragma once

#include "utils/types/common.h"

unsigned char bits_get_char_bit(char c, unsigned char index);
unsigned char bits_get_size_bit(size_t n, int index);

size_t bits_fromBytes(size_t nBytes);
size_t bits_toBytes(size_t nbits);

size_t bits_padding(size_t nbits);

size_t bits_get_minimum_amount_of_required_bytes(size_t nbits);
