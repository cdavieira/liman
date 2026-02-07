#pragma once

#include "CompHeader.h"
#include "utils/types/common.h"

typedef struct CompReader CompReader;
typedef struct CompReaderOutput {
  size_t sizeBytes;
  size_t sizeBits;
  size_t padBits;
} CompReaderOutput;

CompReader *compReader_new(const char *filename);
CompReader *compReader_destroy(CompReader *reader);

CompHeader *compReader_get_header(CompReader *reader);

/**
 * Traduz o conteudo de um arquivo compactado segundo a árvore de codificação
 * fornecida
 *
 * Funcionamento:
 * 1. ler um bit
 * 2. verificar se esse bit leva até algum no folha da arvore/HuffmanTree
 * 3. se sim, então chegou-se a uma letra. Caso contrário, continua leitura
 */
CompReaderOutput compReader_translate(CompReader *reader, const char *filename);
