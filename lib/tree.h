#ifndef _tree
#define _tree

typedef struct tree tree;

typedef tree nodulo; //simplesmente para esclarecer algumas partes/intenções do codigo

/*  criar_arvore
    Descrição: cria uma nova árvore genérica e preenche seus campos com os argumentos fornecidos
    
    Input: variável que carrega as informações da arvore, nó da esquerda e nó da direita
    
    Output: ponteiro dinamicamente alocado para árvore preenchida
    
    Pré condição: -
    
    Pós condicao: -
*/
tree* criar_arvore(void* conteudo, nodulo* sae, nodulo* sad);

/*  alocar_arvore
    Descrição: aloca dinamicamente ponteiro para tipo genérico de árvore e seta nulo para seus campos
    
    Input: -
    
    Output: ponteiro dinamicamente alocado para árvore não preenchida
    
    Pré condição: -
    
    Pós condicao: -
*/
tree* alocar_arvore();

/*  desalocar_arvore
    Descrição: desaloca um ponteiro para árvore genérica
    
    Input: ponteiro para tipo genérico de árvore
    
    Output: nulo
    
    Pré condição: ponteiro deve ter sido dinamicamente alocado
    
    Pós condicao: caso receba o retorno dessa função, ponteiro é setado para nulo (proteção a double-free)
*/
tree* desalocar_arvore(tree* arvore, void* (*desalocar_conteudo)());

/*  preencher_arvore
    Descrição: preenche ponteiro de árvore genérica
    
    Input: ponteiro para árvore genérica, ponteiro void que contém informações da árvore, nó filho esquerdo e nó filho direito
    
    Output: 
    
    Pré condição: árvore não nula
    
    Pós condicao: árvore preenchida
*/
void preencher_arvore(tree* arvore, void* conteudo, nodulo* sae, nodulo* sad);

/*  adicionar_nodulo
    Descrição: adiciona um nó filho a uma árvore raiz/mãe
    
    Input: árvore a ter nó adicionado (mãe), nó a ser adicionado (filho) e código que orienta para qual lado deve ser feita a atribuição (direito ou esquerdo)
    
    Output: árvore mãe com nó filho relativo ao codigo fornecido modificado 
    
    Pré condição: codigo deve estar de acordo com os macros LEFT e RIGHT
    
    Pós condicao: -
*/
tree* adicionar_nodulo(tree* mae, nodulo* filho, int codigo);

/*  remover_nodulo
    Descrição: remove um nó filho de uma árvore raiz/mãe
    
    Input: árvore a ter nó removido (mãe) e código que orienta qual nó filho deve ser removido (direito ou esquerdo)
    
    Output: árvore mãe com nó filho removido, relativo ao codigo fornecido 
    
    Pré condição: ponteiro para árvore mãe deve ser não-nula, codigo deve estar de acordo com os macros LEFT e RIGHT
    
    Pós condicao: -
*/
tree* remover_nodulo(tree* mae, int codigo);

/*  imprimir_arvore
    Descrição: imprime estrutura da arvore
    
    Input: arvore a ser impressa, funções que exibem o conteúdo de um nó folha e de um não folha
    
    Output: -
    
    Pré condição: argumentos devem ser não-nulos 
    
    Pós condicao: -
*/
void imprimir_arvore(tree* mae, void (*exibir_conteudo_folha)(), void (*exibir_conteudo_nao_folha)());

/*  testar_nodulo_folha
    Descrição: testa se a árvore fornecida pode ser um nó folha ou não
    
    Input: árvore/nó a ser analisado
    
    Output: 1 caso a árvore apresente nós filhos nulos, 0 caso contrário
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned testar_nodulo_folha(nodulo* candidato);

/*  pegar_conteudo_arvore
    Descrição: disponibiliza o conteúdo disponível na árvore
    
    Input: árvore a ter conteúdo acessado
    
    Output: ponteiro void para conteúdo
    
    Pré condição: argumento deve ser não nulo
    
    Pós condicao: -
*/
void* pegar_conteudo_arvore(tree* arvore);

/*  pegar_sae_arvore
    Descrição: fornece nó filho esquerdo da árvore
    
    Input: árvore mãe a ter nó filho consultado
    
    Output: ponteiro para nó filho esquerdo
    
    Pré condição: árvore mãe deve ser não nula
    
    Pós condicao: -
*/
tree* pegar_sae_arvore(tree* arvore);

