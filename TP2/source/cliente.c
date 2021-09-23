#include "cliente.h"
#include "tree.h"
#include "compactador.h"
#include "descompactador.h"
#include "mapa.h"
#include "listaTree.h"
#include "bitmap.h"
#include "bitmapPLUS.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void imprime(mapa* map);

static void assert(void* p, char* msg);

static void assert(void* p, char* msg){
    if(!p){
        puts(msg);
        exit(1);
    }
}

//nome dessa funcao deve mudar para nao confundir depois com a funcao que descompacta
mapa* montar_mapa(char* nome){
    mapa* map = 0;
    if(nome){
        FILE* fpin = fopen(nome, "rb");
        assert(fpin, "Erro ao abrir arquivo para leitura! Terminando programa...");
        unsigned long tabela_ocorrencias[128] = {0, };      //armazena a frequencia de cada letra no arquivo texto
        for(char c=fgetc(fpin);c!=EOF;c=fgetc(fpin)) tabela_ocorrencias[c]++;
        listaArvores* ls = criar_listaArvores(128);
        for(unsigned i=pegar_indice_maior_numero(tabela_ocorrencias, 128);i;i=pegar_indice_maior_numero(tabela_ocorrencias, 128)){
            ls = adicionar_listaArvores(ls, criar_mapa(i, tabela_ocorrencias[i], 0, 0), 0); //a adicao do maior numero é sempre feita na posicao inicial da lista circular. Isso assegura que ao final o menor elemento estará na posição inicial da lista (também é possível fazer uma função que pega o indice do menor numero e o adiciona)
            tabela_ocorrencias[i] = 0;
        }
        map = algoritmo_Huffman(ls); //constroi um arvore inteira baseando-se numa lista circular (esvazia e desaloca essa no processo)
        gravar_codigos_mapa(map);
        fclose(fpin);
    }
    return map;
}

//o nome dessa funcao poderia ser compacta
void compactar(char* nomeArquivoTexto, char* nomeArquivoBinario, mapa* mapa_caracteres){
    if(nomeArquivoTexto && nomeArquivoBinario && mapa_caracteres){
        FILE* fpin = fopen(nomeArquivoTexto, "rb"), *fpout = fopen(nomeArquivoBinario, "wb");
        assert(fpin, "Erro ao abrir arquivo para leitura! Terminando programa...");
        assert(fpout, "Erro ao gerar arquivo compactado! Terminando programa...");
        exportar_mapa_formato_bitmap(mapa_caracteres, fpout); //escrevendo mapa de caracteres no começo do arquivo
        exportar_texto_formato_bitmap(mapa_caracteres, fpin, fpout); //iniciando gravacao do conteudo do arquivoTexto no arquivo binario
        fclose(fpin);
        fclose(fpout);
    }
}

//ler um bit e verificar se esse bit consegue me levar ate algum no folha da minha arvore;se conseguir, entao chegamos a uma letra, caso contrario continua leitura;
void descompactar(char* nomeArquivoBinario, char* nomeArquivoSaida){
    if(nomeArquivoBinario && nomeArquivoSaida){
        FILE *fpin = fopen(nomeArquivoBinario, "rb"), *fpout = fopen(nomeArquivoSaida, "w");
        assert(fpin, "Erro ao gerar arquivo compactado! Terminando programa...");
        bitmap* bm = remontar_mapa_forma_bitmap(fpin);
        //a função anterior foi pensada para deixar propositalmente o leitor do arquivo fpin posicionada para a leitura do primeiro caracter da descompactação, por isso a lógica seguinte parte desse pressuposto
        mapa* map = reconstruir_mapa_forma_original(bm); //nao utiliza fpin ainda
        bitmapLibera(bm);
        /*
        printf("Mapa reconstruido do arquivo binario");
        imprimir_mapa(map);
        putchar('\n');
        */
        traduzir_mensagem(fpin, fpout, map);
        liberar_mapa(map);        
        fclose(fpin);
        fclose(fpout);
    }
}

static void imprime(mapa* map){
    if(map){
        printf("%c", pegar_ASCII_mapa(map));
    }
}