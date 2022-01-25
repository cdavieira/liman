#ifndef _cliente
#define _cliente

/*  compactar
    Descrição: compacta um arquivo texto em formato .comp
    
    Input: nome de arquivo texto
    
    Output: -
    
    Pré condição: arquivo texto fornecido deve existir
    
    Pós condicao: -
*/
void compactar(char* nomeArquivoTexto);

/*  descompactar
    Descrição: descompacta um arquivo compactado pelo programa Compactador
    
    Input: nome de arquivo binário/compactado pelo programa Compactador
    
    Output: -
    
    Pré condição: arquivo compactado deve existir e ter sido gerado pela função compactar
    
    Pós condicao: -
*/
void descompactar(char* nomeArquivoBinario);

#endif