# liman

O **liman** (*linux + huffman*) é um programa capaz de reduzir o
tamanho total em bytes de arquivos (ex: `.txt`, `.png` , `.jpg`, `.gif`
e outros), aplicando compressão sem perda de dados baseada no *algoritmo de Huffman*.

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

* compactar um arquivo contido na pasta `input`
```bash
    make compress input="nome_do_arquivo.txt"
```

* descompactar um arquivo contido na pasta `output`
```bash
    make decompress input="nome_do_arquivo.txt"
```

* limpar todos os objetos/compactados/executaveis
```bash
    make clean
```

## Pastas

```
.
├── input
├── lib
│   ├── data-structures
│   └── utils
└── src
    ├── data-structures
    └── utils
```

## Do *algoritmo de Huffman*

O **algoritmo de Huffman** funciona recodificando códigos comumente usados para
codificar caracteres.

A exemplo da representação ASCII estendida, essa codifica 256 caracteres usando
**8 bits** por caracter. Portanto, se um arquivo de texto chamado `senha.txt`
contém 4 caracteres, ao todo seu tamanho é 32 bits.

O **algoritmo de Huffman** atua na **redução do número de bits por caracter**,
de forma a criar códigos que utilizam menos bits do que o usual para codificar
caracteres.

Assim, em vez dos usuais **8 bits** por caracter, por vezes o **algoritmo de
Huffmann** permite que sejam utilizados apenas **4 bits**, **5 bits** ou **6
bits** para armazenar 1 caracter, o que reduz consideravelmente o tamanho total
de alguns tipos de arquivo.

## Dos arquivos `.comp` gerados

Quanto aos arquivos `.comp` gerados pelo programa, esses são arquivos
*binários*, sendo que o formato com que são escritos é o seguinte:

`<tabela de huffman em formato de árvore> <mensagem
codificada segundo tabela de huffman>`

A descompressão dos arquivos `.comp` basicamente segue 2 passos:
1. remontar a *tabela de huffman*
2. ler a mensagem em binário e trocar o *código de huffman* pelo seu
   correspondente ASCII estendido

> *tabela de huffman*: um estrutura em arvore que dispõe uma correspondência
> 1:1 entre os códigos de huffman e os codigos da tabela ASCII

> *A compactação via *algoritmo de Huffman*, ainda que sem perda, não é tão
eficiente quanto outras existentes.*

> tl;dr: em geral, quanto mais vezes alguns dos caracteres de uma codificação
> de caracteres se repetem em um texto, mais eficiente se torna o algoritmo de
> Huffmann.

## Desempenho de compactação

O projeto contem alguns arquivos na pasta `input`, que foram usados para testar
o desempenho da compactação para diferentes tipos de arquivo.

  input     | extensao | tamanho original  |  tamanho compactado  | taxa de compressão  |
:------:    | :------: |:----------------: | :------------------: | :----------------:  | 
bible.txt   |   txt    |  4_451_368 bytes  |    2_577_870 bytes   |       +42.78%       |
teste.txt   |   txt    |  97_042 bytes     |    79_227 bytes      |       +18.36%       |
pikachu.gif |   gif    |  4_459_259 bytes  |    4_449_596 bytes   |       +00.22%       |
gatinhu.png |   png    |  160_039 bytes    |    160_361 bytes     |       -00.02%       |
jpg.jpg     |   jpg    |  35_989 bytes     |    36_032 bytes      |       -00.12%       |

## Discussões

A estratégia de compactação proposta pelo algoritmo de Hufmann nem sempre é
eficiente.  Pela forma como funciona, essa se torna mais proveitosa para
textos/arquivos que utilizam caracteres que se repetem com muita frequência e
que utilizam um número restrito de letras de um alfabeto.

A exemplo dos textos escritos em ingles/portugues, esses em geral utilizam
apenas 26 (letras) + 10 (números) caracteres, totalizando 36 caracteres. Nesse
sentido, apenas 36 dos 256 caracteres previstos pela representação ASCII
estendida são em geral usados, sendo esse o cenário predileto para a aplicação
do *algoritmo de Huffman* (salvo algumas exceções). Essa percepção se confirma
pelas boas taxas de compactação dos arquivos `.txt` testados.

É notório, ainda, que alguns arquivos de extensões como `.png`, `.jpg`, `.gif`
não apresentam taxas de compressão consideráveis e, algumas vezes, na verdade
sofrem um efeito contrário àquele esperado de uma compressão de arquivos.

Isso ocorre porque esses arquivos utilizam praticamente todos os caracteres da
tabela ASCII estendida na codificação da informação ali presente. Para
esses arquivos, outros tipos de compactadores são necessários para armazenar a
informação de forma eficiente e sem perdas.
