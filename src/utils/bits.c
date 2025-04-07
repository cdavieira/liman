#include "utils/bits.h"

unsigned char bits_bitAt(char c, unsigned char index){
	//index e [0, 7]
	index &= 7;
	return (c >> index) & 1;
}
