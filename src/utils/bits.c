#include "utils/bits.h"

unsigned char bits_bitAt(char c, unsigned char index){
	return (c >> (index & 7)) & 1;
}
