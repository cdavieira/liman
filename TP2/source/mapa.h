#ifndef _mapa
#define _mapa

typedef struct map bitmap;

typedef struct mapa mapa;

typedef struct caracter caracter;

/*  pegar_ASCII_mapa
    Descrição: retorna o codigo ASCII do caracter contido no mapa 
    
    Input: mapa
    
    Output: código ASCII do caracter armazenado no mapa
    
    Pré condição: mapa deve ter sido alocado utilizando a função de criar_mapa
    
    Pós condicao: -
*/
unsigned int pegar_ASCII_mapa(mapa* node);

/*  pegar_peso_mapa
    Descrição: retorna o peso/a frequência do caracter contido no mapa 
    
    Input: mapa
    
    Output: peso/frequência do caracter armazenado no mapa
    
    Pré condição: mapa deve ter sido alocado utilizando a função de criar_mapa
    
    Pós condicao: -
*/
unsigned long pegar_peso_mapa(mapa* node);

/*  pegar_bitmap_mapa
    Descrição: retorna o bitmap do caracter contido no mapa 
    
    Input: mapa
    
    Output: ponteiro para bitmap do caracter armazenado no mapa
    
    Pré condição: mapa deve ter passado pela função preencher_bitmap_mapa
    
    Pós condicao: -
*/
bitmap* pegar_bitmap_mapa(mapa* node);

/*  pegar_sae_mapa
    Descrição: retorna o mapa filho esquerdo do mapa fornecido
    
    Input: mapa
    
    Output: ponteiro para mapa filho esquerdo do mapa fornecido
    
    Pré condição: mapa deve ter sido alocado utilizando a função de criar_mapa
    
    Pós condicao: -
*/
mapa* pegar_sae_mapa(mapa* map);

/*  pegar_sad_mapa
    Descrição: retorna o mapa filho direito do mapa fornecido
    
    Input: mapa
    
    Output: ponteiro mapa filho direito do mapa fornecido
    
    Pré condição: mapa deve ter sido alocado utilizando a função de criar_mapa
    
    Pós condicao: -
*/
mapa* pegar_sad_mapa(mapa* map);

/*  criar_mapa
    Descrição: cria um novo mapa (tipo de árvore) e preenche seus campos com os argumentos fornecidos
    
    Input: codigo ASCII do char em questao, peso/frequência desse no texto, mapa filho esquerdo e mapa filho direito
    
    Output: ponteiro dinamicamente alocado para mapa preenchido
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* criar_mapa(unsigned int ASCII, unsigned long peso, mapa* sae, mapa* sad);

/*  liberar_mapa
    Descrição: libera um ponteiro para mapa
    
    Input: ponteiro para mapa
    
    Output: nulo
    
    Pré condição: ponteiro deve ter sido dinamicamente alocado
    
    Pós condicao: caso receba o retorno dessa função, ponteiro é setado para nulo (proteção a double-free)
*/
mapa* liberar_mapa(mapa* map);

/*  adicionar_rota
    Descrição: adiciona um mapa filho a um mapa raiz/mãe
    
    Input: mapa a ter nó adicionado (mãe), mapa a ser adicionado (filho) e código que orienta para qual lado deve ser feita a atribuição (direito ou esquerdo)
    
    Output: mapa raiz com mapa filho relativo ao codigo fornecido modificado 
    
    Pré condição: codigo deve estar de acordo com os macros LEFT e RIGHT
    
    Pós condicao: -
*/
mapa* adicionar_rota(mapa* map, mapa* filho, int codigo);

/*  remover_rota
    Descrição: remove um mapa filho de um mapa raiz/mãe
    
    Input: mapa a ter nó removido (mãe) e código que orienta qual nó filho deve ser removido (direito ou esquerdo)
    
    Output: mapa raiz com mapa filho removido, relativo ao codigo fornecido 
    
    Pré condição: ponteiro para mapa deve ser não-nulo, codigo deve estar de acordo com os macros LEFT e RIGHT
    
    Pós condicao: -
*/
mapa* remover_rota(mapa* map, int codigo);

/*  imprimir_mapa
    Descrição: imprime estrutura de mapa
    
    Input: mapa a ser impresso
    
    Output: mapa impresso na saida padrao

    Observações: nós não-folha e folha são impressos de modos diferentes
    
    Pré condição: argumentos devem ser não-nulos 
    
    Pós condicao: -
*/
void imprimir_mapa(mapa* map);

