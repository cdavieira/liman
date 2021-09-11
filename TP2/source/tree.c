#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tree{
	void* conteudo;
	tree* sae;
	tree* sad;
};

tree* criar_arvore(void* conteudo, nodulo* sae, nodulo* sad){
	tree* arv = alocar_arvore();
	return preencher_arvore(arv, conteudo, sae, sad);;
}

tree* alocar_arvore(){
	return calloc(1, sizeof(tree));
}

tree* desalocar_arvore(tree* arvore, void* (*desalocar_conteudo)()){
	if(arvore){
		if((*arvore).sae) desalocar_arvore((*arvore).sae, desalocar_conteudo);
		if((*arvore).sad) desalocar_arvore((*arvore).sad, desalocar_conteudo);
		if(desalocar_conteudo) desalocar_conteudo((*arvore).conteudo);
		free(arvore);
	}
	return 0;
}

tree* preencher_arvore(tree* arvore, void* conteudo, nodulo* sae, nodulo* sad){
	if(arvore) *arvore = (tree){.conteudo = conteudo, .sae = sae, .sad = sad};
	return arvore;
}

tree* adicionar_nodulo(tree* mae, nodulo* filho, int codigo){
	if(!mae) return filho;
	if(codigo==left) (*mae).sae = filho;
	else (*mae).sad = filho;
	return mae;
}

tree* remover_nodulo(tree* mae, int codigo){
	tree* no_removido = codigo==left?(*mae).sae:(*mae).sad;
	if(codigo==left) (*mae).sae = 0;
	else (*mae).sad = 0;
	return no_removido;
}

void imprimir_arvore(tree* mae, void (*exibir_conteudo)()){
	if(mae){
		printf("<");
		if(exibir_conteudo) exibir_conteudo((*mae).conteudo);
		if((*mae).sae) imprimir_arvore((*mae).sae, exibir_conteudo);
		if((*mae).sad) imprimir_arvore((*mae).sad, exibir_conteudo);
		printf(">");
	}
}

unsigned testar_nodulo(nodulo* candidato){
	return candidato?!((*candidato).sad&&!(*candidato).sae?1:0):0;
}

void* pegar_conteudo_arvore(tree* arvore){
	return arvore?(*arvore).conteudo:0;
}

tree* pegar_sae_arvore(tree* arvore){
	return arvore?(*arvore).sae:0;
}

tree* pegar_sad_arvore(tree* arvore){
	return arvore?(*arvore).sad:0;
}

/*
tree* buscar_conteudo_arvore(tree* arvore, void* informacao_pesquisada, void* (*pegar_informacao_arvore)(), unsigned (*comparar)()){
	tree* buscado = 0;
	if(arvore && pegar_informacao_arvore && comparar){
		buscado = comparar(pegar_informacao_arvore((*arvore).conteudo), informacao_pesquisada)?arvore:0;
		return buscado || buscar_conteudo_arvore((*arvore).sae, informacao_pesquisada, pegar_informacao_arvore, comparar) || buscar_conteudo_arvore((*arvore).sad, informacao_pesquisada, pegar_informacao_arvore, comparar);
	}
	return buscado;
}
*/