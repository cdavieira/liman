#pragma once

typedef struct caracter caracter;

typedef struct map bitmap;

/*  pegar_ASCII_caracter
    Descrição: retorna o codigo ASCII do caracter
    
    Input: caracter
    
    Output: codigo ASCII do caracter (unsigned int)
    
    Pré condição: caracter deve ter sido alocado utilizando a função de criar_caracter
    
    Pós condicao: -
*/
unsigned pegar_ASCII_caracter(caracter* c);

/*  pegar_peso_caracter
    Descrição: retorna o peso/a frequência do caracter
    
    Input: caracter
    
    Output: peso/frequência do caracter
    
    Pré condição: caracter deve ter sido alocado utilizando a função de criar_caracter
    
    Pós condicao: -
*/
unsigned long pegar_peso_caracter(caracter* c);

/*  pegar_bmap_caracter
    Descrição: retorna o bitmap do caracter
    
    Input: caracter
    
    Output: ponteiro para bitmap do caracter
    
    Pré condição: caracter deve ter sido atualizado pela chamada da funcao atualizar_bmap_caracter
    
    Pós condicao: -
*/
bitmap* pegar_bmap_caracter(caracter* c);

/*  criar_caracter
    Descrição: cria um novo caracter  e preenche seus campos com os argumentos fornecidos
    
    Input: codigo ASCII do char em questao e peso/frequência desse
    
    Output: ponteiro dinamicamente alocado para caracter preenchido
    
    Pré condição: -
    
    Pós condicao: -
*/
caracter* criar_caracter(unsigned letra, unsigned long peso);

/*  alocar_caracter
    Descrição: aloca dinamicamente uma variavel do tipo caracter
    
    Input: -
    
    Output: ponteiro para caracter
    
    Pré condição: -
    
    Pós condicao: ponteiro dinamicamente alocado
*/
caracter* alocar_caracter();

/*  desalocar_caracter
    Descrição: libera um ponteiro para caracter
    
    Input: ponteiro para caracter
    
    Output: nulo
    
    Pré condição: ponteiro deve ter sido dinamicamente alocado
    
    Pós condicao: caso receba o retorno dessa função, ponteiro é setado para nulo (proteção a double-free)
*/
caracter* desalocar_caracter(caracter* c);

/*  atualizar_ASCII_caracter
    Descrição: atualiza o codigo ASCII do caracter
    
    Input: caracter a ser modificado e novo codigo ASCII
    
    Output: caracter fornecido modificado
    
    Pré condição: ponteiros fornecidos devem ser nao nulos
    
    Pós condicao: -
*/
caracter* atualizar_ASCII_caracter(caracter* c, unsigned letra);

/*  atualizar_peso_caracter
    Descrição: atualiza o peso do caracter
    
    Input: caracter a ser modificado e peso atualizado
    
    Output: caracter fornecido modificado
    
    Pré condição: ponteiros fornecidos devem ser nao nulos
    
    Pós condicao: -
*/
caracter* atualizar_peso_caracter(caracter* c, unsigned long peso);

/*  atualizar_bmap_caracter
    Descrição: atualiza o bitmap do caracter
    
    Input: caracter a ser modificado e novo bitmap
    
    Output: caracter fornecido modificado
    
    Pré condição: ponteiros fornecidos devem ser nao nulos
    
    Pós condicao: -
*/
caracter* atualizar_bmap_caracter(caracter* c, bitmap* bm);

/*  exibir_ASCII_caracter
    Descrição: exibe o codigo ASCII armazenado no carater
    
    Input: caracter
    
    Output: -
    
    Pré condição: argumento fornecido nao nulo
    
    Pós condicao: -
*/
void exibir_ASCII_caracter(caracter* c);

/*  exibir_peso_caracter
    Descrição: exibe o peso armazenado no carater
    
    Input: caracter
    
    Output: -
    
    Pré condição: argumento fornecido nao nulo
    
    Pós condicao: -
*/
void exibir_peso_caracter(caracter* c);

/*  exibir_caracter
    Descrição: exibe todas informações armazenadas no carater
    
    Input: caracter
    
    Output: -
    
    Pré condição: argumento fornecido nao nulo
    
    Pós condicao: -
*/
void exibir_caracter(caracter* c);

/*  comparar_ASCII_caracter
    Descrição: checa se o caracter apresenta o mesmo codigo ASCII da letra fornecida
    
    Input: ponteiros caracter e codigo ASCII da letra
    
    Output: 1 caso o caracter apresente código ASCII identico ao da letra fornecida, 0 caso contrario
    
    Pré condição: argumentos fornecidos não nulos
    
    Pós condicao: -
*/
unsigned comparar_ASCII_caracter(caracter* c, unsigned* letra);

/*  pegar_endereco_ASCII_caracter
    Descrição: retorna o endereço da váriavel que armazena o codigo ASCII de um caracter
    
    Input: caracter
    
    Output: ponteiro para unsigned que contem o codigo ASCII do caracter
    
    Pré condição: argumentos fornecidos nao nulos
    
    Pós condicao: -
*/
unsigned* pegar_endereco_ASCII_caracter(caracter* c);

/*  comparar_peso_caracter
    Descrição: checa se o primeiro caracter entrado apresenta peso superior ou igual ao segundo caracter fornecido
    
    Input: dois ponteiros para variáveis caracter
    
    Output: 1 caso o primeiro caracter seja tanto quanto ou mais frequente que o segundo, 0 caso contrario ou algum dos argumentos seja nulo
    
    Pré condição: -
    
    Pós condicao: -
*/
unsigned comparar_peso_caracter(caracter* c1, caracter* c2);
