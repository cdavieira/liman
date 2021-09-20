#include "compactador.h"
#include "mapa.h"
#include "listaTree.h"
#include "bitmap.h"
#include "bitmapPLUS.h"
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

static void escrever_mapa(mapa* map, FILE* fpout);

static void gerar_bitmap_map(mapa* map, bitmap* bm); //funcao auxiliar da funcao escrever mapa

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

        //ESCREVER O MAPA DE CARACTERES ANTES DE ESCREVER O CONTEUDO NAO ESQUECER
        puts("MAPA DE HUFFMAN QUE ESTA SENDO COMPACTADO");
        escrever_mapa(mapa_caracteres, fpout);
        //parei vendo a impressao do mapa de caracteres no arquivo ; faltou verificar se a impressao das letras dps das folhas esta correta
        //falta tambem modificar os lugares onde aparece stdout para fpout

        bitmap* bm_completo = bitmapInit(calcular_tamanho_bits_mapa(mapa_caracteres)); //max_size = tamanho em bits do mapa, quantidade de bytes alocados pelo conteudo do bitmap = numero em BYTES !!!!!
        unsigned int ascii = 0;
        for(char c=fgetc(fpin);c!=EOF;c=fgetc(fpin)){
            ascii = c;
            bitmapCatContents(bm_completo, pegar_bitmap_mapa(buscar_ASCII_mapa(mapa_caracteres, &ascii)));
        }
        puts("TEXTO QUE ESTA SENDO COMPACTADO");
        bitmapUnloadContents(bm_completo, stdout);
        // bitmapUnloadContents(bm_completo, fpout);
        //ler um bit e verificar se esse bit consegue me levar ate algum no folha da minha arvore;se conseguir, entao chegamos a uma letra, caso contrario continua leitura;
        fclose(fpin);
        fclose(fpout);
        bitmapLibera(bm_completo);
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

static void escrever_mapa(mapa* map, FILE* fpout){
    if(map && fpout){
        //imprimir o tamanho de bits que o mapa apresenta, depois os bits do mapa; quando chegar numa folha (1) imprimir em sequencia 1 byte inidicando qual letra esta nessa folha
        // unsigned long tam = calcular_tamanho_bits_mapa(map);
        unsigned long tam_bm = contar_nodes_mapa(map)+(contar_folhas_mapa(map)*8);
        puts("Numero de bits da arvore:");
        fwrite(&tam_bm, sizeof(unsigned long), 1, stdout);
        // fwrite(&tam, sizeof(unsigned long), 1, fpout);  //escrevendo o numero de bits que serao necessarios para ler e remontar o mapa
        // unsigned long tam_bm = contar_nodes_mapa(map)+(contar_folhas_mapa(map)*8);
        bitmap* bm = bitmapInit(tam_bm);
        gerar_bitmap_map(map, bm);
        bitmapUnloadContents(bm, stdout);
        bitmapLibera(bm);
    }
}

static void gerar_bitmap_map(mapa* map, bitmap* bm){
    if(map && bm){
        unsigned ehFolha = testar_folha_mapa(map);
        bitmapAppendLeastSignificantBit(bm, (unsigned char) ehFolha); //necessário fazer a subtração para converter o conteudo da variavel rota[i] do codigo ASCII para 0 ou 1 (unsigned char)
        if(!ehFolha){
            gerar_bitmap_map(pegar_sae_mapa(map), bm);
            gerar_bitmap_map(pegar_sad_mapa(map), bm);
        }
        else{
            unsigned char letra = pegar_ASCII_mapa(map);
            unsigned int bit = 0;
            for(size_t i=0;i<8;i++){
                bit = letra&0x01;
                letra = letra>>1;
                bitmapAppendLeastSignificantBit(bm, (unsigned char) bit);
            }
        }
    }
}

//funcao desconstruir mapa