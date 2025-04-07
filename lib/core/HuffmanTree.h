#pragma once

typedef struct bitmap bitmap;
typedef struct HuffmanTree HuffmanTree;
#include <stdio.h>

//Instantiation/destruction
HuffmanTree* huffmanTree_new(unsigned int ASCII, unsigned long weight, HuffmanTree* left, HuffmanTree* right);
HuffmanTree* huffmanTree_destroy(HuffmanTree* hufftree);

//Getters
size_t huffmanTree_get_id(HuffmanTree* node);
unsigned int huffmanTree_get_ASCII(HuffmanTree* node);
unsigned* huffmanTree_get_ASCII_addr(HuffmanTree* hufftree);
unsigned long huffmanTree_get_weight(HuffmanTree* node);
bitmap* huffmanTree_get_bitmap(HuffmanTree* node);
HuffmanTree* huffmanTree_get_left(HuffmanTree* hufftree);
HuffmanTree* huffmanTree_get_right(HuffmanTree* hufftree);

//Setters
HuffmanTree* huffmanTree_set_left(HuffmanTree* hufftree, HuffmanTree* filho);
HuffmanTree* huffmanTree_set_right(HuffmanTree* hufftree, HuffmanTree* filho);
HuffmanTree* huffmanTree_set_bitmap(HuffmanTree* hufftree, bitmap* bm);

//To be or not to be
unsigned huffmanTree_is_leaf(HuffmanTree* hufftree);

//Recursive calls
unsigned long huffmanTree_get_height(HuffmanTree* hufftree);
unsigned long huffmanTree_get_count(HuffmanTree* hufftree);
unsigned long huffmanTree_get_leaf_count(HuffmanTree* hufftree);

/* constroi o trajeto necessário para se chegar a um determinado HuffmanTree de uma arvore */
char* huffmanTree_find_path(HuffmanTree* arvore, HuffmanTree* node);

/* percorre a estrutura de um HuffmanTree utilizando as instruções contidas em rota */
HuffmanTree* huffmanTree_descend(HuffmanTree* hufftree, unsigned long codeLen, unsigned long code);

/* busca por um caracter ASCII em toda estrutura do HuffmanTree raiz fornecido */
HuffmanTree* huffmanTree_search_ASCII(HuffmanTree* hufftree, unsigned* ASCII);

/* h1.weight > h2.weight ? 1 : 0 */
int huffmanTree_compare_freq(HuffmanTree* t1, HuffmanTree* t2);

/* calcula o tamanho necessario em bits para representar a mensagem do HuffmanTree */
unsigned long huffmanTree_getMsgSize(HuffmanTree* hufftree);



/* imprime estrutura da arvore no formato .dot */
void huffmanTree_print(HuffmanTree* hufftree, FILE* fp);
void huffmanTree_print_codes(HuffmanTree* hufftree, FILE* fp);
