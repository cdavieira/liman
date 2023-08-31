#include "compress.h" //algoritmo_Huffmann, gravar_codigos_mapa
#include "map.h" //encontrar_rota_node_mapa
#include "tree-list.h" //criar_listaArvores, adicionar_listaArvores, remover_listaArvores, pegar_mapa_listaArvores
#include "bitmap.h" //bitmapCatContents
#include "bitmap-plus.h"
#include "huffmann-debug.h"
#include "bits.h" //pegar_bit_char
#include <stdio.h> //rewind, feof
#include <stdlib.h>
#include <string.h>

/* define o tamanho do vetor que armazena as ocorrencias dos caracteres ASCII.
 * os indices do vetor se referem aos codigos ASCII (0 a 255)
 * o valor contido nesses indices é o número de vezes que aquele caracter
 * apareceu no arquivo a ser compactado */
#define ascii_charset 256

mapa* montar_mapa(FILE* fpin){
	if(!fpin){
		return NULL;
	}

	/* armazena a frequencia de cada letra no arquivo texto
	 * como o caracter nulo não é contabilizado no primeiro loop,
	 * é necessário inicializar o vetor de ocorrencias com 1 na posição 0
	 * (que corresponde ao caracter ASCII '\0' */
	unsigned long tabela_ocorrencias[ascii_charset] = {1, };
	mapa* map = NULL;
	listaArvores* ls = NULL;
	ssize_t index;

	for(int c=fgetc(fpin); !feof(fpin); c=fgetc(fpin)){
		tabela_ocorrencias[c]++;
	}

	ls = criar_listaArvores(ascii_charset);
	index = pegar_indice_maior_numero(tabela_ocorrencias, ascii_charset);
	while(index != -1){
		/* a adicao do maior numero é sempre feita na posicao inicial da lista
		 * circular. Isso assegura que ao final o menor elemento estará na
		 * posição inicial da lista (também é possível fazer uma função que
		 * pega o indice do menor numero e o adiciona) */
		ls = adicionar_listaArvores(ls, criar_mapa(index, tabela_ocorrencias[index], 0, 0), 0);

		//limpando a existência do caracter no buffer após adição na lista
		tabela_ocorrencias[index] = 0;

		index = pegar_indice_maior_numero(tabela_ocorrencias, ascii_charset);
	}

	// constroi um arvore inteira baseando-se numa lista circular
	// esvazia e desaloca essa no processo
	map = algoritmo_Huffman(ls);
	gravar_codigos_mapa(map);

	// para a chamada da funcao 'exportar_texto' depois
	rewind(fpin);

	return map;
}

long pegar_indice_maior_numero(unsigned long* v, const size_t tam){
	if(!v){
		return -1;
	}

	long index = -1;
	unsigned long maior = 0;
	for(unsigned long i=0; i<tam; i++){
		if(v[i] > maior){
			maior = v[i];
			index = i;
		}
	}

	return index;
}

mapa* algoritmo_Huffman(listaArvores* lc){
	if(!lc){
		return NULL;
	}
	mapa* mapa_completo = NULL;
	mapa *mapa_esq = NULL, *mapa_dir = NULL;
	unsigned long peso1, peso2;
	unsigned int size = pegar_numero_elementos_listaArvores(lc);

	//se a lista tiver mais que um elemento, entao o algoritmo de huffman não terminou
	while(size > 1){
		//pegar o primeiro elemento
		mapa_esq = pegar_mapa_listaArvores(lc, 0);

		//remove-lo da lista (será somado e depois adicionado novamente)
		lc = remover_listaArvores(lc, 0);

		//pegar o segundo elemento
		mapa_dir = pegar_mapa_listaArvores(lc, 0);

		//remove-lo da lista (será somado e depois adicionado novamente)
		lc = remover_listaArvores(lc, 0);

		//soma e readicao do resultado numa posicao da lista que preserve seu ordenamento crescente
		peso1 = pegar_peso_mapa(mapa_esq);
		peso2 = pegar_peso_mapa(mapa_dir);
		peso1 += peso2;
		lc = adicionar_ordenadamente_listaArvores(lc, criar_mapa(0, peso1, mapa_esq, mapa_dir));

		size = pegar_numero_elementos_listaArvores(lc);
	}

	//há apenas um elemento na lista circular e esse elemento corresponde à arvore completa
	mapa_completo = pegar_mapa_listaArvores(lc, 0);

	//remover a arvore da lista de arvores
	lc = remover_listaArvores(lc, 0);

	//opcional: liberar a lista passada
	lc = liberar_listaArvores(lc);

	return mapa_completo;
}

void gravar_codigos_mapa(mapa* map){
	if(!map){
		return ;
	}
	gerar_codigos_mapa_auxiliar(map, map);
}

/* a unica diferença dessa para a sua semelhante é que preserva o mapa
 * pai na chamada das funcoes (e realmente faz o trabalho acontecer) */
