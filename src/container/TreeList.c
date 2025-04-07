#include "container/TreeList.h"
#include "container/CircularLinkedList.h"
#include "core/HuffmanTree.h"

TreeList* treelist_new(unsigned long max){
	return (TreeList*) cl_new(max);
}

TreeList* treelist_destroy(TreeList* l){
	return (TreeList*) cl_destroy((CircularList*) l, (void*) huffmanTree_destroy);
}

size_t treelist_get_size(TreeList* l){
	return cl_get_size((CircularList*) l);
}

HuffmanTree* treelist_get_tree(TreeList* l, long indice){
	return (HuffmanTree*) cl_get_item((CircularList*)l, indice);
}

TreeList* treelist_shift(TreeList* l, HuffmanTree* map){
	return (TreeList*)cl_add((CircularList*)l, (void*) map, 0);
}

TreeList* treelist_add_inorder(TreeList* l, HuffmanTree* mapa){
	return (TreeList*)cl_add_inorder((CircularList*)l, (void*)mapa, (int (*)(void*, void*))huffmanTree_compare_freq);
}

HuffmanTree* treelist_unshift(TreeList* l){
	return (HuffmanTree*)cl_remove((CircularList*)l, 0);
}
