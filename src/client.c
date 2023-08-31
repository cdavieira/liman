#include "client.h"
#include "compress.h"
#include "decompress.h"
#include "huffmann-debug.h"
#include "filename-handler.h"
#include "map.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void compactar(char* nomeArquivoEntrada){
	if(!nomeArquivoEntrada){
		return ;
	}

	char *nomeArquivoSaida;
	FILE *fpin;
	FILE *fpout;
	mapa* mapa_caracteres;

	fpin = fopen(nomeArquivoEntrada, "rb");
	assert(("Erro ao abrir arquivo para leitura!", fpin));

	nomeArquivoSaida = adicionar_extensao(nomeArquivoEntrada, ".comp");
	fpout = fopen(nomeArquivoSaida, "wb");
	assert(("Erro ao gerar arquivo compactado!", fpout));

	//aplicando huffmann e criando mapa de caracteres
	mapa_caracteres = montar_mapa(fpin);

	//escrevendo mapa de caracteres no começo do arquivo
	exportar_mapa_formato_bitmap(mapa_caracteres, fpout);

	//iniciando gravacao do conteudo do arquivoTexto no arquivo binario
	exportar_texto_formato_bitmap(mapa_caracteres, fpin, fpout);

	liberar_mapa(mapa_caracteres);
	free(nomeArquivoSaida);
	fclose(fpin);
	fclose(fpout);
}

/* Funcionamento da descompactação:
 * 1. ler um bit
 * 2. verificar se esse bit leva até algum no folha da arvore/mapa
 * 3. se sim, então chegou-se a uma letra. Caso contrário, continua leitura */
void descompactar(char* nomeArquivoBinario){
	if(!nomeArquivoBinario){
		return ;
	}

	char *nomeArquivoSaida;
	FILE *fpin;
	FILE *fpout;
	bitmap *bm;
	mapa *map;

	fpin = fopen(nomeArquivoBinario, "rb");
	assert(("Erro ao abrir arquivo compactado! Terminando programa...", fpin));

	nomeArquivoSaida = remover_extensao(nomeArquivoBinario);
	fpout = fopen(nomeArquivoSaida, "wb");
	assert(("Erro ao abrir arquivo para escrita! Terminando programa...", fpin));

	//analisar_armazenamento_arquivo(fpin);

	//imprimir_conteudo_binario(fpin);

	bm = remontar_mapa_forma_bitmap(fpin);

	/* 'remontar_mapa_forma_bitmap' deixa propositalmente o leitor do arquivo fpin posicionada para a leitura do primeiro caracter da descompactação
	 * A lógica a seguir parte desse pressuposto de funcionamento*/
	map = reconstruir_mapa_forma_original(bm);

	//imprimir_codigos_huffmann(map);
	traduzir_mensagem(fpin, fpout, map);

	// 'bitmapLibera' não é null segura, portante é necessário a condicional
	free(nomeArquivoSaida);
	if(bm){
		bitmapLibera(bm);
	}
	liberar_mapa(map);
	fclose(fpin);
	fclose(fpout);
}
