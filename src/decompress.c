#include "decompress.h"
#include "data-structures/bitmap.h" //bitmapInit, bitmapAppendLeastSignificantBit
#include "data-structures/map.h" //criar_mapa
#include "utils/macros.h" //left, right
#include "utils/huffmann-debug.h" //contar_bits_mapa
#include "utils/bits.h" //pegar_bit_char
#include <stdlib.h> //fgetc, rewind

static long ftell_ultimo_char(FILE *fp);

static mapa* reconstruir_mapa_forma_original_aux(bitmap* bm, unsigned *index, unsigned modo);

static mapa* reconstruir_mapa_forma_original_aux_ler_no_folha(bitmap* bm, unsigned *index);

/*
 * A leitura considera o bit mais a esquerda como o mais significativo e o mais a direita como menos significativo
 * exemplo:
 *	um char gravado com a sequência de bits '00101101' corresponderá ao decimal 45, pois:
 *	0*2^7 + 0*2^6 + 1*2^5 + 0*2^4 +  1*2^3 + 1*2^2 + 0*2^1 + 1*2^0
 */
bitmap* remontar_mapa_forma_bitmap(FILE* fpin){
	if(!fpin){
		return NULL;
	}

	/*o numero inicial contido no arquivo binario ; note que o numero que é
	 * gravado inicialmente no arquivo compactado não contabiliza em sua contagem os
	 * bits de preenchimento adicionais que foram colocados no bitmap escrito
	 * (rever funcao exportar_mapa_formato_bitmap) */
	unsigned long const bm_size = contar_bits_mapa(fpin);
	if(!bm_size){
		return NULL;
	}

	bitmap* bm = bitmapInit(bm_size);
	char c = 0;
	unsigned long bits_lidos = 0;
	while(bits_lidos < bm_size){
		/* a leitura utilizando fgetc garante que um número de bits multiplo de
		* 8 será lido ; de fato, o número de bits escrito no arquivo para o
		* mapa foi pensado para ser um múltiplo de oito */
		c = fgetc(fpin);

		/*  esse for só será possível, porque o tamanho do bitmap gerado pela
		 *  funcao exportar_mapa_formato_bitmap é sempre um multiplo de 8.
		 *  Logo, esse loop não põe o programa em risco de SEGFAULT em nenhum
		 *  caso e nem adianta a leitura de uma parte do arquivo relacionada a
		 *  mensagem codificada */
		for(int j=7; j>=0; j--){
			bitmapAppendLeastSignificantBit(bm, pegar_bit_char(c, j));
			bits_lidos++;
			if(bits_lidos == bm_size){
				break;
			}
		}
	}
	return bm;
}

mapa* reconstruir_mapa_forma_original(bitmap* bm){
	unsigned index = 0;
	return reconstruir_mapa_forma_original_aux(bm, &index, left);
}

/* O que faz: inicia a rotina de reconstrução do
 * mapa na forma de bitmap para a forma original */
static mapa* reconstruir_mapa_forma_original_aux(bitmap* bm, unsigned *index, unsigned modo){
	if(!bm){
		return NULL;
	}

	mapa* map = NULL;
	unsigned char bit;

	if(*index<bitmapGetLength(bm)){
		/* pega o bit da posicao index e DEPOIS da chamada da funcao incrementa
		 * em 1 o indice */

		bit = bitmapGetBit(bm, (*index)++); 

		if(bit){
			/* se o bit for um significa que estamos lendo um nó folha e
			 * chamamos a rotina de leitura de nós folhas */

			map = reconstruir_mapa_forma_original_aux_ler_no_folha(bm, index);  
		}
		else{   
			/* se o bit for zero significa que estamos lendo um nó não folha,
			 * então criamos um mapa virgem e adicionamos seus nós filhos
			 * chamando recursivamente a função de construção da árvore */

			map = criar_mapa(0, 0, 0, 0);
			map = adicionar_rota(map, reconstruir_mapa_forma_original_aux(bm, index, left), left);
			map = adicionar_rota(map, reconstruir_mapa_forma_original_aux(bm, index, right), right);
		}
	}

	return map;
}

