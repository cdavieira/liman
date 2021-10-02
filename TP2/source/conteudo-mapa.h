#ifndef _conteudo_mapa
#define _conteudo_mapa

typedef struct caracter caracter;

typedef struct map bitmap;

/*  pegar_ASCII_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned pegar_ASCII_caracter(caracter* c);

/*  pegar_peso_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long pegar_peso_caracter(caracter* c);

/*  pegar_bmap_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
bitmap* pegar_bmap_caracter(caracter* c);

/*  criar_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
caracter* criar_caracter(unsigned letra, unsigned long peso);

/*  alocar_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
caracter* alocar_caracter();

/*  desalocar_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
caracter* desalocar_caracter(caracter* c);

/*  atualizar_ASCII_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
caracter* atualizar_ASCII_caracter(caracter* c, unsigned letra);

/*  atualizar_peso_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
caracter* atualizar_peso_caracter(caracter* c, unsigned long peso);

/*  atualizar_bmap_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
caracter* atualizar_bmap_caracter(caracter* c, bitmap* bm);

/*  exibir_ASCII_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void exibir_ASCII_caracter(caracter* c);

/*  exibir_peso_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void exibir_peso_caracter(caracter* c);

/*  exibir_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void exibir_caracter(caracter* c);

/*  comparar_ASCII_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned comparar_ASCII_caracter(caracter* c, unsigned* letra);

/*  pegar_endereco_ASCII_caracter
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned* pegar_endereco_ASCII_caracter(caracter* c);

#endif