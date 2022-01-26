#ifndef _analisar_compactado
#define _analisar_compactado

#include <stdio.h>

typedef struct mapa mapa;

typedef struct map bitmap;

/*  contar_bits_mapa
    Descrição: conta o número de bits utilizados para escrever o mapa no arquivo de entrada
    
    Input: ponteiro para arquivo de entrada onde está gravado o mapa no formato de bitmap
    
    Output: 
    
    Pré condição: argumento fornecido deve ser não nulo e bitmap deve ter sido escrito utilizando a função exportar_mapa_formato_bitmap
    
    Pós condicao: -
*/
unsigned long contar_bits_mapa(FILE* fpin);

/*  analisar_armazenamento_arquivo
    Descrição: analisa a composição em bytes do arquivo .comp gerado durante a compactação utilizados para escrita do mapa e da mensagem em bits
    
    Input: ponteiro para arquivo de entrada onde está gravado o mapa e a mensagem
    
    Output: 
    
    Pré condição: analisa apenas arquivos de conteudos gerados e gravados a partir da funcao compacta
    
    Pós condicao: -
*/
void analisar_armazenamento_arquivo(FILE* fpin);

/*  imprimir_conteudo_binario
    Descrição: imprime o mapa e a mensagem gravadas no arquivo de entrada apontado pontado por fpin
    
    Input: ponteiro para arquivo de entrada onde está gravado o mapa e a mensagem
    
    Output: 
    
    Pré condição: imprime apenas arquivos de conteudos gerados e gravados a partir da funcao compacta
    
    Pós condicao: -
*/
void imprimir_conteudo_binario(FILE* fpin);

/*  imprimir_codigos_huffmann
    Descrição: imprime na saida padrao todos os codigos gerados pelo algoritmo de huffmann
    
    Input: mapa/árvore de Huffmann
    
    Output: 
    
    Pré condição: o mapa fornecido deve ter sido processado pela função do algoritmo de huffmann ou pela função de reconstrução do mapa
    
    Pós condicao: -
*/
void imprimir_codigos_huffmann(mapa* mapa_caracteres);

/*  imprimir_conteudo_emBits
    Descrição: escreve bit a bit do conteudo do bitmap num arquivo de saída
    
    Input: nome do arquivo de saída onde será gravado o conteúdo do bitmap, bitmap preenchido
    
    Output: 
    
    Pré condição: ponteiros não nulos
    
    Pós condicao: -
*/
void imprimir_conteudo_emBits(char* saida, bitmap* bm);

#endif
