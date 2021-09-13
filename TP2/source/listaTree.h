#ifndef _listaTree
#define _listaTree

typedef struct mapa mapa;

typedef struct listaArvores listaArvores;

listaArvores* criar_listaArvores(unsigned long max);

listaArvores* liberar_listaArvores(listaArvores* lista);

mapa* pegar_listaArvores(listaArvores* lista, unsigned long indice);

listaArvores* adicionar_listaArvores(listaArvores* lista, mapa* map, unsigned long indice);

listaArvores* remover_listaArvores(listaArvores* lista, unsigned long indice);

void mostrar_listaArvores(listaArvores* lista);

#endif