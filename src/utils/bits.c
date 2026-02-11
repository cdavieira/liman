#include "utils/bits.h"

unsigned char bits_get_char_bit(char c, unsigned char index) {
  return (c >> (index & 7)) & 1;
}

unsigned char bits_get_size_bit(size_t n, int index) {
  return (n >> (index & 63)) & 1;
}

size_t bits_fromBytes(size_t nBytes) { return nBytes << 3; }
size_t bits_toBytes(size_t nbits) { return nbits >> 3; }

size_t bits_padding(size_t nbits) {
  size_t rem = nbits % 8;
  return rem == 0 ? 0 : 8 - rem;
}

size_t bits_get_minimum_amount_of_required_bytes(size_t nbits) {
  return (nbits >> 3) + (nbits % 8 == 0 ? 0 : 1);
}
