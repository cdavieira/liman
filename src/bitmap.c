/** Define um TAD representando um mapa de bits.
 * @file bitmap.c
 * @author Joao Paulo Andrade Almeida (jpalmeida@inf.ufes.br)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bitmap.h"

struct map {
    unsigned int max_size;        ///< tamanho maximo em bits
    unsigned int length;         ///< tamanho atual em bits
    unsigned char* contents;     ///< conteudo do mapa de bits
};

/**
 * Funcao auxiliar que imprime uma mensagem de falha e aborta o programa caso testresult seja falso.
 * @param testresult Valor booleano representando o resultado do teste que deveria ser verdadeiro.
 * @param message A mensagem para ser impressa se resultado do teste for falso.
 */
void assert(int testresult, char* message) {
	if (!testresult) {
		printf("%s\n", message);
		exit(EXIT_FAILURE);
	}
}


/**
 * Retorna o conteudo do mapa de bits.
 * @param bm O mapa de bits.
 */
unsigned char* bitmapGetContents(bitmap* bm) {
	return bm->contents;
}

/**
 * Retorna o tamanho maximo do mapa de bits.
 * @param bm O mapa de bits.
 * @return O tamanho maximo do mapa de bits.
 */
unsigned int bitmapGetMaxSize(bitmap* bm) {
	return bm->max_size;
}

/**
 * Retorna o tamanho atual do mapa de bits.
 * @param bm O mapa de bits.
 * @return O tamanho atual do mapa de bits.
 */
unsigned int bitmapGetLength(bitmap* bm) {
	return bm->length;
}

/**
 * Constroi um novo mapa de bits, definindo um tamanho maximo.
 * @param max_size O tamanho maximo para o mapa de bits.
 * @return O mapa de bits inicializado.
 */
bitmap* bitmapInit(unsigned int max_size) {
	bitmap* bm;
    bm = (bitmap*)malloc(sizeof(bitmap));
	// definir tamanho maximo em bytes, com arredondamento para cima
	unsigned int max_sizeInBytes=(max_size+7)/8;
	// alocar espaco de memoria para o tamanho maximo em bytes
	bm->contents=calloc(max_sizeInBytes, sizeof(char));
	// verificar alocacao de memoria
	assert(bm->contents!=NULL, "Erro de alocacao de memoria.");
	// definir valores iniciais para tamanho maximo e tamanho atual
	bm->max_size=max_size;
	bm->length=0;
	return bm;
}

/**
 * Retorna o valor do bit na posicao index.
 * @param bm O mapa de bits.
 * @param index A posicao do bit.
 * @pre index<bitmapGetLength(bm)
 * @return O valor do bit.
 */
unsigned char bitmapGetBit(bitmap* bm, unsigned int index) // index in bits
{
	// verificar se index<bm.length, pois caso contrario, index e' invalido
	assert(index<bm->length, "Acesso a posicao inexistente no mapa de bits.");
	// index/8 e' o indice do byte que contem o bit em questao
	// 7-(index%8) e' o deslocamento do bit em questao no byte
	return (bm->contents[index/8] >> (7-(index%8))) & 0x01;
}

/**
 * Modifica o valor do bit na posicao index.
 * @param bm O mapa de bits.
 * @param index A posicao do bit.
 * @param bit O novo valor do bit.
 * @post bitmapGetBit(bm,index)==bit
 */
static void bitmapSetBit(bitmap* bm, unsigned int index, unsigned char bit) {
    // verificar se index<bm->length, pois caso contrario, index e' invalido
    assert(index<bm->length, "Acesso a posicao inexistente no mapa de bits.");
    // index/8 e' o indice do byte que contem o bit em questao
    // 7-(index%8) e' o deslocamento do bit em questao no byte
    bit=bit & 0x01;
    bit=bit<<(7-(index%8));
    bm->contents[index/8]= bm->contents[index/8] | bit;
}


/**
 * Adiciona um bit no final do mapa de bits.
 * @param bm O mapa de bits.
 * @param bit O novo valor do bit.
 * @pre bitmapGetLength(bm) < bitmapGetMaxSize(bm)
 * @post (bitmapGetBit(bm,bitmapGetLength(bm) @ pre)==bit) 
 * and (bitmapGetLength(bm) == bitmapGetLength(bm) @ pre+1)
 */
void bitmapAppendLeastSignificantBit(bitmap* bm, unsigned char bit) {
	// verificar se bm->length<bm->max_size, caso contrario, o bitmap esta' cheio
	assert(bm->length<bm->max_size, "Tamanho maximo excedido no mapa de bits.");
	// como um bit sera' adicionado, devemos incrementar o tamanho do mapa de bits
	bm->length++;
	bitmapSetBit(bm, bm->length-1, bit);
}

/**
 * Libera a memória dinâmica alocada para o mapa de bits.
 * @param bm O mapa de bits.
 */
void bitmapLibera (bitmap* bm){
    
    free (bm->contents);
    free (bm);
}


