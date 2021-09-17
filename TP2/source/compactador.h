#ifndef _compactador
#define _compactador

typedef struct mapa mapa;

mapa* ler_arquivo(char* nome);

//mapa_caracteres deve ter sido gerado pela funcao ler_arquivo (que alias poderia mudar de nome ne ja q ela gera o alfabeto)
void escrever_arquivo_binario(char* nomeArquivoTexto, char* nomeArquivoBinario, mapa* mapa_caracteres);

#endif