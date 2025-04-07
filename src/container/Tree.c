#include "container/Tree.h"
#include "utils/math.h"
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

Tree* tree_destroy(Tree* t, void* (*desalocar_item)(void*)){
	if(!t){
		return NULL;
	}
	t->left = tree_destroy(t->left, desalocar_item);
	t->right = tree_destroy(t->right, desalocar_item);
	if(desalocar_item){
		t->item = desalocar_item(t->item);
	}
	free(t);
	return NULL;
}



Tree* tree_set_item(Tree* t, void* item){
	t->item = item;
	return t;
}

Tree* tree_add_left(Tree* root, Tree* filho){
	root->left = filho;
	return root;
}

Tree* tree_add_right(Tree* root, Tree* filho){
	root->right = filho;
	return root;
}

Tree* tree_pop_left(Tree* root){
	Tree* t = root->left;
	root->left = NULL;
	return t;
}

Tree* tree_pop_right(Tree* root){
	Tree* t = root->right;
	root->right = NULL;
	return t;
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

//dot format
void tree_print(Tree* root, FILE* fp){
	fprintf(fp, "digraph Tree {\n");
	fprintf(fp, "    node [fontname=\"Arial\"];\n");
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

unsigned tree_is_leaf(Tree* candidato){
	return candidato? (!candidato->right && !candidato->left ? 1 : 0) : 0;
}

void* tree_get_item(Tree* arvore){
	return arvore->item;
}

size_t tree_get_id(Tree* root){
	return root->id;
}

Tree* tree_get_left(Tree* t){
	return t ? t->left : NULL;
}

Tree* tree_get_right(Tree* t){
	return t ? t->right : NULL;
}

Tree* tree_search(Tree* t, void* search, unsigned (*fcmp)(void*, void*)){
	Tree* tsearch = NULL;
	if(tree_is_leaf(t)){
		if(fcmp(t, search)){
			return t;
		}
	}
	else{
		tsearch = tree_search(t->left, search, fcmp);
		if(!tsearch){
			tsearch = tree_search(t->right, search, fcmp);
		}
	}
	return tsearch;
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
	return !tree_is_leaf(t) + ulgreater(tree_get_height(t->left), tree_get_height(t->right));
}

unsigned long tree_get_height2node(Tree* t, Tree* node, unsigned long height){
	if(!t){
		return 0;
	}
	return t == node? height : 
	  (tree_get_height2node(t->right, node, height+1) + tree_get_height2node(t->left, node, height+1));
}

unsigned tree_exists(Tree* t, Tree* node){
	if(!t){
		return 0;
	}
	return t==node? 1 : (tree_exists(t->right, node) + tree_exists(t->left, node));
}

char* tree_find_path(Tree* t, Tree* node){
	if(!t || !node){
		return NULL;
	}

	if(!tree_exists(t, node)){
		return NULL;
	}

	char* rota = NULL;

	/* indica se o node pesquisado existe à direito do node raiz */
	int existencia;

	/* altura relativa de um nódulo em relação a raiz */
	/* corresponde ao número de bits necessários para escrever a rota */
	size_t nivel = tree_get_height2node(t, node, 0);

	/* alocando o numero de bits necessarios para localização do node na
	 * t + 1 caracter de terminação da string */
	rota = calloc(nivel+1, sizeof(char));

	/* armazena os nos intermediarios ate o node pesquisado */
	Tree *corrente = NULL;

	/* guarda o node pai do node corrente */
	Tree *pai_corrente = t;
	for(unsigned long i=0; i<nivel; i++){
		// testa a existencia do node pesquisado na raiz direita do node pai
		existencia = tree_exists(pai_corrente->right, node);

		// se o node pesquisado existir a direita, ir para direita, senao ir para esquerda
		corrente = existencia?pai_corrente->right : pai_corrente->left;

		// se tivermos ido para a direita node passo anterior, então adicionamos 1 ao char, senao adicionamos 0
		if(existencia){
			strcat(rota, "1");
		}
		else{
			strcat(rota, "0");
		}

		// atualizamos o pai da variavel corrente para a proxima iteração
		pai_corrente = corrente;
	}

	return rota;
}

Tree* tree_descend(Tree* root, unsigned long codeLen, unsigned long code){
	Tree* node = NULL;
	unsigned char bit;
	for(unsigned long i=0; i<codeLen; i++){
		bit = (code >> (codeLen - i - 1)) & 1;
		if(bit == 0){
			node = root->left;
		}
		else{
			node = root->right;
		}
		root = node;
		if(!root){
			break;
		}
	}
	return node;
}
