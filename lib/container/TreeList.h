#pragma once

#include <stddef.h>

#include "core/HuffmanTree.h"

//A circular list of Tree's
typedef struct TreeList TreeList;

TreeList* treelist_new(unsigned long max);
TreeList* treelist_destroy(TreeList* l);

size_t treelist_get_size(TreeList* l);
HuffmanTree* treelist_get_tree(TreeList* l, long idx);

/**
 * add a tree to the beginning of the list
 */
TreeList* treelist_shift(TreeList* l, HuffmanTree* huff);

/**
 * add a tree somewhere in the list, so that it remains sorted
 */
TreeList* treelist_add_inorder(TreeList* l, HuffmanTree* huff);

/**
 * removes the tree of the beginning of the list
 */
HuffmanTree* treelist_unshift(TreeList* l);
