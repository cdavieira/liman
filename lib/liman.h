#pragma once

#include <stddef.h>


/**
 * compacta um arquivo em formato .comp
 *
 * @param[in] filename nome de arquivo texto
 *
 * @return -
 */
void compress(char* filename);

/**
 * analisa arquivos do formato .comp
 *
 * @param[in] filename nome do arquivo compactado
 *
 * @return -
 */
void analyze(char* compfile);

/**
 * descompacta um arquivo compactado pela função 'compactar'
 *
 * @param[in] compfile nome do arquivo binário/compactado
 *
 * @return -
 */
void decompress(char* compfile);
