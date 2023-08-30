#ifndef _compactador
#define _compactador
#include <stdio.h>

typedef struct listaArvores listaArvores;

typedef struct mapa mapa;

typedef struct map bitmap;

mapa* montar_mapa(FILE* fpin);

long pegar_indice_maior_numero(unsigned long* v, size_t tam);

/*  algoritmo_Huffman
    Descrição: performa o algoritmo de Huffmann para construir a árvore de codificação dos caracteres presesentes no arquivo texto e disponíveis na lista circular fornecida
    
    Input: lista circular de árvores singulares contendo caracteres presentes no arquivo texto, já preenchidos com seus respectivos codigos ASCII e suas frequências   
    
    Output: árvore de codificação final dos caracteres presentes no texto
    
    Pré condição: o argumento fornecido deve ser não nulo e deve ter sido alocado
    
    Pós condicao: o parâmetro fornecido é liberado
*/
mapa* algoritmo_Huffman(listaArvores* lc);

/*  gravar_codigos_mapa
    Descrição: preenche os campos de cada no folha do mapa com seu respectivo bitmap, que contem o seu codigo em bits de acordo com a árvore construída
    
    Input: árvore que mapeia caracteres no arquivo texto
    
    Output: -
    
    Pré condição: mapa fornecido deve ter sido gerado pelo algoritmo de Huffmann
    
    Pós condicao: -
*/
void gravar_codigos_mapa(mapa* map);

/*  gerar_codigos_mapa_auxiliar
    Descrição: preenche os campos de cada no folha do mapa com seu respectivo bitmap, que contem o seu codigo em bits de acordo com a árvore construída, preservando a árvore raiz/mãe inicial
    
    Input: árvore filha e árvore pai/mãe/geratriz
    
    Output: -
    
    Pré condição: argumentos fornecidos devem ser não nulos
    
    Pós condicao: -
*/
static void gerar_codigos_mapa_auxiliar(mapa* filho, mapa* pai);

/*  exportar_mapa_formato_bitmap
    Descrição: grava a árvore de caracteres mapeados no formato de bits num arquivo apontado por fpout.
    
    Funcionamento: nódulos não folha são representados por 0's e nódulos folha por 1's. Ao escrever um nódulo folha, escreve-se 1 byte que consiste no caracter ASCII armazenado nesse nó. Esse processo continua até o último nó da árvore
    A escrita é sempre feita com a seguinte precedencia: nó raiz -> no filho esquerdo -> no filho direito

    Input: mapa a ser escrito na forma de bits e ponteiro para arquivo onde esse será gravado
    
    Output: -
    
    Pré condição: mapa deve ter sido gerado pelo algoritmo de Huffmann e argumentos fornecidos devem ser nao nulos
    
    Pós condicao: -
*/
void exportar_mapa_formato_bitmap(mapa* map, FILE* fpout);

/*  gerar_mapa_formato_bitmap
    Descrição: gera o formato em bitmap de um mapa
    
    Input: mapa a ser convertido para bits e bitmap em construção
    
    Output: -
    
    Pré condição: argumentso fornecidos devem ser não nulos
    
    Pós condicao: -
*/
void gerar_mapa_formato_bitmap(mapa* map, bitmap* bm); //funcao auxiliar da funcao escrever mapa

/*  exportar_texto_formato_bitmap
    Descrição: grava o conteudo do arquivo texto apontado por fpin num arquivo apontado por fpout no formato de bits segundo o árvore de caracteres mapeados.
    
    Input: árvore de caracteres mapeados, ponteiro para arquivo texto a ser codificado e ponteiro para arquivo de saída onde será descarregado o conteudo da codificação (em bits)
    
    Output: -
    
    Pré condição: mapa deve ter sido tratado pela função gravar_codigos_mapa e argumentos fornecidos devem ser não-nulos
    
    Pós condicao: -
*/
void exportar_texto_formato_bitmap(mapa* map, FILE* fpin, FILE* fpout);


#endif