/* Reconstroi nós folhas da forma de bits para a forma original */
static mapa* reconstruir_mapa_forma_original_aux_ler_no_folha(bitmap* bm, unsigned *index){
	if(!bm){
		return NULL;
	}

	mapa* map = NULL;
	unsigned char ascii = 0;
	unsigned char bit;
	for(unsigned i=0;i<8;i++){		  
		// lendo bit por bit contido no bitmap e incrementando a posição do leitor (index)
		bit = bitmapGetBit(bm, (*index)++); 
		/* a leitura vai da esquerda para direita, portanto segue a ordem do
		 * bit mais significativo para o menos significativo. Por conta disso,
		 * os bits obtidos pela funcao 'bitmapGetBit' devem ser deslocados,
		 * a fim de que sejam posicionados na casa que pertencem */
		bit<<=(7-i);					
		// reconstruindo o char ascii original (bit a bit)
		ascii |= bit;
	}
	map = criar_mapa(ascii, 0, 0, 0);	  
	return map;
}

/* assume-se que o ponteiro no arquivo fpin está posicionado logo no inicio da
 * mensagem codificada */
void traduzir_mensagem(FILE* fpin, FILE* fpout, mapa* mapa_caracteres){
	if(!fpin || !fpout || !mapa_caracteres){
		return ;
	}

	/* altura: tamanho maximo que um codigo de huffmann pode ter */
	unsigned long altura = calcular_altura_mapa(mapa_caracteres); 
	unsigned char *codigo = calloc(altura+1, sizeof(unsigned char));
	unsigned char u=0, bit=0;
	unsigned bits_disponiveis_codigo = altura;
	unsigned stopcode = 0;
	mapa* corrente = NULL;
	long ultimo_char = ftell_ultimo_char(fpin);
	long char_atual = 1;

	/* foi necessário mudar a condição do for para a funcao 'feof' em vez de
	 * c!=EOF, pois é possível que o arquivo binario apresente EOF como
	 * caracter utilizado para carregar a mensagem */
	for(int c = fgetc(fpin); !feof(fpin) ;c = fgetc(fpin), char_atual++){ 
		u = c;						  
		for(int i=7; i>=0;i--){	  
			if(stopcode){
				break;
			}
			bit = pegar_bit_char(u, i);

			/* caso não existam mais bits disponíveis no cache de tamanho igual
			 * ao numero máximo de caracteres que podem ser usados para se
			 * chegar a um nó folha */

			if(!bits_disponiveis_codigo){ 
				/* libera-se o código previamente construído para se adicionar
				 * um novo bit */
				free(codigo);		   

				/* alocando novamente o codigo para que possa ser reconstruído 
				 * +1: incluindo o '/0' final */
				codigo = calloc(altura+1, sizeof(unsigned char)); 

				/* atualizando o numero de bits disponiveis no buffer para o
				 * máximo novamente */
				bits_disponiveis_codigo = altura;   
			}

			/* atribuindo de fato o valor do bit no indice especifico em que
			 * deve ser adicionado */
			*(codigo+altura-bits_disponiveis_codigo) = bit?'1':'0'; 

			/* percorrendo árvore de mapas */
			corrente = percorrer_mapa(mapa_caracteres, codigo);	 

			/* caso o mapa obtido no deslocamento anterior seja um nó folha,
			 * então significa que o código que construímos nos levou a um
			 * caracter codificado válido da árvore de codificação */
			if(testar_folha_mapa(corrente)){						
				if(!pegar_ASCII_mapa(corrente) && char_atual==ultimo_char){
					/* caso o mapa obtido seja um \0 e o char_atual
					 * corresponder ao ultimo char presente no arquivo, isso
					 * significa que chegamos ao fim da mensagem e que devemos
					 * parar de executar a rotina do loop para nao imprimir
					 * bits lixo (0's) no arquivo descompactado */
					stopcode=1;		 
				}
				if(stopcode){
					break;
				}
				/* caso o caracter seja diferente de \0 grava-se ele no arquivo de saida */
				fprintf(fpout, "%c", pegar_ASCII_mapa(corrente));   

				/* isso forçara a refazer o codigo na proxima iteracao */
				bits_disponiveis_codigo=1; 
			}

			/* atualizando o numero de bits disponíveis */
		   --bits_disponiveis_codigo; 
		}
	}
	free(codigo); 
}

static long ftell_ultimo_char(FILE *fp){
	if(!fp){
		return -1;
	}
	long pos = -1;

	// grava a posicao inicial da stream
	long init_pos = ftell(fp), final_pos = 0;	

	// vai para a posicao final da stream
	fseek(fp, 0, SEEK_END);				

	// grava a posicao final da stream
	final_pos = ftell(fp);

	// computa a diferença entre a posicao inicial e final
	pos = final_pos - init_pos;			

	// retorna para a posicao inicial da stream
	fseek(fp, init_pos, SEEK_SET);			

	// retorna a diferença entre o indice atual até que se chegue ao indice final do arquivo
	return pos;						
}
