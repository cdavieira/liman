#pragma once

#include <sys/types.h>
#include "types/HuffmanTree.h"


typedef struct InputBytes InputBytes;
typedef struct ByteFrequency ByteFrequency;
typedef struct FileHeader FileHeader;
typedef struct FileBody FileBody;

enum HuhmanOpts {
	HUHMAN_HEADER = 1,
	HUHMAN_BODY = 2,
	HUHMAN_CODES = 4,
	HUHMAN_PDF = 8,
};


InputBytes* inputBytes_new(char* f);
InputBytes* inputBytes_destroy(InputBytes* in);


ByteFrequency* byteFreq_new(InputBytes* in);
ByteFrequency* byteFreq_destroy(ByteFrequency* f);
int byteFreq_compare_callback(const void* b1, const void* b2);


HuffmanTree* huffmanAlgorithm(ByteFrequency* f);


/*
 * tree encoding convention:
 * 0 for non-leaf nodes
 * 1 for leaf nodes
 *
 * a tree 't' is printed in the following way:
 * print(t)
 * print_recursive(t->left)
 * print_recursive(t->right)
 *
 * if 't' is a leaf node, then 1 byte gets printed next, which is the ASCII
 * character stored in that leaf node
 *
 * Summary:
 * 1. transverse the tree recursively
 * 2. if a leaf node is encountered, print bit '1' followed by 8 bits. Otherwise print bit '0'.
 * */
FileHeader* limanHeader_new(HuffmanTree* root);
/**
 * Read and parse the huffman tree stored in the compressed file
 *
 * The file cursor is advanced (on purpose) until 1 byte before the beginning
 * of the encoded message. This 1 byte tells how much padding was added in the
 * end of the encoded message to ensure byte alignment.
 *
 * For the compressed file, the bits are stored like this:
 *     00101101 = 0*2^7 + 0*2^6 + 1*2^5 + 0*2^4 +  1*2^3 + 1*2^2 + 0*2^1 + 1*2^0
 * 
 * It's worth mentioning that the huffmanTree was stored with proper byte
 * alignment.
 * For that reason, this function uses 'fgetc' to read each byte of the tree
 * sequentially.
 */
FileHeader* limanHeader_read(FILE* fp);
FileHeader* limanHeader_destroy(FileHeader* hdr);


FileBody* limanBody_new(HuffmanTree* root, InputBytes* in);
FileBody* limanBody_read(HuffmanTree* root, FILE* fp);
FileBody* limanBody_destroy(FileBody* body);


/**
 * Dump the contents of the header and the body to filename
 * */
int liman_dump(FileHeader* header, FileBody* body, const char* filename);

/**
 * Traduz o conteudo de um arquivo compactado segundo a árvore de codificação
 * fornecida
 *
 * Essa função parte do pressuposto que o leitor do fd FILE* está posicionado
 * 1 byte antes do início da mensagem. Esse é um efeito intencional
 * da função 'limanHeader_read'. Esse 1 byte que antecede a mensagem diz
 * quantos bits de padding foram adicionados ao final do arquivo para garantir
 * o byte alignment do arquivo.
 *
 * Funcionamento:
 * 1. ler um bit
 * 2. verificar se esse bit leva até algum no folha da arvore/HuffmanTree
 * 3. se sim, então chegou-se a uma letra. Caso contrário, continua leitura
 *
 * @return the number of bits read (that is, the size of the msg in bits)
 */
int liman_decode(HuffmanTree* root, FileBody* body, FILE* fp);

/* (similar to 'liman_decode') decodes the compressed message, but it does so
 * by reading the compressed file and decompressing it at the same time */
int liman_decode_single_pass(HuffmanTree* root, FILE* fpin, FILE* fpout);


void compress(char* filename);
void analyze(char* compfile, int opts);
void decompress(char* compfile);
