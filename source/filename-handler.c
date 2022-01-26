#include "filename-handler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char* retornar_extensao(char* nomeArquivo){
	char* extensao = 0;
	if(nomeArquivo){
		char* token = strrchr(nomeArquivo, '.'); //procura ultima ocorrencia de . na string fornecida
		if(token) extensao = strdup(token);
	}
	return extensao;
}

char* remover_extensao(char* nomeArquivo){
	char* prefixo = 0;
	if(nomeArquivo){
		char* extensao = retornar_extensao(nomeArquivo);
		size_t tamanho_extensao = 0, tamanho_nomeArquivo = strlen(nomeArquivo);
		if(extensao){
			tamanho_extensao = strlen(extensao);
			tamanho_nomeArquivo -= tamanho_extensao;
			prefixo = calloc(tamanho_nomeArquivo+1, sizeof(char));
			memcpy(prefixo, nomeArquivo, tamanho_nomeArquivo);
			prefixo[tamanho_nomeArquivo] = '\0';
		}
	}
	return prefixo?prefixo:strdup(nomeArquivo);
}

char* adicionar_extensao(char* nomeArquivo, char* extensao){
	char* nomeFinal = 0;
	if(nomeArquivo && extensao){
		size_t tamanho_nomeArquivo = strlen(nomeArquivo), tamanho_extensao = strlen(extensao), tamanho_nomeFinal = tamanho_nomeArquivo+tamanho_extensao;
		nomeFinal = calloc(tamanho_nomeFinal+1, sizeof(char));
		strcat(nomeFinal, nomeArquivo);
		strcat(nomeFinal, extensao);
		nomeFinal[tamanho_nomeFinal] = '\0';
	}
	return nomeFinal;
}

