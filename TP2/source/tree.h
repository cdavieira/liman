#ifndef _tree
#define _tree
#define left 0
#define right 1

typedef struct tree tree;

typedef tree nodulo; //simplesmente para esclarecer algumas partes/intenções do codigo

tree* criar_arvore(void* conteudo, nodulo* sae, nodulo* sad);

tree* alocar_arvore();

tree* desalocar_arvore(tree* arvore, void* (*desalocar_conteudo)());

tree* preencher_arvore(tree* arvore, void* conteudo, nodulo* sae, nodulo* sad);

tree* adicionar_nodulo(tree* mae, nodulo* filho, int codigo);

tree* remover_nodulo(tree* mae, int codigo);

void imprimir_arvore(tree* mae, void (*exibir_conteudo)());

unsigned testar_nodulo(nodulo* candidato);

void* pegar_conteudo_arvore(tree* arvore);

tree* pegar_sae_arvore(tree* arvore);

tree* pegar_sad_arvore(tree* arvore);


//tree* buscar_conteudo_arvore(tree* arvore, void* informacao_pesquisada, void* (*pegar_informacao_arvore)(), unsigned (*comparar)());

#endif