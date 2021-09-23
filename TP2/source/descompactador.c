#include "descompactador.h"
#include "bitmap.h"
#include "tree.h" //so para ter o codigo left e right...
#include "mapa.h"
#include <stdlib.h>
#include <string.h>

static mapa* reconstruir_mapa_forma_original_aux(bitmap* bm, unsigned *index, unsigned modo);

static mapa* reconstruir_mapa_forma_original_aux_ler_no_folha(bitmap* bm, unsigned *index);

//MODO DE LEITURA: QUANTO MAIS A ESQUERDA (BIT MAIS SIGNIFICATIVO) QUANTO MAIS A DIREITA (BIT MENOS SIGNIFICATIVO)
//EXEMPLO: 00101101 = 0*2^7 + 0*2^6 + 1*2^5 + 0*2^4 +  1*2^3 + 1*2^2 + 0*2^1 + 1*2^0
bitmap* remontar_mapa_forma_bitmap(FILE* fpin){
    bitmap* bm = 0;
    if(fpin){
        unsigned long tam_bm = 0;   //o numero inicial contido no arquivo binario ; note que o numero que é gravado inicialmente no arquivo compactado não inclui em sua contagem os zeros de preenchimento adicionais que foram colocados no bitmap escrito (rever funcao exportar_mapa_formato_bitmap)
        fread(&tam_bm, sizeof(unsigned long), 1, fpin); //lendo o numero inicial contido no arquivo binario (tamanho em bits do bitmap a ser lido)
        if(tam_bm){
            bm = bitmapInit(tam_bm);
            char c = 0;
            unsigned long i = 0;
            while(i<tam_bm){
                c = fgetc(fpin);
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


//parei aqui pensando em fazer uma funcao auxiliar que vai ler bit por bit e fazer aquela logica louca
mapa* reconstruir_mapa_forma_original(bitmap* bm){
    unsigned index = 0;
    return reconstruir_mapa_forma_original_aux(bm, &index, left); //passando como ponteiro isso permite que o index seja atualizado em qualquer nivel da chamada recursiva
}

static mapa* reconstruir_mapa_forma_original_aux(bitmap* bm, unsigned *index, unsigned modo){
    mapa* map = 0;
    if(bm){ 
        if(*index<bitmapGetLength(bm)){
            unsigned char bit = bitmapGetBit(bm, (*index)++); //pega o bit da posicao index e DEPOIS da chamada da funcao incrementa em 1 o indice
            if(bit) map = reconstruir_mapa_forma_original_aux_ler_no_folha(bm, index);
            else{
                map = criar_mapa(0, 0, 0, 0);
                map = adicionar_rota(map, reconstruir_mapa_forma_original_aux(bm, index, left), left);
                map = adicionar_rota(map, reconstruir_mapa_forma_original_aux(bm, index, right), right);
            }
        }
    }
    return map;
}

static mapa* reconstruir_mapa_forma_original_aux_ler_no_folha(bitmap* bm, unsigned *index){
    mapa* map = 0;
    if(bm){
        unsigned char ascii = 0, bit = 0;
        for(unsigned i=0;i<8;i++){          //bit passa a ser exercer um papel diferente
            bit = bitmapGetBit(bm, (*index)++);
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
        unsigned bits_disponiveis_codigo = altura;
        mapa* corrente = 0;
        for(char c = fgetc(fpin); c!=EOF ;c = fgetc(fpin)){
            u = c;
            for(unsigned i=0;i<8;i++){
                bit = u&0x80;
                bit>>=7;
                u<<=1;
                if(!bits_disponiveis_codigo){
                    free(codigo);
                    codigo = 0;
                    codigo = calloc(altura+1, sizeof(unsigned char)); //incluindo o '/0' final
                    bits_disponiveis_codigo = altura;
                }
                *(codigo+altura-bits_disponiveis_codigo) = bit?'1':'0';
                corrente = percorrer_mapa(mapa_caracteres, codigo);
                if(testar_folha_mapa(corrente)){
                    fprintf(fpout, "%c", pegar_ASCII_mapa(corrente));
                    bits_disponiveis_codigo=1; //isso forçara a refazer o codigo na proxima iteracao
                }
               --bits_disponiveis_codigo;
            }
        }
        free(codigo);
        codigo = 0;
    }
}

/* bitmap* remontar_mensagem_forma_bitmap(FILE* fpin){
    bitmap* bm = 0;
    if(fpin){
        bm = bitmapInit(tam_bm);
        char c = 0;
        unsigned long i = 0;
        while(i<tam_bm){
            c = fgetc(fpin);
            for(unsigned j=0;j<8;j++){  //esse for só será possível, porque o tamanho do bitmap gerado pela funcao exportar_mapa_formato_bitmap é sempre um multiplo de 8. Logo, esse loop não põe o programa em risco de SEGFAULT em nenhum caso e nem adianta a leitura de uma parte do arquivo relacionada a mensagem codificada
                bitmapAppendLeastSignificantBit(bm, ((((unsigned char)c)&0x80)>>7)); //0x80: 128, pegando o bit da 8ª casa do char. Esse valor tem peso 2^7, por isso precisamos fazer rshift dele em 7 casas para converte-lo em 0 ou 1 (2^0)
                i++;
                if(i==tam_bm) break;
                c = c<<1;       //atualizando e colocando um novo bit na posicao de bit mais significante
            }
        }
    }
    return bm;
} */