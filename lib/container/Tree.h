#pragma once

#include <stdio.h>

typedef struct Tree Tree;

Tree* tree_new(void* conteudo, Tree* sae, Tree* sad);

/**
 * desaloca um ponteiro para árvore genérica
 */
Tree* tree_destroy(Tree* root, void* (*desalocar_conteudo)(void*));

/**
 * adiciona um nó filho a uma árvore raiz/mãe
 */
Tree* tree_set_item(Tree* t, void* item);
Tree* tree_add_left(Tree* root, Tree* filho);
Tree* tree_add_right(Tree* root, Tree* filho);

/**
 * remove um nó filho de uma árvore raiz/mãe
 */
Tree* tree_pop_left(Tree* root);
Tree* tree_pop_right(Tree* root);

/**
 * imprime estrutura da árvore
 */
void tree_print(Tree* root, FILE* fp);

/**
 * testa se a árvore fornecida pode ser um nó folha ou não
 *
 * @return 1 caso a árvore apresente nós filhos nulos, 0 caso contrário
 */
unsigned tree_is_leaf(Tree* candidato);

void* tree_get_item(Tree* root);
Tree* tree_get_left(Tree* root);
Tree* tree_get_right(Tree* root);
size_t tree_get_id(Tree* root);

/**
 * busca por um conteúdo especifíco em toda estrutura da árvore mãe fornecida
 */
Tree* tree_search(Tree* root, void* search, unsigned (*fcmp)());

/**
 * conta o número de nós presentes na estrutura da árvore mãe, incluindo ela mesma
 */
unsigned long tree_get_count(Tree* root);

/**
 * conta o número de nós folha presentes na estrutura da árvore mãe
 */
unsigned long tree_get_leaf_count(Tree* root);

/**
 * calcula a altura da árvore fornecida
 */
unsigned long tree_get_height(Tree* root);

/**
 * calcula a altura do nó fornecido (se existente) na árvore mãe/raiz passada 
 * height should be 0 if t == root 
 */
unsigned long tree_get_height2node(Tree* t, Tree* no, unsigned long height);

/**
 * testa se o nódulo fornecido está presente na estrutura da árvore mãe passada
 */
unsigned tree_exists(Tree* root, Tree* t);

/**
 * constroi o trajeto necessário para se chegar a um determinado nó de uma árvore 
 * */
char* tree_find_path(Tree* root, Tree* t);

/**
 * percorre a estrutura de uma árvore segundo o code
 */
Tree* tree_descend(Tree* root, unsigned long codeLen, unsigned long code);
