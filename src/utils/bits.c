#include "utils/bits.h"

unsigned char pegar_bit_char(char c, unsigned char index){
	//configurando index para estar no intervalo [0, 7]
	index &= 7;
	while(index){
		c >>= 1;
		index--;
	}
	return c&1;
}
