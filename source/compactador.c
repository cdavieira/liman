#include "compactador.h"
#include "mapa.h"
#include "listaTree.h"
#include "bitmap.h"
#include "bitmapPLUS.h"
#include "analisar-compactado.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ascii_char_set 257 //consiste no tamanho do vetor onde serão armazenadas as ocorrencias dos caracteres ASCII. Em outras palavras, corresponde ao conjunto total de caracteres ASCII que devem ser considerados no mapeamento (padrão 128)

mapa* montar_mapa(FILE* fpin){
    mapa* map = 0;
    if(fpin){
        unsigned long tabela_ocorrencias[ascii_char_set] = {1, };      //armazena a frequencia de cada letra no arquivo texto
        for(int c=fgetc(fpin);!feof(fpin);c=fgetc(fpin)){
            tabela_ocorrencias[c]++;
        }
        listaArvores* ls = criar_listaArvores(ascii_char_set);
        for(long i=pegar_indice_maior_numero(tabela_ocorrencias, ascii_char_set);i!=-1;i=pegar_indice_maior_numero(tabela_ocorrencias, ascii_char_set)){
            ls = adicionar_listaArvores(ls, criar_mapa(i, tabela_ocorrencias[i], 0, 0), 0); //a adicao do maior numero é sempre feita na posicao inicial da lista circular. Isso assegura que ao final o menor elemento estará na posição inicial da lista (também é possível fazer uma função que pega o indice do menor numero e o adiciona)
            tabela_ocorrencias[i] = 0;  //limpando a existência do caracter no buffer após adição na lista
        }	
        map = algoritmo_Huffman(ls); //constroi um arvore inteira baseando-se numa lista circular (esvazia e desaloca essa no processo)
        gravar_codigos_mapa(map);
        rewind(fpin); //para a chamada da funcao exportar_texto depois
    }
    return map;
}