/*  testar_folha_mapa
    Descrição: testa se o mapa fornecido pode ser um nó folha ou não
    
    Input: mapa a ser analisado
    
    Output: 1 caso o mapa apresente mapas filhos nulos, 0 caso contrário
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned testar_folha_mapa(mapa* map);

/*  contar_nodes_mapa
    Descrição: conta o número de mapas presentes na estrutura do mapa raiz, incluindo ele mesmo na contagem
    
    Input: mapa que contem estrutura a ter mapas contabilizados
    
    Output: número de mapas presentes na estrutura
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long contar_nodes_mapa(mapa* map);

/*  contar_folhas_mapa
    Descrição: conta o número de mapas folha presentes na estrutura do mapa raiz
    
    Input: mapa que contem estrutura a ter mapas folha contabilizados
    
    Output: número de mapas folha presentes na estrutura
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long contar_folhas_mapa(mapa* map);

/*  calcular_altura_mapa
    Descrição: calcula a altura do mapa fornecido
    
    Input: mapa a ter altura calculada
    
    Output: altura do mapa
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long calcular_altura_mapa(mapa* map);

/*  calcular_altura_node_mapa
    Descrição: calcula a altura do mapa fornecido (se existente) no mapa raiz passado 
    
    Input: mapa raiz, mapa de interesse e altura do mapa raiz (0, caso o mapa raiz seja o começo da estrutura)
    
    Output: altura do mapa na estrutura do mapa raiz
    
    Pré condição: argumentos devem ser não-nulos
    
    Pós condicao: -
*/
unsigned long calcular_altura_node_mapa(mapa* arvore, mapa* node, unsigned long altura_inicial);

/*  testar_existencia_node_mapa
    Descrição: testa se o mapa fornecido está presente na estrutura do mapa raiz passado
    
    Input: mapa raiz que contém estrutura de mapas e mapa de interesse
    
    Output: 1 caso o mapa pesquisado exista no mapa raiz, 0 caso contrário
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned testar_existencia_node_mapa(mapa* arvore, mapa* node);

/*  encontrar_rota_node_mapa
    Descrição: constroi o trajeto necessário para se chegar a um determinado mapa de uma arvore
    
    Input: mapa raiz e mapa pesquisado
    
    Output: caso encontre o mapa pesquisado no mapa raiz, ponteiro para char dinamicamente alocado contendo o trajeto até o nó especificado (utilizando os codigos convencionados de orientação), nulo caso contrário
    
    Pré condição: -
    
    Pós condicao: -
*/
char* encontrar_rota_node_mapa(mapa* arvore, mapa* node);

/*  percorrer_mapa
    Descrição: percorre a estrutura de um mapa utilizando as instruções contidas em rota
    
    Input: mapa a ter estrutura percorrida e string contendo orientação de ida, utilizando o codigo convencionado (0s e 1s)
    
    Output: caso suceda retorna o mapa encontrado após seguir as instruções contidas na rota, caso contrário retorna nulo
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* percorrer_mapa(mapa* map, char* rota);

/*  preencher_ASCII_mapa
    Descrição: atualiza o codigo ASCII do caracter armazenado em um mapa
    
    Input: mapa a ser modificado e novo codigo ASCII
    
    Output: mapa fornecido modificado
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* preencher_ASCII_mapa(mapa* map, unsigned ASCII);

/*  preencher_peso_mapa
    Descrição: atualiza o peso do caracter armazenado em um mapa
    
    Input: mapa a ser modificado e novo peso
    
    Output: mapa fornecido modificado
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* preencher_peso_mapa(mapa* map, unsigned peso);

/*  preencher_bitmap_mapa
    Descrição: atualiza o bitmap do caracter armazenado em um mapa
    
    Input: mapa a ser modificado e novo bitmap
    
    Output: mapa fornecido modificado com novo bitmap
    
    Pré condição: -
    
    Pós condicao: -
*/
mapa* preencher_bitmap_mapa(mapa* map, bitmap* bm);

/*  buscar_ASCII_mapa
    Descrição: busca por um caracter ASCII em toda estrutura do mapa raiz fornecido
    
    Input: mapa onde sera realizada a busca (mãe/raiz) e ponteiro para codigo do caracter ASCII buscado
    
    Output: caso haja êxito, retorna-se o primeiro mapa contido na estrutura do mapa raiz que contém o item pesquisado, caso contrário retorna nulo
    
    Pré condição: argumentos fornecidos não nulos
    
    Pós condicao: -
*/
mapa* buscar_ASCII_mapa(mapa* map, unsigned* ASCII);

/*  pegar_endereco_ASCII_mapa
    Descrição: retorna o endereço da váriavel que armazena o codigo ASCII de um caracter contido num mapa
    
    Input: mapa
    
    Output: ponteiro para unsigned que contem o codigo ASCII do caracter armazenado no mapa
    
    Pré condição: argumentos fornecidos nao nulos
    
    Pós condicao: -
*/
unsigned* pegar_endereco_ASCII_mapa(mapa* map);

/*  calcular_tamanho_bits_mapa
    Descrição: calcula o tamanho necessario em bits para representar o mapa utilizando um bitmap
    
    Input: mapa
    
    Output: número inteiro (unsigned long) necessário para representar o mapa utilizando um bitmap
    
    Pré condição: mapa precisa ter passado pelo algoritmo de huffman
    
    Pós condicao: -
*/
unsigned long calcular_tamanho_bits_mapa(mapa* map);

#endif