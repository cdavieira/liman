#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tree{
	void* conteudo;
	tree* sae;
	tree* sad;
};

static unsigned long maior(unsigned long n1, unsigned long n2);

static unsigned long maior(unsigned long n1, unsigned long n2){
	return n1>n2?n1:n2;
}

tree* criar_arvore(void* conteudo, nodulo* sae, nodulo* sad){
	tree* arv = alocar_arvore();
	return modificar_arvore(arv, conteudo, sae, sad);;
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
	if(arvore){
		if(conteudo) (*arvore).conteudo = conteudo;
		if(sae) (*arvore).sae = sae;
		if(sad) (*arvore).sad = sad;
	}
	return arvore;
}

tree* modificar_arvore(tree* arvore, void* conteudo, nodulo* sae, nodulo* sad){
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

unsigned testar_nodulo_folha(nodulo* candidato){
	return candidato?(!(*candidato).sad&&!(*candidato).sae?1:0):0;
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

//precisa de revisao, dps olhar a outra funcao que se parece com essa
tree* buscar_conteudo_arvore(tree* arvore, void* informacao_pesquisada, void* (*pegar_informacao_arvore)(), unsigned (*comparar)()){
	tree* buscado = 0;
	if(arvore && pegar_informacao_arvore && comparar){
		if(testar_nodulo_folha(arvore)) if(comparar(pegar_informacao_arvore(arvore), informacao_pesquisada)) return arvore;
		else{
			if(!buscado) buscado = buscar_conteudo_arvore((*arvore).sae, informacao_pesquisada, pegar_informacao_arvore, comparar);
			if(!buscado) buscado = buscar_conteudo_arvore((*arvore).sad, informacao_pesquisada, pegar_informacao_arvore, comparar);
		}
	}
	return buscado;
}

unsigned long calcular_nodulos(tree* arvore){
	if(arvore) return testar_nodulo_folha(arvore) + calcular_nodulos((*arvore).sae) + calcular_nodulos((*arvore).sad); 
	else return 0;
}

unsigned long calcular_altura_arvore(tree* arvore){
	if(arvore) return !testar_nodulo_folha(arvore) + maior(calcular_altura_arvore((*arvore).sae),calcular_altura_arvore((*arvore).sad));
	else return 0;
}

unsigned long calcular_altura_nodulo(tree* arvore, nodulo* no, unsigned long altura_inicial){
	if(arvore) return arvore==no?altura_inicial:(calcular_altura_nodulo((*arvore).sad, no, altura_inicial+1)+calcular_altura_nodulo((*arvore).sae, no, altura_inicial+1));
	else return 0;
}

unsigned testar_existencia_nodulo(tree* arvore, nodulo* no){
	if(arvore) return arvore==no?1:(testar_existencia_nodulo((*arvore).sad, no)+testar_existencia_nodulo((*arvore).sae, no));
	else return 0;
}

char* encontrar_rota_nodulo(tree* arvore, nodulo* nodulo){
	char* rota = 0;	//variavel que conterá o caminho correto até o nodulo, caso exista.
	if(arvore && nodulo){
		if(testar_existencia_nodulo(arvore, nodulo)){	//se o nodulo existir na estrutura da arvore mae...
			unsigned long nivel = calcular_altura_nodulo(arvore, nodulo, 0), existencia=testar_existencia_nodulo((*arvore).sad, nodulo)?1:0; //nivel: em que ordem hierarquica está o nodulo pesquisado (importante pois esse é o número de bits necessários para escrevelo), existencia = variavel que testa a existencia do nodulo sempre a direita do nodulo raiz
			rota = calloc(nivel+1, sizeof(char));		//alocando o numero de bits necessarios para localização do nodulo na arvore + 1 caracter de terminação da string
			tree* corrente = existencia?(*arvore).sad:(*arvore).sae;	//armazena os nodulos intermediarios ate o nodulo pesquisado//como a variavel 'existencia' averigua a presença sempre no galho da direita, a variavel 'corrente' depende do valor de existencia, pois pode armazenar a celula da direita ou da esquerda dependendo de qual dessas é o caminho correto ate o nodulo em questao
			for(unsigned long i=0;i<nivel;i++){
				existencia?strcat(rota, right_str):strcat(rota, left_str);	//se existencia=1 isso significa que o nodulo esta a direita, caso contrario esta a esquerda
				corrente = existencia?(*corrente).sad:(*corrente).sae;			//atualiza-se a celula corrente indo-se para a próxima célula pertencente ao pathing correto até o nodulo pesquisado
				existencia = testar_existencia_nodulo(corrente, nodulo);		//atualiza-se a variavel existencia para os nos filhos da celula corrente previamente atualizada
			}
		}
	}
	return rota;
}

nodulo* ir_ate_nodulo(tree* arvore, char* rota){
	nodulo* node = 0;
	if(arvore && rota){
		size_t tam = strlen(rota);
		tree* corrente = arvore;
		for(size_t i=0;i<tam;i++){
			node = rota[i]==left_char?(*corrente).sae:(*corrente).sad;
			corrente = node;
			if(!corrente) break;
		}
	}
	return node;
}