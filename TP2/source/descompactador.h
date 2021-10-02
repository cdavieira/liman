#ifndef _descompactador
#define _descompactador
#include <stdio.h>

typedef struct map bitmap;

typedef struct mapa mapa;

//mapa gravado no arquivo binario deve ter sido gerado pela funcao exportar_mapa_formato_bitmap
/*  remontar_mapa_forma_bitmap
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
bitmap* remontar_mapa_forma_bitmap(FILE* fpin);

/*  reconstruir_mapa_forma_original
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* reconstruir_mapa_forma_original(bitmap* bm);

/*  traduzir_mensagem
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void traduzir_mensagem(FILE* fpin, FILE* fpout, mapa* mapa_caracteres);

#endif