#pragma once

#include <stddef.h>

#include "core/HuffmanTree.h"

typedef struct TreeList TreeList;

/**
 * cria uma nova lista circular de arvores
 */
TreeList* treelist_new(unsigned long max);

/**
 * libera lista circular de arvores
 */
TreeList* treelist_destroy(TreeList* lista);

/**
 * retorna o número atual de elementos na lista
 */
size_t treelist_get_size(TreeList* lista);

/**
 * retorna o mapa (arvore) de um indice da lista 
 */
HuffmanTree* treelist_get_tree(TreeList* lista, long indice);

/**
 * adiciona uma arvore no inicio da lista
 *
 * Caso seja fornecido um indice positivo maior do que o numero atual de elementos e menor do que o número máximo de elementos, o elemento é adicionado ao final da lista circular
 */
TreeList* treelist_shift(TreeList* lista, HuffmanTree* map);

/**
 * adiciona um mapa em um indice da lista de arvores, de forma que fique ordenada
 *
 * Caso seja fornecido um indice positivo maior do que o numero atual de elementos e menor do que o número máximo de elementos, o mapa é adicionado ao final da lista de arvores
 */
TreeList* treelist_add_inorder(TreeList* lista, HuffmanTree* mapa);

/**
 * retira a arvore contida no indice 0
 */
HuffmanTree* treelist_unshift(TreeList* lista);
