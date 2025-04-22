#include "liman.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>

void huffman(int argc, char* argv[]);
void unhuffman(int argc, char* argv[]);
void huhman(int argc, char* argv[]);

int main(int argc, char *argv[]){
	if(!strcmp(argv[0], "./huffman")){
		huffman(argc, argv);
	}
	else if(!strcmp(argv[0], "./unhuffman")){
		unhuffman(argc, argv);
	}
	else if(!strcmp(argv[0], "./huhman")){
		huhman(argc, argv);
	}
	return 0;
}

void huffman(int argc, char* argv[]){
	if(argc != 2){
		printf("%s filename\n", argv[0]);
		return ;
	}
	compress(argv[1]);
}

void unhuffman(int argc, char* argv[]){
	if(argc != 2){
		printf("%s filename.comp\n", argv[0]);
		return ;
	}
	decompress(argv[1]);

}

void huhman(int argc, char* argv[]){
	if(argc < 3){
		printf("%s filename.comp [-h|-b|-c|-p]\n"
			"-h: print information about the 'header' of a .comp file\n"
			"-b: print information about the 'body' of a .comp file\n"
			"-c: create a 'huhman-*.txt' file, with all huffman codes for each encoded ASCII letter\n"
			"-p: create a 'huhman-*.dot' file, which can be parsed by tools to create a png/pdf/jpeg of the generated huffman tree\n"
			, argv[0]);
		return ;
	}

	extern int opterr;
	opterr = 0;
	extern int optind;
	extern char* optarg;

	const char* optstring = "hbtcp";
	int myopts = 0;
	int ch;
	while((ch = getopt(argc, argv, optstring)) != -1){
		switch(ch){
			case 'h':
				myopts |= HUHMAN_HEADER;
				break ;
			case 'b':
				myopts |= HUHMAN_BODY;
				break ;
			case 'c':
				myopts |= HUHMAN_CODES;
				break ;
			case 'p':
				myopts |= HUHMAN_PDF;
				break ;
			case '?':
			default:
				printf("huhman: unknown option %c\n", ch);
				break;
		}
	}

	analyze(argv[argc-1], myopts);
}
