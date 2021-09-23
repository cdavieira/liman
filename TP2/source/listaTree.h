#ifndef _listaTree
#define _listaTree

typedef struct mapa mapa;

typedef struct listaArvores listaArvores;

listaArvores* criar_listaArvores(unsigned long max);

listaArvores* liberar_listaArvores(listaArvores* lista);

listaArvores* mudar_inicio_listaArvores(listaArvores* lista, long indice);

unsigned int pegar_numero_elementos_listaArvores(listaArvores* lista);

mapa* pegar_mapa_listaArvores(listaArvores* lista, long indice);

listaArvores* adicionar_listaArvores(listaArvores* lista, mapa* map, long indice);

listaArvores* remover_listaArvores(listaArvores* lista, long indice);

void mostrar_listaArvores(listaArvores* lista);

#endif