#include "descompactador.h"
#include "bitmap.h"
#include "convencoes.h"
#include "mapa.h"
#include "analisar-compactado.h"
#include <stdlib.h>

static mapa* reconstruir_mapa_forma_original_aux(bitmap* bm, unsigned *index, unsigned modo);

static mapa* reconstruir_mapa_forma_original_aux_ler_no_folha(bitmap* bm, unsigned *index);

//MODO DE LEITURA: QUANTO MAIS A ESQUERDA (BIT MAIS SIGNIFICATIVO) QUANTO MAIS A DIREITA (BIT MENOS SIGNIFICATIVO)
//EXEMPLO: 00101101 = 0*2^7 + 0*2^6 + 1*2^5 + 0*2^4 +  1*2^3 + 1*2^2 + 0*2^1 + 1*2^0
bitmap* remontar_mapa_forma_bitmap(FILE* fpin){
    bitmap* bm = 0;
    if(fpin){
        unsigned long tam_bm = contar_bits_mapa(fpin);   //o numero inicial contido no arquivo binario ; note que o numero que é gravado inicialmente no arquivo compactado não inclui em sua contagem os zeros de preenchimento adicionais que foram colocados no bitmap escrito (rever funcao exportar_mapa_formato_bitmap)
        if(tam_bm){
            bm = bitmapInit(tam_bm);
            char c = 0;
            unsigned long i = 0;
            while(i<tam_bm){
                c = fgetc(fpin);    //a leitura utilizando fgetc garante que um número de bits multiplo de 8 será lido ; de fato, o número de bits escrito no arquivo para o mapa foi pensado para ser um múltiplo de oito
                for(unsigned j=0;j<8;j++){  //esse for só será possível, porque o tamanho do bitmap gerado pela funcao exportar_mapa_formato_bitmap é sempre um multiplo de 8. Logo, esse loop não põe o programa em risco de SEGFAULT em nenhum caso e nem adianta a leitura de uma parte do arquivo relacionada a mensagem codificada
                    bitmapAppendLeastSignificantBit(bm, ((((unsigned char)c)&0x80)>>7)); //0x80: 128, pegando o bit da 8ª casa do char. Esse valor tem peso 2^7, por isso precisamos fazer rshift dele em 7 casas para converte-lo em 0 ou 1 (2^0)
                    i++;
                    if(i==tam_bm) break;
                    c = c<<1;       //atualizando e colocando um novo bit na posicao de bit mais significante
                }
            }
        }
    }
    return bm;
}

mapa* reconstruir_mapa_forma_original(bitmap* bm){
    unsigned index = 0;
    return reconstruir_mapa_forma_original_aux(bm, &index, left); //passando como ponteiro isso permite que o index seja atualizado em qualquer nivel da chamada recursiva
}

//O que faz: função auxiliar que de fato dá inicio a rotina de reconstrução do mapa na forma de bitmap para a forma original
static mapa* reconstruir_mapa_forma_original_aux(bitmap* bm, unsigned *index, unsigned modo){
    mapa* map = 0;
    if(bm){ 
        if(*index<bitmapGetLength(bm)){
            unsigned char bit = bitmapGetBit(bm, (*index)++); //pega o bit da posicao index e DEPOIS da chamada da funcao incrementa em 1 o indice
            if(bit) map = reconstruir_mapa_forma_original_aux_ler_no_folha(bm, index);  //se o bit for um significa que estamos lendo um nó folha e chamamos a rotina de leitura de nós folhas
            else{   //se o bit for zero significa que estamos lendo um nó não folha, então criamos um mapa virgem e adicionamos seus nós filhos chamando recursivamente a função de construção da árvore 
                map = criar_mapa(0, 0, 0, 0);
                map = adicionar_rota(map, reconstruir_mapa_forma_original_aux(bm, index, left), left);
                map = adicionar_rota(map, reconstruir_mapa_forma_original_aux(bm, index, right), right);
            }
        }
    }
    return map;
}

