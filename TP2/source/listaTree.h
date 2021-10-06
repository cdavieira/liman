#ifndef _listaTree
#define _listaTree

typedef struct mapa mapa;

typedef struct listaArvores listaArvores;

/*  criar_listaArvores
    Descrição: cria uma nova lista circular de arvores
    
    Input: número máximo de arvores da lista
    
    Output: ponteiro dinamicamente alocado para lista preenchida
    
    Pré condição: -
    
    Pós condicao: -
*/
listaArvores* criar_listaArvores(unsigned long max);

/*  liberar_listaArvores
    Descrição: libera lista circular de arvores
    
    Input: ponteiro para lista circular de arvores
    
    Output: ponteiro nulo
    
    Pré condição: ponteiro deve ter sido dinamicamente alocado
    
    Pós condicao: caso receba o retorno dessa função, ponteiro é setado para nulo (proteção à double-free)
*/
listaArvores* liberar_listaArvores(listaArvores* lista);

/*  mudar_inicio_listaArvores
    Descrição: muda a arvore de partida da lista circular
    
    Input: ponteiro para lista circular de arvores e novo indice da lista circular a ser tratado como começo
    
    Output: lista circular modificada
    
    Pré condição: -
    
    Pós condicao: -
*/
listaArvores* mudar_inicio_listaArvores(listaArvores* lista, long indice);

/*  pegar_numero_elementos_listaArvores
    Descrição: retorna o número atual de elementos na lista
    
    Input: ponteiro para tipo genérico de lista circular
    
    Output: número de elementos na lista circular
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned int pegar_numero_elementos_listaArvores(listaArvores* lista);

/*  pegar_mapa_listaArvores
    Descrição: retorna o mapa (arvore) de um indice da lista 
    
    Input: lista e indice do elemento desejado
    
    Output: ponteiro para elemento desejado
    
    Pré condição: argumento fornecido deve ser não nulo
    
    Pós condicao: -
*/
mapa* pegar_mapa_listaArvores(listaArvores* lista, long indice);

/*  adicionar_listaArvores
    Descrição: adiciona uma arvore em um indice da lista circular

    Detalhes:
    Caso seja fornecido um indice positivo maior do que o numero atual de elementos e menor do que o número máximo de elementos, o elemento é adicionado ao final da lista circular

    Input: lista circular a ser modificada, mapa a ser adicionado e posição na lista onde será colocado o indice
    
    Output: lista modificada com mapa adicionado
    
    Pré condição: argumentos não nulos
    
    Pós condicao: -
*/
listaArvores* adicionar_listaArvores(listaArvores* lista, mapa* map, long indice);

/*  remover_listaArvores
    Descrição: retira a arvore contida no indice i da lista
    
    Input: lista a ser modificada e indice da arvore a ser removido
    
    Output: lista atualizada
    
    Pré condição: argumentos não nulos
    
    Pós condicao: arvore removida continua alocado
*/
listaArvores* remover_listaArvores(listaArvores* lista, long indice);


/*  mostrar_listaArvores
    Descrição: imprime as arvores (mapas) da lista circular, começando da posição inicial
    
    Input: lista a ser exibida
    
    Output: 
    
    Pré condição: argumentos fornecidos devem ser não nulos
    
    Pós condicao: -
*/
void mostrar_listaArvores(listaArvores* lista);

#endif