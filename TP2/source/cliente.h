#ifndef _cliente
#define _cliente

//mapa_caracteres deve ter sido gerado pela funcao montar_mapa (que alias poderia mudar de nome ne ja q ela gera o alfabeto)
/*  compactar
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void compactar(char* nomeArquivoTexto, char* nomeArquivoBinario);

//essa funcao requer que o arquivo binario tenha sido gerado pela funcao compactar
/*  descompactar
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void descompactar(char* nomeArquivoBinario, char* nomeArquivoSaida);

#endif