//O que faz: função auxiliar 2 que trata a reconstrução dos nós folhas na forma de bits para a forma original
static mapa* reconstruir_mapa_forma_original_aux_ler_no_folha(bitmap* bm, unsigned *index){
    mapa* map = 0;
    if(bm){
        unsigned char ascii = 0, bit = 0;
        for(unsigned i=0;i<8;i++){          //bit passa a ser exercer um papel diferente
            bit = bitmapGetBit(bm, (*index)++); //lendo 8 bits em sequência (trata-se do codigo ASCII)
            bit<<=(7-i);                    //quanto mais a esquerda, mais significativo é o bit
            ascii|=bit;
        }
        map = criar_mapa(ascii, 0, 0, 0);      //finalizando a leitura do primeiro no-folha  
    }
    return map;
}

//assume-se que o ponteiro no arquivo fpin está posicionado logo no inicio da mensagem codificada
void traduzir_mensagem(FILE* fpin, FILE* fpout, mapa* mapa_caracteres){
    if(fpin && fpout && mapa_caracteres){
        unsigned long altura = calcular_altura_mapa(mapa_caracteres); //tamanho maximo que o codigo pode ter
        unsigned char *codigo = calloc(altura+1, sizeof(unsigned char)), u=0, bit=0;
        unsigned bits_disponiveis_codigo = altura, stopcode = 0;
        mapa* corrente = 0;
        for(char c = fgetc(fpin); !feof(fpin) ;c = fgetc(fpin)){ //foi necessário mudar a condição do for para a funcao feof em vez de c!=EOF, pois é possível que o arquivo binario apresente EOF como caracter utilizado para carregar a mensagem
            u = c;                          //convertendo char para unsigned char
            for(unsigned i=0;i<8;i++){      //analise bit a bit do char lido
                if(stopcode) break;
                bit = u&0x80;               //recuperando bit mais a esquerda do unsigned char
                bit>>=7;                    //convertendo esse numero para 0 ou 1
                u<<=1;                      //atualizando o valor de u (char em analise)
                if(!bits_disponiveis_codigo){ //caso não existam mais bits disponíveis no cache de tamanho igual ao numero máximo de caracteres que podem ser usados para se chegar a um nó folha
                    free(codigo);           //libera-se o código previamente construído para se adicionar um novo bit
                    codigo = 0;
                    codigo = calloc(altura+1, sizeof(unsigned char)); //incluindo o '/0' final //alocando novamente o codigo para que possa ser reconstruído
                    bits_disponiveis_codigo = altura;   //atualizando o numero de bits disponiveis no buffer para o máximo novamente
                }
                *(codigo+altura-bits_disponiveis_codigo) = bit?'1':'0'; //atribuindo de fato o valor do bit no indice especifico em que deve ser adicionado
                corrente = percorrer_mapa(mapa_caracteres, codigo);     //percorrendo árvore de mapas
                if(testar_folha_mapa(corrente)){                        //caso o mapa obtido no deslocamento anterior seja um nó folha, então significa que o código que construímos nos levou a um caracter codificado válido da árvore de codificação
                    if(!pegar_ASCII_mapa(corrente)) stopcode=1;         //caso o mapa obtido seja um \0 isso significa que chegamos ao fim da mensagem e que devemos parar de executar a rotina do loop
                    if(stopcode) break;
                    fprintf(fpout, "%c", pegar_ASCII_mapa(corrente));   //caso o caracter seja diferente de \0 grava-se ele no arquivo de saida
                    bits_disponiveis_codigo=1; //isso forçara a refazer o codigo na proxima iteracao
                }
               --bits_disponiveis_codigo; //atualizando o numero de bits disponíveis
            }
        }
        free(codigo); //para garantir que nçao reste lixo ao termino da função
        codigo = 0;
    }
}