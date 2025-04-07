#pragma once

#include <stddef.h>

typedef struct CircularList CircularList;

/**
 * cria uma nova lista circular, com até no máximo 'max' elementos
 */
CircularList* cl_new(unsigned long max);

/**
 * libera toda memória associada a lista
 */
CircularList* cl_destroy(CircularList* l, void* (*free_element)(void*));

/**
 * retorna o número atual de elementos na lista
 */
size_t cl_get_size(CircularList* lista);

/**
 * retorna o elemento de um indice da lista 
 */
void* cl_get_item(CircularList* lista, size_t indice);

/**
 * adiciona um elemento no indice fornecido
 *
 * caso seja fornecido um indice positivo maior do que o numero atual de elementos e menor do que o número máximo de elementos, o elemento é adicionado ao final da lista circular
 */
CircularList* cl_add(CircularList* lista, void* item, size_t indice);

/**
* adiciona um elemento em um indice da lista circular, de forma que fique ordenada
*
* caso seja fornecido um indice positivo maior do que o numero atual de elementos e menor do que o número máximo de elementos, o elemento é adicionado ao final da lista circular
*/
CircularList* cl_add_inorder(CircularList* lista, void* item, int (*comparar)(void*, void*));

/**
 * retira o item contido no indice i da lista
 *
 * o item removido continua alocado
 */
void* cl_remove(CircularList* lista, size_t indice);
