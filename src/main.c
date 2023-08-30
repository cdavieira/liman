#include "cliente.h" //compactar, descompactar
#include <stdlib.h>
#include <string.h> //strcmp

int main(int argc, char *argv[]){
    if(!strcmp(argv[0], "./huffmann")){
		compactar(argv[1]);
	}
    else if(!strcmp(argv[0], "./unhuffmann")){
		descompactar(argv[1]);
	}
    return 0;
}
