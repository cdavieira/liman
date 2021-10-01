#include "cliente.h"
#include "compactador.h"
#include "descompactador.h"
#include "mapa.h"
#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

static void assert(void* p, char* msg);

static void assert(void* p, char* msg){
    if(!p){
        puts(msg);
        exit(1);
    }
}

void compactar(char* nomeArquivoTexto, char* nomeArquivoBinario){
    if(nomeArquivoTexto && nomeArquivoBinario){
        FILE* fpin = fopen(nomeArquivoTexto, "rb"), *fpout = fopen(nomeArquivoBinario, "wb");
        assert(fpin, "Erro ao abrir arquivo para leitura! Terminando programa...");
        assert(fpout, "Erro ao gerar arquivo compactado! Terminando programa...");
        mapa* mapa_caracteres = montar_mapa(fpin);
        exportar_mapa_formato_bitmap(mapa_caracteres, fpout); //escrevendo mapa de caracteres no começo do arquivo
        exportar_texto_formato_bitmap(mapa_caracteres, fpin, fpout); //iniciando gravacao do conteudo do arquivoTexto no arquivo binario
        liberar_mapa(mapa_caracteres);
        fclose(fpin);
        fclose(fpout);
    }
}

//ler um bit e verificar se esse bit consegue me levar ate algum no folha da minha arvore;se conseguir, entao chegamos a uma letra, caso contrario continua leitura;
void descompactar(char* nomeArquivoBinario, char* nomeArquivoSaida){
    if(nomeArquivoBinario && nomeArquivoSaida){
        FILE *fpin = fopen(nomeArquivoBinario, "rb"), *fpout = fopen(nomeArquivoSaida, "w");
        assert(fpin, "Erro ao gerar arquivo compactado! Terminando programa...");
        
        // Debug
        analisar_armazenamento_arquivo(fpin);
        imprimir_conteudo_binario(fpin);
        
        bitmap* bm = remontar_mapa_forma_bitmap(fpin);
        //a função anterior foi pensada para deixar propositalmente o leitor do arquivo fpin posicionada para a leitura do primeiro caracter da descompactação, por isso a lógica seguinte parte desse pressuposto
        mapa* map = reconstruir_mapa_forma_original(bm); //nao utiliza fpin ainda
        if(bm) bitmapLibera(bm);    //bitmapLibera não é null segura
        traduzir_mensagem(fpin, fpout, map);
        liberar_mapa(map);        
        fclose(fpin);
        fclose(fpout);
    }
}