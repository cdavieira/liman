#include "liman.h"
#include <stdio.h>
#include <string.h>

void huffman_usage(void){
	printf("./huffman [-h] <filename>\n");
}

void unhuffman_usage(void){
	printf("./unhuffman [-h] <filename.comp>\n");
}

int main(int argc, char *argv[]){
	if(!strcmp(argv[0], "./huffman")){
		if(argc != 2 || !strcmp(argv[1], "-h")){
			huffman_usage();
			return 0;
		}
		compress(argv[1]);
	}
	else if(!strcmp(argv[0], "./unhuffman")){
		if(argc != 2 || !strcmp(argv[1], "-h")){
			unhuffman_usage();
			return 0;
		}
		decompress(argv[1]);
	}
	else if(!strcmp(argv[0], "./huhman")){
		analyze(argv[1]);
	}
	return 0;
}
