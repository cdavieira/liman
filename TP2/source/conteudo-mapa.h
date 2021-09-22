#ifndef _conteudo_mapa
#define _conteudo_mapa

typedef struct caracter caracter;

typedef struct map bitmap;

unsigned pegar_ASCII_caracter(caracter* c);

unsigned long pegar_peso_caracter(caracter* c);

bitmap* pegar_bmap_caracter(caracter* c);

caracter* criar_caracter(unsigned letra, unsigned long peso);

caracter* alocar_caracter();

caracter* desalocar_caracter(caracter* c);

caracter* atualizar_ASCII_caracter(caracter* c, unsigned letra);

caracter* atualizar_peso_caracter(caracter* c, unsigned long peso);

caracter* atualizar_bmap_caracter(caracter* c, bitmap* bm);

void exibir_ASCII_caracter(caracter* c);

void exibir_peso_caracter(caracter* c);

void exibir_caracter(caracter* c);

unsigned comparar_ASCII_caracter(caracter* c, unsigned* letra);

unsigned* pegar_endereco_ASCII_caracter(caracter* c);

#endif