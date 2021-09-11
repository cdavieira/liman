#include "text-tools.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void gotoLine(FILE* file, size_t l){
    if(file){
        rewind(file);
        size_t curLine = 0;
        for(char c=0;c!=EOF;c=fgetc(file)){
            if(c=='\n') curLine++;
            if(curLine==l) break;
        }
    }
}

long chLineCount(FILE* file, size_t l){
    if(file){
        long initial_pos = ftell(file), n=0;     //armazenando posição inicial do leitor no arquivo
        gotoLine(file, l);
        for(char c=fgetc(file);c!='\n';c=fgetc(file)){
            if(c==EOF) break;
            else n++;
        }
        fseek(file, initial_pos, SEEK_SET);      //recolocando leitor em sua posição inicial;
        return n;
    }
    else return -1;
}

unsigned long chFileCount(FILE* file){
    if(file){
        long initial_pos = ftell(file), n=0;     //armazenando posição inicial do leitor no arquivo
        gotoLine(file, 0);
        for(char c=fgetc(file);c!=EOF;++n, c=fgetc(file)){}
        fseek(file, initial_pos, SEEK_SET);      //recolocando leitor em sua posição inicial;
        return n;
    }
    else return 0;
}

unsigned long LineCount(FILE* file){
    if(file){
        long initial_pos = ftell(file), n=1;     //armazenando posição inicial do leitor no arquivo
        rewind(file);
        for(char c=fgetc(file);c!=EOF;c=fgetc(file))
            if(c=='\n') n++;
        fseek(file, initial_pos, SEEK_SET);      //recolocando leitor em sua posição inicial;
        return n;
    }
    else return 0;
}

char* lineToStr(FILE* file, size_t l){
    if(file){
        long tam = chLineCount(file, l)+1, initial_pos = ftell(file);     //armazenando posição inicial do leitor no arquivo
        char* lineStr = calloc(tam, sizeof(char));
        gotoLine(file, l);
        if(tam>1){
            for(size_t i=0;i<(tam-1);i++)
                *(lineStr+i) = getc(file);
            *(lineStr+tam-1) = '\0';
        }
        fseek(file, initial_pos, SEEK_SET);                               //recolocando leitor em sua posição inicial;
        return lineStr;
    }
    else return 0;
}