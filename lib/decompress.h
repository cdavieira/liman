#pragma once

#include <stdio.h>

typedef struct map bitmap;

typedef struct mapa mapa;

/*  remontar_mapa_forma_bitmap
    Descrição: reconstroi um mapa gravado em um arquivo na forma de bitmap

    Detalhes:
    Modo de leitura: quanto mais a esquerda bit mais significativo quanto mais a direita bit menos significativo
    Exemplo: 00101101 = 0*2^7 + 0*2^6 + 1*2^5 + 0*2^4 +  1*2^3 + 1*2^2 + 0*2^1 + 1*2^0
    
    Input: ponteiro para arquivo que contem o mapa escrito em bits
    
    Output: ponteiro para bitmap do mapa gravado no arquivo
    
    Pré condição: mapa gravado no arquivo binario deve ter sido gerado pela funcao exportar_mapa_formato_bitmap
    
    Pós condicao: ponteiro alocado dinamicamente (bitmap)
*/
bitmap* remontar_mapa_forma_bitmap(FILE* fpin);

/*  reconstruir_mapa_forma_original
    Descrição: converte um mapa escrito em bitmap para a forma de árvore
    
    Input: ponteiro para bitmap que contem o mapa escrito em bits
    
    Output: ponteiro para mapa do bitmap fornecido
    
    Pré condição: bitmap fornecido deve ter passado pela função remontar_mapa_forma_bitmap
    
    Pós condicao: ponteiro alocado dinamicamente para tipo mapa
*/
mapa* reconstruir_mapa_forma_original(bitmap* bm);

/*  traduzir_mensagem
    Descrição: traduz uma mensagem codificada em bits segundo a árvore de codificação fornecida para seu equivalente em ASCII, gravando-o no arquivo apontado por fpout
    
    Input: ponteiro para arquivo de entrada .comp onde está a mensagem, ponteiro para o arquivo de saída onde será gravada a mensagem traduzida e a árvore de codificação utilizada para a tradução
    
    Output: -
    
    Pré condição: leitor do ponteiro fpin deve estar posicionado no inicio da mensagem no arquivo
    
    Pós condicao: -
*/
void traduzir_mensagem(FILE* fpin, FILE* fpout, mapa* mapa_caracteres);
