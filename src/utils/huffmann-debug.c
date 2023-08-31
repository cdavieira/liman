#include <stdlib.h>
#include <stdio.h>
#include "utils/bits.h"
#include "utils/huffmann-debug.h"
#include "data-structures/map.h"
#include "data-structures/bitmap-plus.h"

unsigned long contar_bits_mapa(FILE* fpin){
	if(!fpin){
		return 0;
	}
	rewind(fpin);

	unsigned long size = 0;
	unsigned char c = fgetc(fpin);
	//bit: 0 represents a non-leaf node, 1 represents a leaf node.
	//leaf nodes are followed by 1 byte, which is a char
	unsigned char bit = pegar_bit_char(c, 7);

	rewind(fpin);
	if(bit){
		return 0;
	}

	unsigned long soma = 1, bits_to_read = 0;
	for(c=fgetc(fpin); c!=EOF; c=fgetc(fpin)){
		for(short i=7; i>=0; i--){
			bit = pegar_bit_char(c, i);

			if(bits_to_read){
				bits_to_read--;
			}
			else{
				bits_to_read = bit? 8:0;
				soma += bit?-1:1;
			}

			size++;

			if(!soma){
				break;
			}
		}
		if(!soma){
			break;
		}
	}
	/* a letra que segue o ultimo no folha do mapa acaba nao sendo lida pelo
	 * algoritmo, por isso é necessário adicionar 8 bits manualmente */
	size += 8; 
	rewind(fpin);

	return size;
}

void analisar_armazenamento_arquivo(FILE* fpin){
	if(!fpin){
		return ;
	}

	unsigned long tamanho_bits_mapa = contar_bits_mapa(fpin), tamanho_bits_mensagem = 0, total=0;
	unsigned bits_vazios = tamanho_bits_mapa % 8;

	for(char c=fgetc(fpin);!feof(fpin);c=fgetc(fpin)){
		tamanho_bits_mensagem+=8;
	}

	tamanho_bits_mensagem -= tamanho_bits_mapa;
	tamanho_bits_mensagem -= bits_vazios;
	total = tamanho_bits_mapa + tamanho_bits_mensagem + bits_vazios;

	printf("TAMANHO TOTAL DO ARQUIVO: %lu BYTES\n", total/8);
	printf("PORCENTAGEM OCUPADA PELO MAPA: %.2f%%\n", 100*((double)tamanho_bits_mapa)/total);
	printf("PORCENTAGEM OCUPADA PELA MENSAGEM: %.2f%%\n", 100*((double)tamanho_bits_mensagem)/total);
	//na verdade tem alguns bit vazios que não foram contabilizados (os que aparecem depois da escrita da mensagem)
	printf("PORCENTAGEM OCUPADA PELOS BITS VAZIOS: %.2f%%\n", 100*((double)bits_vazios)/total);

	rewind(fpin);
}

void imprimir_conteudo_binario(FILE* fpin){
	if(!fpin){
		return ;
	}
	unsigned long tamanho_bits_mapa = contar_bits_mapa(fpin), tamanho_bytes_mapa = tamanho_bits_mapa/8;

	rewind(fpin);

	puts("Mapa:");
	for(unsigned long i=0;i<tamanho_bytes_mapa;i++){
		printf("%c", fgetc(fpin));
	}

	puts("\nMensagem:");
	for(char c=fgetc(fpin);!feof(fpin);c=fgetc(fpin)){
		printf("%c", c);
	}
	putchar('\n');

	rewind(fpin);
}

void imprimir_codigos_huffmann(mapa* mapa_caracteres){
	if(!mapa_caracteres){
		return ;
	}
	char* codigo_huffmann = NULL;
	for(unsigned letra = 0; letra<256; letra++){
		codigo_huffmann = encontrar_rota_node_mapa(mapa_caracteres, buscar_ASCII_mapa(mapa_caracteres, &letra));
		printf("%u(%c): %s\n", letra, letra, codigo_huffmann? codigo_huffmann : "nao codificado");
		free(codigo_huffmann);
	}
}

void imprimir_conteudo_emBits(char* saida, bitmap* bm){
	if(!saida || !bm){
		return ;
	}
	FILE* fpout = fopen(saida, "w");
	bitmapUnloadContentsBinary(bm, fpout);
	fclose(fpout);
}
