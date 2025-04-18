#pragma once

#include "core/HuffmanTree.h"
#include <stdio.h>

/**
 * Count how many bits were used to encode the huffmanTree in the compressed file
 */
size_t count_tree_bits(FILE* fpin);

/**
 * Read and parse the huffman tree stored in the compressed file
 *
 * NOTE: this is a helper/wrapper function for 'read_bitmap' followed by
 * 'bitmap2huffmanTree'
 */
HuffmanTree* read_huffmanTree(FILE* fpin);

/**
 * Read and parse the huffman tree stored in the compressed file and convert
 * it to a Bitmap
 *
 * The file cursor is advanced (on purpose) until the beginning of the encoded
 * message.
 *
 * For the compressed file, the bits are stored like this:
 *     00101101 = 0*2^7 + 0*2^6 + 1*2^5 + 0*2^4 +  1*2^3 + 1*2^2 + 0*2^1 + 1*2^0
 * 
 * It's worth mentioning that the huffmanTree was stored with propoer byte alignment.
 * For that reason, this function uses 'fgetc' to read each byte of the tree
 * sequentially.
 */
Bitmap* read_bitmap(FILE* fpin);

/**
 * Read and parse the huffman tree stored in the compressed file and convert
 * it to a Bitmap
 */
HuffmanTree* bitmap2huffmanTree(Bitmap* bm, unsigned *index);

/**
 * Traduz o conteudo de um arquivo compactado segundo a árvore de codificação
 * fornecida
 *
 * Essa função parte do pressuposto que o leitor do fd FILE* está posicioado
 * no início da mensagem. Esse é um efeito colateral intencional da função
 * 'read_bitmap'
 *
 * Existe a possibilidade dessa função falhar em um cenário que não foi
 * possível reproduzir nos testes, por ser algo raro de ocorrer. Caso o último
 * byte do arquivo compactado tenha mais que um '\0' codificado, então a
 * descompactação teoricamente falhará e o arquivo final estará incompleto.
 * É um bug corrigível. Leia os comentarios em 'decompress.c' para mais
 * informações.
 *
 * Funcionamento:
 * 1. ler um bit
 * 2. verificar se esse bit leva até algum no folha da arvore/HuffmanTree
 * 3. se sim, então chegou-se a uma letra. Caso contrário, continua leitura
 *
 * @return the number of bits read (that is, the size of the msg in bits)
 */
size_t read_msg(FILE* fpin, FILE* fpout, HuffmanTree* mapa_caracteres);
