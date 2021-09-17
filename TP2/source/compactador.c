#include "compactador.h"
#include "mapa.h"
#include "listaTree.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*  
    pensando bem nao sera necessario que a variavel do conteudo do mapa armazene o bitmap de cada caracter eu acho, pelo menos nao agora
*/

static void assert(void* p, char* msg);

static unsigned long pegar_indice_maior_numero(unsigned long* v, size_t tam);

//INCLUIR O SEPARADOR (\0) NO ALGORITMO DE HUFFMAN
static mapa* algoritmo_Huffman(listaArvores* lc);

//map deve ter passado pela etapa de huffman
//essa funcao tem como intencao gerar bitmaps e preencher os campos de cada no folha do mapa com seu respectivo bitmap, que contem o seu codigo em bits
static void gerar_codigos_mapa(mapa* map); //talvez seja um desperdicio ter feito duas funcoes (uma principal e uma auxiliar aqui no backend)

//funcao auxiliar da sua semelhante ; a unica diferença é que preserva o mapa pai na chamada das funcoes e realmente faz o trabalho acontecer
static void gerar_codigos_mapa_auxiliar(mapa* filho, mapa* pai);

mapa* ler_arquivo(char* nome){
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
        gerar_codigos_mapa(map);
        fclose(fpin);
    }
    return map;
}

void escrever_arquivo_binario(char* nomeArquivoTexto, char* nomeArquivoBinario, mapa* mapa_caracteres){
    if(nomeArquivoTexto && nomeArquivoBinario && mapa_caracteres){
        FILE* fpin = fopen(nomeArquivoTexto, "rb"), *fpout = fopen(nomeArquivoBinario, "wb");
        assert(fpin, "Erro ao abrir arquivo para leitura! Terminando programa...");
        assert(fpout, "Erro ao gerar arquivo compactado! Terminando programa...");
        

        fclose(fpin);
        fclose(fpout);
    }
}


static void assert(void* p, char* msg){
    if(!p){
        puts(msg);
        exit(1);
    }
}

static unsigned long pegar_indice_maior_numero(unsigned long* v, size_t tam){
    unsigned long maior = 0, index=0;
    if(v){
        for(unsigned long i=0;i<tam;i++){
            if(v[i]){
                maior=maior?(v[i]>maior?v[i]:maior):v[i];
                index = maior==v[i]?i:index;
            }
        }
    }
    return index;
}

static mapa* algoritmo_Huffman(listaArvores* lc){
    mapa* mapa_completo = 0;
    if(lc){
        mapa *aux1 = 0, *aux2 = 0;
        while(pegar_numero_elementos_listaArvores(lc)>1){   //se a lista tiver mais que um elemento, entao o algoritmo de huffman não terminou
            aux1 = pegar_mapa_listaArvores(lc, 0);          //pegar o primeiro elemento
            lc = remover_listaArvores(lc, 0);               //remove-lo da lista (será somado e depois adicionado novamente)
            aux2 = pegar_mapa_listaArvores(lc, 0);          //pegar o segundo elemento
            lc = remover_listaArvores(lc, 0);               //remove-lo da lista (será somado e depois adicionado novamente)
            lc = adicionar_listaArvores(lc, criar_mapa(0, pegar_peso_mapa(aux1)+pegar_peso_mapa(aux2), aux1, aux2), -1); //soma e readicao do resultado no final da lista (index -1))
        }
        mapa_completo = pegar_mapa_listaArvores(lc, 0);     //nesse ponto, o loop de cima terminou sua execução, o que indica que há apenas um elemento na lista circular. Esse elemento é a arvore completamente construida
        lc = remover_listaArvores(lc, 0);                   //remover a arvore da lista de arvores
        lc = liberar_listaArvores(lc);                      //opcional: liberar a lista passada
    }
    return mapa_completo;
}

static void gerar_codigos_mapa(mapa* map){
    if(map) gerar_codigos_mapa_auxiliar(map, map);
}

static void gerar_codigos_mapa_auxiliar(mapa* filho, mapa* pai){
    if(filho && pai){
        if(testar_folha_mapa(filho)){   //se a arvore filho for um no-folha, proceder com verificacao
            char* rota = encontrar_rota_node_mapa(pai, filho);  //se a arvore filho for no-folha e pertencer a arvore pai, entao a variavel rota é nao nula e recebe o codigo contendo o pathing correto que leva da arvore pai a arvore filha
            if(rota){
                size_t tam = strlen(rota);
                bitmap* bm = bitmapInit((unsigned)tam);         //o tamanho (tam) da string rota corresponde exatamente ao numero de bits que serao utilizados para construcao do bitmap de um caracter
                for(size_t i=0;i<tam;i++){
                    bitmapAppendLeastSignificantBit(bm, (unsigned char) (rota[i]-'0')); //necessário fazer a subtração para converter o conteudo da variavel rota[i] do codigo ASCII para 0 ou 1 (unsigned char)
                }
                filho = preencher_bitmap_mapa(filho, bm);
            }
            free(rota); //é necessario liberar a memoria da string previamente alocada pois a funcao bitmapInit armazena uma string nova internamente que tem tamanho e conteudo semelhante ao fornecido
            rota = 0;
        }
        gerar_codigos_mapa_auxiliar(pegar_sae_mapa(filho), pai);
        gerar_codigos_mapa_auxiliar(pegar_sad_mapa(filho), pai);
    }
}


//funcao desconstruir mapa

//funcao que informa o codigo de uma letra

//transmissao: a transmissao vai ser feita no com um bloco de tamanho igual a altura da arvore seguido de um bit (0 ou 1) que indica se a o arquivo acabou ou nao