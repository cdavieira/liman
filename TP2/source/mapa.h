#ifndef _mapa
#define _mapa

typedef struct map bitmap;

typedef struct mapa mapa;

typedef struct caracter caracter;

/*  pegar_ASCII_mapa
    Descrição: 
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned int pegar_ASCII_mapa(mapa* node);

/*  pegar_peso_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long pegar_peso_mapa(mapa* node);

/*  pegar_bitmap_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
bitmap* pegar_bitmap_mapa(mapa* node);

/*  pegar_sae_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* pegar_sae_mapa(mapa* map);

/*  pegar_sad_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* pegar_sad_mapa(mapa* map);

/*  criar_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* criar_mapa(unsigned int ASCII, unsigned long peso, mapa* sae, mapa* sad);

/*  liberar_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* liberar_mapa(mapa* map);

/*  adicionar_rota
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* adicionar_rota(mapa* map, mapa* filho, int codigo);

/*  remover_rota
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* remover_rota(mapa* map, int codigo);

/*  imprimir_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
void imprimir_mapa(mapa* map);

/*  testar_folha_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned testar_folha_mapa(mapa* map);

/*  contar_nodes_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long contar_nodes_mapa(mapa* map);

/*  contar_folhas_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long contar_folhas_mapa(mapa* map);

/*  calcular_altura_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long calcular_altura_mapa(mapa* map);

/*  calcular_altura_node_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long calcular_altura_node_mapa(mapa* arvore, mapa* node, unsigned long altura_inicial);

/*  testar_existencia_node_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned testar_existencia_node_mapa(mapa* arvore, mapa* node);

/*  encontrar_rota_node_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
char* encontrar_rota_node_mapa(mapa* arvore, mapa* node);

/*  percorrer_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* percorrer_mapa(mapa* map, char* rota);

/*  preencher_ASCII_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* preencher_ASCII_mapa(mapa* map, unsigned ASCII);

/*  preencher_peso_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* preencher_peso_mapa(mapa* map, unsigned peso);

/*  preencher_bitmap_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* preencher_bitmap_mapa(mapa* map, bitmap* bm);

/*  buscar_ASCII_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* buscar_ASCII_mapa(mapa* map, unsigned* ASCII);

/*  pegar_endereco_ASCII_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned* pegar_endereco_ASCII_mapa(mapa* map);

//precisa ter passado pelo algoritmo de huffman
/*  calcular_tamanho_bits_mapa
    Descrição:
    
    Input:
    
    Output: 
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long calcular_tamanho_bits_mapa(mapa* map);

#endif