#include "conteudo-mapa.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

struct caracter{
    unsigned int ASCII;
    unsigned long peso;
    bitmap* bmap;
};

unsigned pegar_ASCII_caracter(caracter* c){
    return c?(*c).ASCII:0;
}

unsigned long pegar_peso_caracter(caracter* c){
    return c?(*c).peso:0;
}

bitmap* pegar_bmap_caracter(caracter* c){
    return c?(*c).bmap:0;
}

caracter* criar_caracter(unsigned letra, unsigned long peso){
    caracter* c = alocar_caracter();
    *c = (caracter) {.ASCII = letra, .peso = peso, };
    return c;
}

caracter* alocar_caracter(){
    return calloc(1, sizeof(caracter));
}

caracter* desalocar_caracter(caracter* c){
    if(c){ 
        if((*c).bmap) bitmapLibera((*c).bmap);
        free(c);
    }
    return 0;
}

caracter* atualizar_ASCII_caracter(caracter* c, unsigned letra){
    if(c) (*c).ASCII = letra;
    return c;
}

caracter* atualizar_peso_caracter(caracter* c, unsigned long peso){
    if(c) (*c).peso = peso;
    return c;
}

caracter* atualizar_bmap_caracter(caracter* c, bitmap* bm){
    if(c) (*c).bmap = bm;
    return c;
}

void exibir_ASCII_caracter(caracter* c){
	if(!c){
		return ;
	}
    pegar_ASCII_caracter(c)?printf("%c", pegar_ASCII_caracter(c)):printf("\\0");
}

void exibir_peso_caracter(caracter* c){
    if(c) printf("%lu", pegar_peso_caracter(c));
}

//funcao obsoleta
void exibir_caracter(caracter* c){
    if(c) pegar_ASCII_caracter(c)?printf("%c", pegar_ASCII_caracter(c)):printf("%ld",pegar_peso_caracter(c));
}

unsigned comparar_ASCII_caracter(caracter* c, unsigned* letra){
    return c?(*c).ASCII==*letra:0;
}

unsigned* pegar_endereco_ASCII_caracter(caracter* c){
    return c?&(*c).ASCII:0;
}

unsigned comparar_peso_caracter(caracter* c1, caracter* c2){
	if(c1 && c2){
		return (*c1).peso>=(*c2).peso?1:0;
	}
	else return 0;
}
