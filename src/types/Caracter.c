#include "types/Caracter.h"
#include <malloc.h>

struct Caracter{
	unsigned int ASCII;
	unsigned long weight;
	Bitmap* bmap;
};



Caracter* caracter_new(unsigned const letra, unsigned long const weight){
	Caracter* c = malloc(sizeof(Caracter));
	*c = (Caracter) {
	  .ASCII = letra,
	  .weight = weight,
	  .bmap = NULL
	};
	return c;
}

Caracter* caracter_destroy(Caracter* c){
	if(!c){
		return NULL;
	}
	if(c->bmap){
		bitmapLibera(c->bmap);
	}
	free(c);
	return NULL;
}




unsigned caracter_get_ASCII(const Caracter * const c){
	return c ? c->ASCII : 0;
}

unsigned long caracter_get_weight(const Caracter * const c){
	return c ? c->weight : 0;
}

Bitmap* caracter_get_bmap(const Caracter * const c){
	return c ? c->bmap : 0;
}




Caracter* caracter_set_ASCII(Caracter* const c, unsigned const letra){
	if(c){
		c->ASCII = letra;
	}
	return c;
}

Caracter* caracter_set_weight(Caracter* const c, unsigned long const weight){
	if(c){
		c->weight = weight;
	}
	return c;
}

Caracter* caracter_set_bmap(Caracter* const c, Bitmap * const bm){
	if(c){
		c->bmap = bm;
	}
	return c;
}





unsigned caracter_compare_ASCII(const Caracter* const c, unsigned const* const letra){
	return c ? c->ASCII == *letra : 0;
}

unsigned caracter_compare_weight(const Caracter* const c1, const Caracter* const c2){
	if(c1 && c2){
		return c1->weight >= c2->weight;
	}
	return 0;
}
