#include "container/CircularLinkedList.h"
#include "types/TreeList.h"
#include "types/HuffmanTree.h"

TreeList* treelist_new(unsigned long max){
	return (TreeList*) cl_new();
}

TreeList* treelist_destroy(TreeList* l){
	return (TreeList*) cl_destroy((CircularList*) l, (void*) huffmanTree_destroy);
}

size_t treelist_get_size(TreeList* l){
	return cl_get_size((CircularList*) l);
}

HuffmanTree* treelist_get_tree(TreeList* l, long idx){
	return (HuffmanTree*) cl_get_item((CircularList*)l, idx);
}

TreeList* treelist_shift(TreeList* l, HuffmanTree* huff){
	return (TreeList*)cl_shift((CircularList*)l, (void*) huff);
}

TreeList* treelist_add_inorder(TreeList* l, HuffmanTree* huff){
	return (TreeList*)cl_add_inorder((CircularList*)l, (void*)huff, (int (*)(void*, void*))huffmanTree_compare_freq);
}

HuffmanTree* treelist_unshift(TreeList* l){
	return (HuffmanTree*)cl_unshift((CircularList*)l);
}
