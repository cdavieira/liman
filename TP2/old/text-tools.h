#ifndef _text_tools_lib
#define _text_tools_lib 1

#ifndef _STDIO_H
#include <stdio.h>
#endif

/* 
    Coloca o leitor do arquivo no começo do indice de linha fornecido.
*/
void gotoLine(FILE*, size_t);

/*
    Descrição
    Conta o número de caracteres no indice de linha fornecido.
    Não inclui na contagem o caracter de quebra de linha.

    Retornos
    Sucesso: o número de caracteres presentes na linha exceto a quebra de linha final, se existente.
    Falha: -1 em caso de falha na abertura do arquivo
*/
long chLineCount(FILE*, size_t);

/*
    Descrição
    Conta o número de caracteres no arquivo fornecido.

    Retornos
    Sucesso: o número de caracteres presentes no arquivo.
    Falha: 0 em caso de falha na abertura do arquivo
*/
unsigned long chFileCount(FILE*);

/*
    Descrição
    Conta o número de linhas no arquivo fornecido.

    Retornos
    Sucesso: o número de linhas presentes no arquivo.
    Falha: 0 em caso de falha na abertura do arquivo
*/
unsigned long LineCount(FILE*);

/* 
    Descrição
    Retorna uma string com todos os caracteres presentes no indice de linha fornecido.
    Não inclui a quebra de linha.

    Retornos
    Sucesso: uma string alocada dinamicamente com todos os caracteres, com exceção da quebra de linha, do indice de linha fornecido.
    Falha: ponteiro nulo em caso de falha na abertura do arquivo
*/
char* lineToStr(FILE*, size_t);


#endif