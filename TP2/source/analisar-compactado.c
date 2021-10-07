#include "analisar-compactado.h"

unsigned long contar_bits_mapa(FILE* fpin){
    unsigned long tam = 0;
    if(fpin){
        unsigned char c = fgetc(fpin);
        if(((c&0x80)>>7)?0:1){
            rewind(fpin);
            unsigned long bit = 0, soma = 1, dump = 0;
            for(c=fgetc(fpin);c!=EOF;c=fgetc(fpin)){
                for(unsigned i=0;i<8;i++){
                    bit = ((c&0x80)>>7);
                    c<<=1;
                    if(!dump){
                        dump = bit?8:0;
                        soma += bit?-1:1;
                    }
                    else dump--;
                    ++tam;
                    if(!soma) break;
                }
                if(!soma) break;
            }
            tam += 8; //a letra que segue o ultimo no folha do mapa acaba nao sendo lida pelo algoritmo, por isso é necessário adicionar 8 bits manualmente
        }
        rewind(fpin);
    }
    return tam;
}

void analisar_armazenamento_arquivo(FILE* fpin){
    if(fpin){
        rewind(fpin);
        unsigned long tamanho_bits_mapa = contar_bits_mapa(fpin), tamanho_bits_mensagem = 0, total=0;
        unsigned bits_vazios = tamanho_bits_mapa%8;
        for(char c=fgetc(fpin);!feof(fpin);c=fgetc(fpin)){
            tamanho_bits_mensagem+=8;
        }
        tamanho_bits_mensagem-=tamanho_bits_mapa;
        tamanho_bits_mensagem-=bits_vazios;
        total = (tamanho_bits_mapa+tamanho_bits_mensagem+bits_vazios);
        printf("TAMANHO TOTAL DO ARQUIVO: %lu BYTES\n", total/8);
        printf("PORCENTAGEM OCUPADA PELO MAPA: %.2f%%\n", 100*((double)tamanho_bits_mapa)/total);
        printf("PORCENTAGEM OCUPADA PELA MENSAGEM: %.2f%%\n", 100*((double)tamanho_bits_mensagem)/total);
        printf("PORCENTAGEM OCUPADA PELOS BITS VAZIOS: %.2f%%\n", 100*((double)bits_vazios)/total);
        //na verdade tem alguns bit vazios que não foram contabilizados (os que aparecem depois da escrita da mensagem)
        rewind(fpin);
    }
}

void imprimir_conteudo_binario(FILE* fpin){
    if(fpin){
        rewind(fpin);
        unsigned long tamanho_bits_mapa = contar_bits_mapa(fpin), tamanho_bytes_mapa = tamanho_bits_mapa/8;
        rewind(fpin);
        puts("Mapa:");
        for(unsigned long i=0;i<tamanho_bytes_mapa;i++){
            printf("%c", fgetc(fpin));
        }
        puts("\nMensagem:");
        for(char c=fgetc(fpin);!feof(fpin);c=fgetc(fpin)){
            printf("%c", c);
        }
        putchar('\n');
        rewind(fpin);
    }
}