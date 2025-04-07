#include "core/Caracter.h"
#include "container/Bitmap.h"
#include <stdio.h>
#include <malloc.h>

struct Caracter{
	unsigned int ASCII;
	unsigned long peso;
	bitmap* bmap;
};



Caracter* caracter_new(unsigned const letra, unsigned long const peso){
	Caracter* c = calloc(1, sizeof(Caracter));
	*c = (Caracter) {.ASCII = letra, .peso = peso, };
	return c;
}

Caracter* caracter_destroy(Caracter* c){
	if(!c){
		return NULL;
	}
	if((*c).bmap){
		bitmapLibera((*c).bmap);
	}
	free(c);
	return NULL;
}




unsigned caracter_get_ASCII(Caracter const * const c){
	return c?(*c).ASCII:0;
}

unsigned long caracter_get_weight(Caracter const * const c){
	return c?(*c).peso:0;
}

bitmap* caracter_get_bmap(Caracter const * const c){
	return c?(*c).bmap:0;
}


unsigned* caracter_get_ASCII_addr(Caracter *c){
	return c? &(*c).ASCII : NULL;
}




Caracter* caracter_set_ASCII(Caracter* const c, unsigned const letra){
	if(c){
		(*c).ASCII = letra;
	}
	return c;
}

Caracter* caracter_set_weight(Caracter* const c, unsigned long const peso){
	if(c){
		(*c).peso = peso;
	}
	return c;
}

Caracter* caracter_set_bmap(Caracter* const c, bitmap * const bm){
	if(c){
		(*c).bmap = bm;
	}
	return c;
}





unsigned caracter_compare_ASCII(Caracter const* const c, unsigned const* const letra){
	return c? (*c).ASCII == *letra : 0;
}

unsigned caracter_compare_weight(Caracter const* const c1, Caracter const* const c2){
	if(c1 && c2){
		return (*c1).peso >= (*c2).peso;
	}
	return 0;
}
