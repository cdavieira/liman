#include "descompactador.h"
#include "bitmap.h"
#include "mapa.h"

static mapa* reconstruir_mapa_forma_original_aux(bitmap* bm, unsigned long index);

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

/* para a funcao burra de cima que faz isso com uma confusaozinha
 
            for(unsigned i=0;i<8;i++){
                ascii = bitmapGetBit(bm, ++index)&pos;
                pos<<=1;
            } 
*/

//parei aqui pensando em fazer uma funcao auxiliar que vai ler bit por bit e fazer aquela logica louca
mapa* reconstruir_mapa_forma_original(bitmap* bm){
    return reconstruir_mapa_forma_original_aux(bm, 0);
}

static mapa* reconstruir_mapa_forma_original_aux(bitmap* bm, unsigned long index){
    mapa* map = 0;
    if(bm){
        unsigned char bit = bitmapGetBit(bm, index);
        printf("%lu:%x\n", index, bit);
        if(bit){
            unsigned char ascii = 0;
            unsigned pos = 1;
            for(unsigned i=0;i<8;i++){
                ascii = bitmapGetBit(bm, ++index)&pos;
                pos<<=1;
                printf("%lu\n", index);
            }
            printf("%c\n", ascii);
            map = criar_mapa(ascii, 0, 0, 0);      //parei aqui analisando o output      
        }
        else{
            map = criar_mapa(0, 0, reconstruir_mapa_forma_original_aux(bm, ++index), reconstruir_mapa_forma_original_aux(bm, ++index));
        }
    }
    return map;
}

void traduzir_mensagem(FILE* fpin, FILE* fpout, mapa* mapa_caracteres){
    if(fpin && fpout && mapa_caracteres){

    }
}



//MODO DE LEITURA: QUANTO MAIS A ESQUERDA (BIT MAIS SIGNIFICATIVO) QUANTO MAIS A DIREITA (BIT MENOS SIGNIFICATIVO)
//EXEMPLO: 00101101 = 0*2^7 + 0*2^6 + 1*2^5 + 0*2^4 +  1*2^3 + 1*2^2 + 0*2^1 + 1*2^0
void tradutor_aux(FILE* fpin){
    if(fpin){
        rewind(fpin);
        unsigned long n = 0;
        fread(&n, sizeof(unsigned long), 1, fpin);
        puts("ASCII");
        printf("%lx", n);
        for(char c = fgetc(fpin); c!=EOF ;c = fgetc(fpin)){
            printf("%c", (unsigned char)c);
        }
        rewind(fpin);
        puts("\nBINARIO");
        fread(&n, sizeof(unsigned long), 1, fpin);
        printf("%lx", n);
        unsigned char bit = 0;
        for(int c = fgetc(fpin); c!=EOF ;c = fgetc(fpin)){
            for(unsigned i=0;i<8;i++){
                bit = c&0x80;   //0x80 = 128 :: armazenando o valor contido no bit de menor significância na variável letra
                bit >>= 7;      //como o bit de menor significância está na última casa da célula, é preciso fazer um rshift de 7 casas para converter o valor para unitário (1 ou 0)
                c = c<<1;       //atualiza-se a letra com um lshift para trazer o bit a direita do lido anterior para a posição de leitura da iteração seguinte
                printf("%x", bit);
            }
        }
        rewind(fpin);
    }
}
