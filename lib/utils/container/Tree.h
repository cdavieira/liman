#pragma once

#include "utils/types/common.h"

typedef struct Tree Tree;

Tree *tree_new(void *conteudo, Tree *sae, Tree *sad);
Tree *tree_destroy(Tree *root, void *(*free_item)(void *));

void *tree_get_item(Tree *root);
Tree *tree_get_left(Tree *root);
Tree *tree_get_right(Tree *root);
Tree *tree_get_child(Tree *root, unsigned lr);
size_t tree_get_id(Tree *root);

Tree *tree_set_item(Tree *t, void *item);
Tree *tree_set_left(Tree *root, Tree *filho);
Tree *tree_set_right(Tree *root, Tree *filho);

/**
 * tells if this node is a leaf node or not
 * a leaf node doesn't have neither a left nor a right node
 *
 * @return 1 in case the node is a leaf node, 0 otherwise
 */
unsigned tree_is_leaf(Tree *node);

/**
 * searches for a node in the tree, such that: fcmp(node->item, item) == 1
 */
Tree *tree_search(Tree *root, void *search, unsigned (*fcmp)(void *, void *));

/**
 * count the number of nodes in a tree
 */
unsigned long tree_get_count(Tree *root);

/**
 * count the number of leaf nodes in a tree
 */
unsigned long tree_get_leaf_count(Tree *root);

/**
 * return the height of a tree
 */
unsigned long tree_get_height(Tree *root);

void tree_visit_leafs(Tree *root, void (*callback)(Tree *leaf, unsigned height,
                                                   unsigned long path));