long pegar_indice_maior_numero(unsigned long* v, size_t tam){
    long index=-1;
    unsigned long maior = 0;    
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

mapa* algoritmo_Huffman(listaArvores* lc){
    mapa* mapa_completo = 0;
    if(lc){
        mapa *aux1 = 0, *aux2 = 0;
        while(pegar_numero_elementos_listaArvores(lc)>1){   //se a lista tiver mais que um elemento, entao o algoritmo de huffman não terminou
            aux1 = pegar_mapa_listaArvores(lc, 0);          //pegar o primeiro elemento
            lc = remover_listaArvores(lc, 0);               //remove-lo da lista (será somado e depois adicionado novamente)
            aux2 = pegar_mapa_listaArvores(lc, 0);          //pegar o segundo elemento
            lc = remover_listaArvores(lc, 0);               //remove-lo da lista (será somado e depois adicionado novamente)
            lc = adicionar_ordenadamente_listaArvores(lc, criar_mapa(0, pegar_peso_mapa(aux1)+pegar_peso_mapa(aux2), aux1, aux2)); //soma e readicao do resultado numa posicao da lista que preserve seu ordenamento crescente)
        }
        mapa_completo = pegar_mapa_listaArvores(lc, 0);     //nesse ponto, o loop de cima terminou sua execução, o que indica que há apenas um elemento na lista circular. Esse elemento é a arvore completamente construida
        lc = remover_listaArvores(lc, 0);                   //remover a arvore da lista de arvores
        lc = liberar_listaArvores(lc);                      //opcional: liberar a lista passada
    }
    return mapa_completo;
}

void gravar_codigos_mapa(mapa* map){
    if(map) gerar_codigos_mapa_auxiliar(map, map);
}

//funcao auxiliar da sua semelhante ; a unica diferença é que preserva o mapa pai na chamada das funcoes e realmente faz o trabalho acontecer
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

/*
    a arvore é exibida da seguinte maneira: 0 significa um nodulo raiz, 1 significa um nodulo folha
    a precedencia é sempre: nodulo raiz -> nodulo esquerdo -> nodulo direito
    quando for indicado um nodulo folha (1), logo em seguida imprime-se 1 byte que indica o codigo na tabela ASCII do caracter armazenado nesse nodulo
    resumo: imprimir os bits do mapa; quando chegar numa folha (1) imprimir 1 byte indicando qual letra esta nessa folha; continuar até não restarem mais nós folhas
*/
void exportar_mapa_formato_bitmap(mapa* map, FILE* fpout){
    if(map && fpout){
        unsigned long tam_bm = contar_nodes_mapa(map)+(contar_folhas_mapa(map)*8);
        tam_bm = tam_bm%8?(tam_bm+(8-tam_bm%8)):tam_bm;        //colocando bits adicionais de preenchimento no bitmap (evitar problemas na leitura/descompactacao do mapa) //os bits adicionais completarão o numero até que seja um multiplo de 8 ; esses bits adicionais serao zeros na impressao
        bitmap* bm = bitmapInit(tam_bm);
        gerar_mapa_formato_bitmap(map, bm);  //preenchendo o bitmap que contem a arvore na forma de bits corridos
        bitmapUnloadContents(bm, fpout);    //descarregando o conteudo do bitmap no arquivo apontado por fpout
        bitmapLibera(bm);
    }
}

void gerar_mapa_formato_bitmap(mapa* map, bitmap* bm){
    if(map && bm){
        unsigned ehFolha = testar_folha_mapa(map);                     //adquirindo informação se nodulo é ou não é folha
        bitmapAppendLeastSignificantBit(bm, (unsigned char) ehFolha);  //apençar a informacao em forma de bit no mapa de bits
        if(!ehFolha){
            gerar_mapa_formato_bitmap(pegar_sae_mapa(map), bm);                 //senao for no folha, entao chamar recursivamente a funcao para os nodulos filhos
            gerar_mapa_formato_bitmap(pegar_sad_mapa(map), bm);
        }
        else{                                                          //se for um no folha, entao é necessario ainda escrever a letra que esta armazenada nesse no folha
            unsigned char letra = pegar_ASCII_mapa(map), bit = 0;
            for(size_t i=0;i<8;i++){        //iterando nas 8 casas ocupantes de um byte (da variavel unsigned char letra lida acima)
                bit = letra&0x80;           //0x80 = 128 :: armazenando o valor contido no bit de menor significância na variável letra
                bit >>= 7;                  //como o bit de menor significância está na última casa da célula, é preciso fazer um rshift de 7 casas para converter o valor para unitário (1 ou 0)
                letra = letra<<1;           //atualiza-se a letra com um lshift para trazer o bit a direita do lido anterior para a posição de leitura da iteração seguinte
                bitmapAppendLeastSignificantBit(bm, (unsigned char) bit); //adiciona-se o bit
            }
        }
    }
}

void exportar_texto_formato_bitmap(mapa* map, FILE* fpin, FILE* fpout){
    if(map && fpin && fpout){
        bitmap* bm_texto = bitmapInit(calcular_tamanho_bits_mapa(map)+8), *bm_codigo_caracter = 0; //max_size = tamanho em bits do mapa, quantidade de bytes alocados pelo conteudo do bitmap = numero em BYTES !!!!! //o +8 serve para que o campo unsigned char* apresente um '/0' ao final
        unsigned int ascii = 0;                         //essa variavel se faz necessaria, pois não é possível armazenar o valor de fgetc numa variavel unsigned, já que essa funcao retorna -1 para o caracter EOF e esse é o fim do loop
        for(int c=fgetc(fpin);!feof(fpin);c=fgetc(fpin)){ //!!! aqui estava a dor de cabeça !!! na inicialização desse for !!!
            ascii = c;
            bm_codigo_caracter = pegar_bitmap_mapa(buscar_ASCII_mapa(map, &ascii));
            bitmapCatContents(bm_texto, bm_codigo_caracter);
        }
        ascii = 0;
        bitmapCatContents(bm_texto, pegar_bitmap_mapa(buscar_ASCII_mapa(map, &ascii))); //adicionando o codigo do caracter nulo ao final
        bitmapUnloadContents(bm_texto, fpout); //descarregando o conteudo do bitmap no arquivo apontado por fpout
        //imprimir_conteudo_emBits("bits.txt", bm_texto);
        bitmapLibera(bm_texto);
    }
}
