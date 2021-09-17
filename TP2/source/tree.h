#ifndef _tree
#define _tree
#define left 0
#define right 1
#define left_char '0'
#define right_char '1'
#define left_str "0"
#define right_str "1"

typedef struct tree tree;

typedef tree nodulo; //simplesmente para esclarecer algumas partes/intenções do codigo

tree* criar_arvore(void* conteudo, nodulo* sae, nodulo* sad);

tree* alocar_arvore();

tree* desalocar_arvore(tree* arvore, void* (*desalocar_conteudo)());

//essa função preenche a arvore caso essa apresente um campo nulo || não é possível preencher a arvore com valores nulos com essa funcao
tree* preencher_arvore(tree* arvore, void* conteudo, nodulo* sae, nodulo* sad);

//essa funcao modifica a arvore completamente com os parametros passados || permite até mesmo passar nulo para os campos da arvore
tree* modificar_arvore(tree* arvore, void* conteudo, nodulo* sae, nodulo* sad);

tree* adicionar_nodulo(tree* mae, nodulo* filho, int codigo);

tree* remover_nodulo(tree* mae, int codigo);

void imprimir_arvore(tree* mae, void (*exibir_conteudo)());

unsigned testar_nodulo_folha(nodulo* candidato);

void* pegar_conteudo_arvore(tree* arvore);

tree* pegar_sae_arvore(tree* arvore);

tree* pegar_sad_arvore(tree* arvore);

tree* buscar_conteudo_arvore(tree* arvore, void* informacao_pesquisada, void* (*pegar_informacao_arvore)(), unsigned (*comparar)());

unsigned long calcular_nodulos(tree* arvore);

unsigned long calcular_altura_arvore(tree* arvore);

unsigned long calcular_altura_nodulo(tree* arvore, nodulo* no, unsigned long altura_inicial);

unsigned testar_existencia_nodulo(tree* arvore, nodulo* nodulo);

char* encontrar_rota_nodulo(tree* arvore, nodulo* nodulo);

nodulo* ir_ate_nodulo(tree* arvore, char* rota);

#endif