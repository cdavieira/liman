#include "container/Tree.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Tree{
	size_t id;
	void* item;
	Tree* left;
	Tree* right;
};

Tree* tree_new(void* item, Tree* left, Tree* right){
	static size_t id = 0;
	Tree* t = calloc(1, sizeof(Tree));
	t->id = id++;
	t->item = item;
	t->left = left;
	t->right = right;
	return t;
}

Tree* tree_destroy(Tree* t, void* (*free_item)(void*)){
	if(!t){
		return NULL;
	}
	t->left = tree_destroy(t->left, free_item);
	t->right = tree_destroy(t->right, free_item);
	if(free_item){
		t->item = free_item(t->item);
	}
	free(t);
	return NULL;
}



void* tree_get_item(Tree* t){
	return t->item;
}

size_t tree_get_id(Tree* t){
	return t->id;
}

Tree* tree_get_left(Tree* t){
	return t ? t->left : NULL;
}

Tree* tree_get_child(Tree* root, unsigned lr){
	switch(lr){
		case 0: return root->left;
		case 1: return root->right;
		default: return NULL;
	}
	return NULL;
}

Tree* tree_get_right(Tree* t){
	return t ? t->right : NULL;
}



Tree* tree_set_item(Tree* t, void* item){
	t->item = item;
	return t;
}

Tree* tree_set_left(Tree* root, Tree* t){
	root->left = t;
	return root;
}

Tree* tree_set_right(Tree* root, Tree* t){
	root->right = t;
	return root;
}



static void tree_print_rec(Tree* node, FILE* fp){
	if(!node){
		return;
	}

	if(node->left){
		fprintf(fp, "    %lu -> %lu;\n", node->id, node->left->id);
		tree_print_rec(node->left, fp);
	}
	if(node->right){
		fprintf(fp, "    %lu -> %lu;\n", node->id, node->right->id);
		tree_print_rec(node->right, fp);
	}
}

void tree_print(Tree* root, FILE* fp){
	fprintf(fp,
	    "digraph Tree {\n"
	    "    node [fontname=\"Arial\"];\n"
	);
	if(!root){
		fprintf(fp, "\n");
	}
	else if (!root->left && !root->right){
		fprintf(fp, "    %lu;\n", root->id);
	}
	else{
		tree_print_rec(root, fp);
	}
	fprintf(fp, "}\n");
}

unsigned tree_is_leaf(Tree* t){
	return t? !t->right && !t->left: 0;
}

Tree* tree_search(Tree* t, void* search, unsigned (*fcmp)(void*, void*)){
	if(!tree_is_leaf(t)){
		Tree* ltree = tree_search(t->left, search, fcmp);
		return ltree ? ltree : tree_search(t->right, search, fcmp);
	}
	return fcmp(t, search) ? t : NULL;
}

unsigned long tree_get_count(Tree* t){
	if(!t){
		return 0;
	}
	return 1 + tree_get_count(t->left) + tree_get_count(t->right);
}

unsigned long tree_get_leaf_count(Tree* t){
	if(!t){
		return 0;
	}
	return tree_is_leaf(t) + tree_get_leaf_count(t->left) + tree_get_leaf_count(t->right);
}

unsigned long tree_get_height(Tree* t){
	if(!t){
		return 0;
	}
	unsigned long lh = tree_get_height(t->left);
	unsigned long rh = tree_get_height(t->right);
	return !tree_is_leaf(t) + (lh > rh ? lh : rh);
}

unsigned tree_exists(Tree* t, Tree* node){
	if(!t){
		return 0;
	}
	return t == node? 1 : tree_exists(t->right, node) || tree_exists(t->left, node);
}

static int tree_get_path(
    Tree* root,
    Tree* node,
    unsigned long rlen,
    unsigned long rcode,
    unsigned long *len,
    unsigned long *code)
{
	if(root == NULL){
		return 0;
	}
	if(root == node){
		*len = rlen;
		*code = rcode;
		return 1;
	}
	return \
	  tree_get_path(root->left,  node, rlen+1, (rcode << 1) | 0, len, code) ||
	  tree_get_path(root->right, node, rlen+1, (rcode << 1) | 1, len, code);
}

char* tree_find_path_str(Tree* t, Tree* node){
	unsigned long codeLen;
	unsigned long nodeCode;
	if(!tree_get_path(t, node, 0, 0, &codeLen, &nodeCode)){
		return NULL;
	}
	char* route = malloc((codeLen+1)*sizeof(char));
	for(int i=codeLen-1, j=0; i>=0; i--, j++){
		route[j] = '0' + ((nodeCode >> i) & 1);
	}
	route[codeLen] = '\0';
	return route;
}

int tree_find_path(Tree* root, Tree* node, unsigned long *codeLen, unsigned long *nodeCode){
	return tree_get_path(root, node, 0, 0, codeLen, nodeCode);
}

Tree* tree_descend(Tree* node, unsigned long codeLen, unsigned long code){
	unsigned char bit;
	for(int i=codeLen-1; node && i>=0; i--){
		bit = (code >> i) & 1;
		if(bit == 0){
			node = node->left;
		}
		else{
			node = node->right;
		}
	}
	return node;
}
