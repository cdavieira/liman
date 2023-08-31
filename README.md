# winrar

Programa capaz de reduzir o tamanho total em bytes de arquivos (ex: .txt, .png , .jpg, .gif e outros), aplicando compressão sem perda de dados baseada no algoritmo de Huffmann.

## 

## Como rodar

* clonar esse repositorio
```bash
    git clone https://github.com/paisdegales/winrar.git
```

* ir até a pasta onde o projeto foi salvo
```bash
    cd path/to/project
```

* gerar os executaveis
```bash
    make
```

* limpar todos os objetos/compactados/executaveis
```bash
	make clean
```

* compactar um arquivo contido na pasta 'input'
```bash
	make compress input="nome_do_arquivo.txt"
```

* descompactar um arquivo contido na pasta 'output'
```bash
	make decompress input="nome_do_arquivo.txt"
```

## Pastas

```
.
├── input
├── lib
│   ├── data-structures
│   └── utils
├── misc
└── src
    ├── data-structures
    └── utils
```

## Testes

> COLOCAR TESTES, INFORMANDO TAMANHO INICIAL DO ARQUIVO E TAMANHO POS COMPACTACAO

>> TALVEZ ATE INFORMAR QUANTOS POR CENTO A COMPACTACAO TEM DO MAPA E DA MENSAGEM CODIFICADA

# todo
* escrever todas as funções que são utilizadas por cada library
* colocar qualifiers const onde der (getters principalmente)
* criar arquivo utils que reuna tudo que foi usado de aleatorio ao longo do programa (funcoes)
* remover funcoes nao utilizados 
* remover ambiguidades no codigo quando possivel
* melhorar instruções do makefile
* utilizar ferramentas de analise do codigo (gcov, cflow, complexity, profilling)
* melhorar README
> fazendo
* refazer relatorio/deleta-lo daqui
> mais informações sobre o funcionamento do programa no pdf (que será atualizado ainda, decorrente de algumas recém-mudanças feitas no funcionamento do código).

