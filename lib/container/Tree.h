#pragma once

#include <stdio.h>

typedef struct Tree Tree;

Tree* tree_new(void* conteudo, Tree* sae, Tree* sad);
Tree* tree_destroy(Tree* root, void* (*free_item)(void*));

void* tree_get_item(Tree* root);
Tree* tree_get_left(Tree* root);
Tree* tree_get_right(Tree* root);
Tree* tree_get_child(Tree* root, unsigned lr);
size_t tree_get_id(Tree* root);

Tree* tree_set_item(Tree* t, void* item);
Tree* tree_set_left(Tree* root, Tree* filho);
Tree* tree_set_right(Tree* root, Tree* filho);

/**
 * print this tree in .dot format
 */
void tree_print(Tree* root, FILE* fp);

/**
 * tells if this node is a leaf node or not
 * a leaf node doesn't have neither a left nor a right node
 *
 * @return 1 if case node is a leaf node, 0 otherwise
 */
unsigned tree_is_leaf(Tree* node);

/**
 * searches for a node in the tree, such that: fcmp(node->item, item) == 1
 */
Tree* tree_search(Tree* root, void* search, unsigned (*fcmp)());

/**
 * count the number of nodes in a tree
 */
unsigned long tree_get_count(Tree* root);

/**
 * count the number of leaf nodes in a tree
 */
unsigned long tree_get_leaf_count(Tree* root);

/**
 * return the height of a tree
 */
unsigned long tree_get_height(Tree* root);

/**
 * checks if 't' can be found from 'root'
 *
 * @return 1 in case it can, 0 otherwise
 */
unsigned tree_exists(Tree* root, Tree* t);

/**
 * generate a string which can be used to navigate through the tree
 * and find a node
 *
 * the string is made of two characters: '0's and '1'
 * '0' indicates that the next tree is the left one
 * '1' indicates that the next tree is the right one
 *
 * @return an allocated string if t belongs to root, otherwise NULL
 * */
char* tree_find_path_str(Tree* root, Tree* t);

/**
 * generate a binary number which can be used to navigate through the tree
 * and find a node
 *
 * the number should be interpreted as a sequence of bits (just like if
 * reading in binary) and should be read from the most significant bit to the
 * least one
 *
 * nodeCode is made of 0's and 1's, where:
 * 0 indicates that the next tree is the left one
 * 1 indicates that the next tree is the right one
 *
 * codeLen is the number of bits that makes up 'nodeCode'
 *
 * For example:
 * codeLen=3 nodeCode=010, node = root->left->right->left
 * codeLen=5 nodeCode=1110, node = root->right->right->right->left
 *
 * @return 1 if node belongs to root, otherwise 0
 * */
int tree_find_path(Tree* root, Tree* node, unsigned long *codeLen, unsigned long *nodeCode);

/**
 * transverse the root node using the given nodeCode. Refer to
 * 'tree_find_path' for a brief explanation on how to use this number to
 * transverse the tree.
 *
 * @return NULL if the nodeCode leads to a NULL node, otherwise a pointer to that node
 */
Tree* tree_descend(Tree* root, unsigned long codeLen, unsigned long code);