static void gerar_codigos_mapa_auxiliar(mapa* filho, mapa* pai){
	if(!filho || !pai){
		return ;
	}

	//se a arvore filho for um no-folha, proceder com verificacao
	if(testar_folha_mapa(filho)){
		/* se a arvore filho for no-folha e pertencer a arvore pai, entao a
		 * variavel rota é nao nula e recebe o codigo contendo o pathing correto
		 * que leva da arvore pai a arvore filho */
		char* rota = encontrar_rota_node_mapa(pai, filho);
		size_t const tam = strlen(rota);

		/* o tamanho (tam) da string rota corresponde exatamente ao numero de
		 * bits que serao utilizados para construcao do bitmap de um caracter */
		bitmap* bm = bitmapInit(tam);
		for(size_t i=0; i<tam; i++){
			/* necessário fazer a subtração para converter o conteudo da
			 * variavel rota[i] do codigo ASCII para 0 ou 1 (unsigned char) */
			bitmapAppendLeastSignificantBit(bm, rota[i] - '0');
		}

		filho = preencher_bitmap_mapa(filho, bm);
		/* é necessario liberar a memoria da string previamente alocada pois a
		 * funcao bitmapInit armazena uma string nova internamente que tem
		 * tamanho e conteudo semelhante ao fornecido */
		free(rota);
	}
	gerar_codigos_mapa_auxiliar(pegar_sae_mapa(filho), pai);
	gerar_codigos_mapa_auxiliar(pegar_sad_mapa(filho), pai);
}

/*
 * formato da exibição:
 * 0 significa nódulo raiz
 * 1 significa nódulo folha
 *
 * ordem de exibição: nodulo raiz -> nodulo esquerdo -> nodulo direito
 *
 * Após imprimir um nodulo folha (1), imprime-se 1 byte que indica o codigo na tabela ASCII do caracter armazenado naquele nodulo
 *
 * RESUMO:
 * 1. imprimir os bits do mapa
 * 2. quando chegar numa folha (1) imprimir 1 byte indicando qual letra esta nessa folha
 * 3. continuar até não restarem mais nós folhas */
void exportar_mapa_formato_bitmap(mapa* map, FILE* fpout){
	if(!map || !fpout){
		return ;
	}

	unsigned long const numero_minimo_bits_mapa = contar_nodes_mapa(map) + 8*contar_folhas_mapa(map);
	unsigned long const resto = numero_minimo_bits_mapa % 8;
	unsigned long const numero_bits_preenchimento = 8 - resto;

	/* colocando bits adicionais de preenchimento no bitmap
	 * permite evitar problemas na leitura/descompactacao do mapa
	 * os bits adicionais completarão o numero até que seja um multiplo de 8
	 * esses bits adicionais serao zeros na impressao */
	unsigned long const tam_bm = resto? numero_minimo_bits_mapa + numero_bits_preenchimento : numero_minimo_bits_mapa;
	bitmap* bm = bitmapInit(tam_bm);

	// preenchendo um bitmap com a arvore (na forma de bits corridos
	gerar_mapa_formato_bitmap(map, bm);

	// descarregando o conteudo do bitmap no arquivo apontado por fpout
	bitmapUnloadContents(bm, fpout);
	bitmapLibera(bm);
}

void gerar_mapa_formato_bitmap(mapa* map, bitmap* bm){
	if(!map || !bm){
		return ;
	}

	unsigned const ehFolha = testar_folha_mapa(map);
	bitmapAppendLeastSignificantBit(bm, ehFolha);
	if(!ehFolha){
		gerar_mapa_formato_bitmap(pegar_sae_mapa(map), bm);
		gerar_mapa_formato_bitmap(pegar_sad_mapa(map), bm);
		return ;
	}

	// se for um no folha, entao é necessario ainda escrever a letra que esta armazenada nesse no folha
	unsigned char letra = pegar_ASCII_mapa(map), bit = 0;
	for(int i=7; i>=0; i--){
		bit = pegar_bit_char(letra, i);
		bitmapAppendLeastSignificantBit(bm, bit);
	}
}

void exportar_texto_formato_bitmap(mapa* map, FILE* fpin, FILE* fpout){
	if(!map || !fpin || !fpout){
		return ;
	}

	/* o parametro da funcao 'bitmapInit' deve ser dado em **bits**
	 * o campo 'max_size' da struct bitmap corresponde ao tamanho em **bytes** do mapa
	 * +8 corresponde ao byte adicional que será adicionado ao final dessa função
	 * Esse byte adicional corresponde ao '\0' */
	unsigned long const size = calcular_tamanho_bits_mapa(map) + 8;
	bitmap *bm_texto = bitmapInit(size);
	bitmap *bm_codigo_caracter = NULL;
	unsigned int ascii = 0;

	for(int c=fgetc(fpin); !feof(fpin); c=fgetc(fpin)){
		ascii = c;
		bm_codigo_caracter = pegar_bitmap_mapa(buscar_ASCII_mapa(map, &ascii));
		bitmapCatContents(bm_texto, bm_codigo_caracter);
	}

	//adicionando o codigo do caracter nulo ao final do bitmap
	ascii = 0;
	bitmapCatContents(bm_texto, pegar_bitmap_mapa(buscar_ASCII_mapa(map, &ascii)));

	//descarregando o conteudo do bitmap no arquivo apontado por fpout
	bitmapUnloadContents(bm_texto, fpout);

	//imprimir_conteudo_emBits("bits.txt", bm_texto);
	bitmapLibera(bm_texto);
}
