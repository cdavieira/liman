#ifndef _listaCircular
#define _listaCircular

typedef struct listaCircular listaCircular;

typedef struct celulaCircular celulaCircular;

/*  criar_listaCircular
    Descrição: cria uma nova lista circular genérica
    
    Input: número máximo de elementos da lista
    
    Output: ponteiro dinamicamente alocado para lista preenchida
    
    Pré condição: -
    
    Pós condicao: -
*/
listaCircular* criar_listaCircular(unsigned long max);

/*  liberar_listaCircular
    Descrição: libera lista circular
    
    Input: ponteiro para tipo genérico de lista circular e funcao que libera o conteúdo dessa
    
    Output: ponteiro nulo
    
    Pré condição: ponteiro deve ter sido dinamicamente alocado
    
    Pós condicao: caso receba o retorno dessa função, ponteiro é setado para nulo (proteção à double-free)
*/
listaCircular* liberar_listaCircular(listaCircular* lista, void* (*liberar_conteudo)());

/*  mudar_elemento_inicial_listaCircular
    Descrição: muda o elemento de partida da lista circular
    
    Input: ponteiro para tipo genérico de lista circular e novo indice da lista circular a ser tratado como começo
    
    Output: lista circular modificada
    
    Pré condição: -
    
    Pós condicao: -
*/
listaCircular* mudar_elemento_inicial_listaCircular(listaCircular* lista, long indice);

/*  pegar_numero_elementos_listaCircular
    Descrição: retorna o número atual de elementos na lista
    
    Input: ponteiro para tipo genérico de lista circular
    
    Output: número de elementos na lista circular
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned int pegar_numero_elementos_listaCircular(listaCircular* lista);

/*  pegar_listaCircular
    Descrição: retorna o elemento de um indice da lista 
    
    Input: lista e indice do elemento desejado
    
    Output: ponteiro para elemento desejado
    
    Pré condição: argumento fornecido deve ser não nulo
    
    Pós condicao: -
*/
void* pegar_listaCircular(listaCircular* lista, long indice);

/*  adicionar_listaCircular
    Descrição: adiciona um elemento em um indice da lista circular

    Detalhes:
    Caso seja fornecido um indice positivo maior do que o numero atual de elementos e menor do que o número máximo de elementos, o elemento é adicionado ao final da lista circular

    Input: lista circular a ser modificada, item a ser adicionado e posição na lista onde será colocado o indice
    
    Output: lista modificada com item adicionado
    
    Pré condição: argumentos não nulos
    
    Pós condicao: -
*/
listaCircular* adicionar_listaCircular(listaCircular* lista, void* item, long indice);

/*  remover_listaCircular
    Descrição: retira o item contido no indice i da lista
    
    Input: lista a ser modificada e indice do elemento a ser removido
    
    Output: lista atualizada
    
    Pré condição: argumentos não nulos
    
    Pós condicao: item removido continua alocado
*/
listaCircular* remover_listaCircular(listaCircular* lista, long indice);

/*  mostrar_listaCircular
    Descrição: imprime os elementos da lista circular, começando da posição inicial
    
    Input: lista a ser exibida e função que exibe o conteúdo da célula da lista
    
    Output: 
    
    Pré condição: argumentos fornecidos devem ser não nulos
    
    Pós condicao: -
*/
void mostrar_listaCircular(listaCircular* lista, void (*mostrar_conteudo)());

#endif