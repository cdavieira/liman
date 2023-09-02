#include "client.h" //compactar, descompactar
#include <string.h> //strcmp

int main(int argc, char *argv[]){
    if(!strcmp(argv[0], "./huffman")){
		compactar(argv[1]);
	}
    else if(!strcmp(argv[0], "./unhuffman")){
		descompactar(argv[1]);
	}
    return 0;
}
