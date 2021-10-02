#ifndef _listaTree
#define _listaTree

typedef struct mapa mapa;

typedef struct listaArvores listaArvores;

/*  criar_listaArvores
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaArvores* criar_listaArvores(unsigned long max);

/*  liberar_listaArvores
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaArvores* liberar_listaArvores(listaArvores* lista);

/*  mudar_inicio_listaArvores
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaArvores* mudar_inicio_listaArvores(listaArvores* lista, long indice);

/*  pegar_numero_elementos_listaArvores
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned int pegar_numero_elementos_listaArvores(listaArvores* lista);

/*  pegar_mapa_listaArvores
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* pegar_mapa_listaArvores(listaArvores* lista, long indice);

/*  adicionar_listaArvores
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaArvores* adicionar_listaArvores(listaArvores* lista, mapa* map, long indice);

/*  remover_listaArvores
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaArvores* remover_listaArvores(listaArvores* lista, long indice);


/*  mostrar_listaArvores
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void mostrar_listaArvores(listaArvores* lista);

#endif