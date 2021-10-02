#ifndef _listaCircular
#define _listaCircular

typedef struct listaCircular listaCircular;

typedef struct celulaCircular celulaCircular;

/*  criar_listaCircular
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaCircular* criar_listaCircular(unsigned long max);

/*  liberar_listaCircular
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaCircular* liberar_listaCircular(listaCircular* lista, void* (*liberar_conteudo)());

/*  mudar_elemento_inicial_listaCircular
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaCircular* mudar_elemento_inicial_listaCircular(listaCircular* lista, long indice);

/*  pegar_numero_elementos_listaCircular
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned int pegar_numero_elementos_listaCircular(listaCircular* lista);

/*  pegar_listaCircular
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void* pegar_listaCircular(listaCircular* lista, long indice);

/*  
    Caso o indice seja positivo, funciona normalmente
    caso o indice seja negativo: mesma coisa que se estivesse adicionando no positivo, porem a orientação muda
    sabemos que para o caso de indice positivo: i_elemento = numero_elemento-1
    sabemos ainda que i_elemento + | i_elemento_orientacao_trocada | = n_total
    | i_elemento_orientacao_trocada | = (n_total - numero_elemento) + 1 = n_elemento_orientacao_trocada + 1
*/
/*  adicionar_listaCircular
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaCircular* adicionar_listaCircular(listaCircular* lista, void* item, long indice);

//apenas remove a CELULA da lista fornecida, não desaloca o conteudo contida nessa celula
/*  remover_listaCircular
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
listaCircular* remover_listaCircular(listaCircular* lista, long indice);

/*  mostrar_listaCircular
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void mostrar_listaCircular(listaCircular* lista, void (*mostrar_conteudo)());

#endif