/*  pegar_sad_arvore
    Descrição: fornece nó filho direito da árvore
    
    Input: árvore mãe a ter nó filho consultado
    
    Output: ponteiro para nó filho direito
    
    Pré condição: árvore mãe deve ser não nula
    
    Pós condicao: -
*/
tree* pegar_sad_arvore(tree* arvore);

/*  buscar_conteudo_arvore
    Descrição: busca por um conteúdo especifíco em toda estrutura da árvore mãe fornecida
    
    Input: árvore a ser realizada a busca (mãe/raiz), informação buscada, função que acessa informação contida na árvore, função de comparação com retorno não-nulo em caso afirmativo
    
    Output: caso haja êxito, retorna-se a primeira árvore contida na estrutura da árvore mãe que contém o item pesquisado, caso contrário retorna nulo
    
    Pré condição: argumentos fornecidos não nulos
    
    Pós condicao: -
*/
tree* buscar_conteudo_arvore(tree* arvore, void* informacao_pesquisada, void* (*pegar_informacao_arvore)(), unsigned (*comparar)());

/*  contar_nodulos
    Descrição: conta o número de nós presentes na estrutura da árvore mãe, incluindo ela mesma
    
    Input: árvore que contem estrutura a ter nós contabilizados
    
    Output: número de nós contidos na estrutura
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long contar_nodulos(tree* arvore);

/*  calcular_nodulos_folha
    Descrição: conta o número de nós folha presentes na estrutura da árvore mãe
    
    Input: árvore que contem estrutura a ter nós folha contabilizados
    
    Output: número de nós folha contidos na estrutura
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long calcular_nodulos_folha(tree* arvore);

/*  calcular_altura_arvore
    Descrição: calcula a altura da árvore fornecida
    
    Input: árvore a ter altura calculada
    
    Output: altura da arvore
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned long calcular_altura_arvore(tree* arvore);

/*  calcular_altura_nodulo
    Descrição: calcula a altura do nó fornecido (se existente) na árvore mãe/raiz passada 
    
    Input: árvore mãe/raiz, nó de interesse e altura da árvore mãe/raiz (0, caso a árvore mãe não seja nó filho)
    
    Output: altura do nó na estrutura da árvore mãe
    
    Pré condição: argumentos devem ser não-nulos
    
    Pós condicao: -
*/
unsigned long calcular_altura_nodulo(tree* arvore, nodulo* no, unsigned long altura_inicial);

/*  testar_existencia_nodulo
    Descrição: testa se o nódulo fornecido está presente na estrutura da árvore mãe passada
    
    Input: árvore mãe que contém estrutura de nós e nó de interesse
    
    Output: 1 caso o nó pesquisado exista na árvore mãe, 0 caso contrário
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned testar_existencia_nodulo(tree* arvore, nodulo* nodulo);

/*  encontrar_rota_nodulo
    Descrição: constroi o trajeto necessário para se chegar a um determinado nó de uma árvore 
    
    Input: árvore mãe e nó pesquisado
    
    Output: caso encontre o nódulo na árvore, ponteiro para char dinamicamente alocado contendo o trajeto até o nó especificado (utilizando os codigos convencionados de orientação), nulo caso contrário
    
    Pré condição: -
    
    Pós condicao: -
*/
char* encontrar_rota_nodulo(tree* arvore, nodulo* nodulo);

/*  ir_ate_nodulo
    Descrição: percorre a estrutura de uma árvore utilizando as instruções contidas em rota
    
    Input: árvore a ter estrutura percorrida e string contendo orientação de ida, utilizando o codigo convencionado (0s e 1s)
    
    Output: caso suceda retorna o nódulo encontrado após seguir as instruções contidas na rota, caso contrário retorna nulo
    
    Pré condição: -
    
    Pós condicao: -
*/
nodulo* ir_ate_nodulo(tree* arvore, char* rota);

/*  maior
    Descrição: compara dois numeros e retorna se o primeiro é maior que o segundo
    
    Input: dois numeros a serem comparados
    
    Output: 1 caso o primeiro numero seja maior que o segundo, 0 caso contrário

    Pré condição: -
    
    Pós condicao: -
*/
static unsigned long maior(unsigned long n1, unsigned long n2);

/*  comparar_conteudos_arvore
    Descrição: compara os conteudos de duas árvores
    
    Input: duas árvores a serem comparadas
    
    Output: 1 caso bem sucedida a comparação segundo seu critério, 0 caso contrário

    Pré condição: -
    
    Pós condicao: -
*/
int comparar_conteudos_arvore(tree* arv1, tree* arv2, void* (*pegar_informacao_arvore)(), unsigned (*comparar)());

#endif
