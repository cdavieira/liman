#include "cliente.h"
#include "compactador.h"
#include "descompactador.h"
#include "analisar-compactado.h"
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

void compactar(char* nomeArquivoTexto){
    if(nomeArquivoTexto){
        FILE* fpin = fopen(nomeArquivoTexto, "rb"), *fpout = fopen("./respostas/compactado.comp", "wb");
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

//idéia básica: ler um bit e verificar se esse bit consegue me levar ate algum no folha da minha arvore; se conseguir, entao chegamos a uma letra, caso contrario continua leitura;
void descompactar(char* nomeArquivoBinario){
    if(nomeArquivoBinario){
        FILE *fpin = fopen(nomeArquivoBinario, "rb"), *fpout = fopen("./respostas/descompactado.txt", "w");
        assert(fpin, "Erro ao gerar arquivo compactado! Terminando programa...");
        // Debug
        // analisar_armazenamento_arquivo(fpin);
        // imprimir_conteudo_binario(fpin);
        bitmap* bm = remontar_mapa_forma_bitmap(fpin);
        //a função anterior foi pensada para deixar propositalmente o leitor do arquivo fpin posicionada para a leitura do primeiro caracter da descompactação, por isso a lógica seguinte parte desse pressuposto
        mapa* map = reconstruir_mapa_forma_original(bm);
        if(bm) bitmapLibera(bm);    //bitmapLibera não é null segura, portante é necessário a condicional
        traduzir_mensagem(fpin, fpout, map);
        liberar_mapa(map);        
        fclose(fpin);
        fclose(fpout);
    }
}