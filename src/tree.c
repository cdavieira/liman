#include "tree.h"
#include "convencoes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tree{
	void* conteudo;
	tree* sae;
	tree* sad;
};

static unsigned long maior(unsigned long n1, unsigned long n2){
	return n1>n2?n1:n2;
}

tree* criar_arvore(void* conteudo, nodulo* sae, nodulo* sad){
	tree* arv = alocar_arvore();
	preencher_arvore(arv, conteudo, sae, sad);;
	return arv;
}

tree* alocar_arvore(){
	return calloc(1, sizeof(tree));
}

tree* desalocar_arvore(tree* arvore, void* (*desalocar_conteudo)()){
	if(arvore){
		(*arvore).sae = desalocar_arvore((*arvore).sae, desalocar_conteudo);
		(*arvore).sad = desalocar_arvore((*arvore).sad, desalocar_conteudo);
		if(desalocar_conteudo){
			(*arvore).conteudo = desalocar_conteudo((*arvore).conteudo);
		}
		free(arvore);
	}
	return NULL;
}

void preencher_arvore(tree* arvore, void* conteudo, nodulo* sae, nodulo* sad){
	if(!arvore){
		return ;
	}

	(*arvore).conteudo = conteudo;
	(*arvore).sae = sae;
	(*arvore).sad = sad;
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

void imprimir_arvore(tree* mae, void (*exibir_conteudo_folha)(), void (*exibir_conteudo_nao_folha)()){
	if(!mae || !exibir_conteudo_folha || !exibir_conteudo_nao_folha){
		return ;
	}

	printf("<");
	if(testar_nodulo_folha(mae)){
		exibir_conteudo_folha((*mae).conteudo);
	}
	else{
		exibir_conteudo_nao_folha((*mae).conteudo);
	}
	imprimir_arvore((*mae).sae, exibir_conteudo_folha, exibir_conteudo_nao_folha);
	imprimir_arvore((*mae).sad, exibir_conteudo_folha, exibir_conteudo_nao_folha);
	printf(">");
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

tree* buscar_conteudo_arvore(tree* arvore, void* informacao_pesquisada, void* (*pegar_informacao_arvore)(), unsigned (*comparar)()){
	tree* buscado = 0;
	if(arvore && pegar_informacao_arvore && comparar){
		if(testar_nodulo_folha(arvore)){
			if(comparar(pegar_informacao_arvore(arvore), informacao_pesquisada)) return arvore;
		}
		else{
			if(!buscado) buscado = buscar_conteudo_arvore((*arvore).sae, informacao_pesquisada, pegar_informacao_arvore, comparar);
			if(!buscado) buscado = buscar_conteudo_arvore((*arvore).sad, informacao_pesquisada, pegar_informacao_arvore, comparar);
		}
	}
	return buscado;
}

unsigned long contar_nodulos(tree* arvore){
	if(arvore) return 1 + contar_nodulos((*arvore).sae) + contar_nodulos((*arvore).sad);
	else return 0;
}

unsigned long calcular_nodulos_folha(tree* arvore){
	if(arvore) return testar_nodulo_folha(arvore) + calcular_nodulos_folha((*arvore).sae) + calcular_nodulos_folha((*arvore).sad);
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
	if(!arvore || !nodulo){
		return NULL;
	}

	if(!testar_existencia_nodulo(arvore, nodulo)){
		return NULL;
	}

	char* rota = NULL;

	/* indica se o nodulo pesquisado existe à direito do nodulo raiz */
	int existencia;

	/* altura relativa de um nódulo em relação a raiz */
	/* corresponde ao número de bits necessários para escrever a rota */
	size_t nivel = calcular_altura_nodulo(arvore, nodulo, 0);

	/* alocando o numero de bits necessarios para localização do nodulo na
	 * arvore + 1 caracter de terminação da string */
	rota = calloc(nivel+1, sizeof(char));

	/* armazena os nodulos intermediarios ate o nodulo pesquisado */
	tree *corrente = NULL;

	/* guarda o no pai do no corrente */
	tree *pai_corrente = arvore;
	for(unsigned long i=0; i<nivel; i++){
		// testa a existencia do nodulo pesquisado na raiz direita do no pai
		existencia = testar_existencia_nodulo((*pai_corrente).sad, nodulo);

		// se o nodulo pesquisado existir a direita, ir para direita, senao ir para esquerda
		corrente = existencia?(*pai_corrente).sad : (*pai_corrente).sae;

		// se tivermos ido para a direita no passo anterior, então adicionamos 1 ao char, senao adicionamos 0
		if(existencia){
			strcat(rota, right_str);
		}
		else{
			strcat(rota, left_str);
		}

		// atualizamos o pai da variavel corrente para a proxima iteração
		pai_corrente = corrente;
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

int comparar_conteudos_arvore(tree* arv1, tree* arv2, void* (*pegar_informacao_arvore)(), unsigned (*comparar)()){
	int resultado = 0;
	if(arv1 && arv2 && pegar_informacao_arvore && comparar){
		if(comparar(pegar_informacao_arvore(arv1), pegar_informacao_arvore(arv2))) resultado = 1;
	}
	return resultado;
}
