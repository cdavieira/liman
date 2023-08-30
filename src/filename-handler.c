#include "filename-handler.h"
#include <string.h> //memcpy, strrchr, strcat, strlen, strdup
#include <stdlib.h> //free, calloc

char* retornar_extensao(char* nomeArquivo){
	if(!nomeArquivo){
		return NULL;
	}

	//procura a ultima ocorrencia de '.' na string fornecida
	char* token = strrchr(nomeArquivo, '.');

	if(token){
		return strdup(token);
	}

	return NULL;
}

char* remover_extensao(char* nomeArquivo){
	if(!nomeArquivo){
		return NULL;
	}

	char* extensao = retornar_extensao(nomeArquivo);

	if(!extensao){
		return NULL;
	}

	const size_t tamanho_extensao = strlen(extensao);
	const size_t tamanho_nomeArquivo = strlen(nomeArquivo) - tamanho_extensao;
	char* prefixo = calloc(tamanho_nomeArquivo+1, sizeof(char));

	memcpy(prefixo, nomeArquivo, tamanho_nomeArquivo);
	free(extensao);

	return prefixo;
}

char* adicionar_extensao(char* nomeArquivo, char* extensao){
	if(!nomeArquivo || !extensao){
		return NULL;
	}

	const size_t tamanho_nomeArquivo = strlen(nomeArquivo);
	const size_t tamanho_extensao = strlen(extensao);
	const size_t tamanho_nomeFinal = tamanho_nomeArquivo + tamanho_extensao + 1;
	char* nomeFinal = calloc(tamanho_nomeFinal, sizeof(char));

	strcat(nomeFinal, nomeArquivo);
	strcat(nomeFinal, extensao);

	return nomeFinal;
}

