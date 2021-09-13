#ifndef _listaCircular
#define _listaCircular

typedef struct listaCircular listaCircular;

typedef struct celulaCircular celulaCircular;

listaCircular* criar_listaCircular(unsigned long max);

listaCircular* liberar_listaCircular(listaCircular* lista, void* (*liberar_conteudo)());

void* pegar_listaCircular(listaCircular* lista, unsigned long indice);

listaCircular* adicionar_listaCircular(listaCircular* lista, void* item, unsigned long indice);

listaCircular* remover_listaCircular(listaCircular* lista, unsigned long indice, void* (*liberar_conteudo)());

void mostrar_listaCircular(listaCircular* lista, void (*mostrar_conteudo)());

#endif