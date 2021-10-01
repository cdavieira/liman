#ifndef _cliente
#define _cliente

//mapa_caracteres deve ter sido gerado pela funcao montar_mapa (que alias poderia mudar de nome ne ja q ela gera o alfabeto)
void compactar(char* nomeArquivoTexto, char* nomeArquivoBinario);

//essa funcao requer que o arquivo binario tenha sido gerado pela funcao compactar
void descompactar(char* nomeArquivoBinario, char* nomeArquivoSaida);

#endif