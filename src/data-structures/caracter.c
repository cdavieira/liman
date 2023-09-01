#include "data-structures/caracter.h"
#include "data-structures/bitmap.h" //bitmapLibera
#include <stdio.h> //printf
#include <stdlib.h> //calloc, free

struct caracter{
	unsigned int ASCII;
	unsigned long peso;
	bitmap* bmap;
};

unsigned pegar_ASCII_caracter(caracter const * const c){
	return c?(*c).ASCII:0;
}

unsigned long pegar_peso_caracter(caracter const * const c){
	return c?(*c).peso:0;
}

bitmap* pegar_bmap_caracter(caracter const * const c){
	return c?(*c).bmap:0;
}

caracter* criar_caracter(unsigned const letra, unsigned long const peso){
	caracter* c = alocar_caracter();
	*c = (caracter) {.ASCII = letra, .peso = peso, };
	return c;
}

caracter* alocar_caracter(){
	return calloc(1, sizeof(caracter));
}

caracter* desalocar_caracter(caracter* c){
	if(!c){
		return NULL;
	}
	if((*c).bmap){
		bitmapLibera((*c).bmap);
	}
	free(c);
	return NULL;
}

caracter* atualizar_ASCII_caracter(caracter* const c, unsigned const letra){
	if(c){
		(*c).ASCII = letra;
	}
	return c;
}

caracter* atualizar_peso_caracter(caracter* const c, unsigned long const peso){
	if(c){
		(*c).peso = peso;
	}
	return c;
}

caracter* atualizar_bmap_caracter(caracter* const c, bitmap * const bm){
	if(c){
		(*c).bmap = bm;
	}
	return c;
}

void exibir_ASCII_caracter(caracter const* const c){
	if(!c){
		return ;
	}
	if(pegar_ASCII_caracter(c)){
		printf("%c", pegar_ASCII_caracter(c));
	}
	else{
		printf("\\0");
	}
}

void exibir_peso_caracter(caracter const* const c){
	if(!c){
		return ;
	}
	printf("%lu", pegar_peso_caracter(c));
}


unsigned comparar_ASCII_caracter(caracter const* const c, unsigned const* const letra){
	return c? (*c).ASCII==*letra : 0;
}

unsigned* pegar_endereco_ASCII_caracter(caracter *c){
	return c?&(*c).ASCII:NULL;
}

unsigned comparar_peso_caracter(caracter const* const c1, caracter const* const c2){
	if(c1 && c2){
		return (*c1).peso>=(*c2).peso?1:0;
	}
	return 0;
}
