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

## Sobre o algoritmo de Huffmann

O algoritmo de Huffmann funciona 'recodificando' códigos comumente usados para
codificar caracteres.

Por exemplo, na representação ASCII extendida, a codificação dos caracteres
é feita com **8 bits**. Então por exemplo, se um arquivo de texto `senha.txt`
contém 4 caracteres, ao todo seu tamanho é 32 bits.

É justamente na quantidade de bits necessários para armazenar 1 caracter que o
algoritmo de Huffmann atua, isto é, ele permite modificar o número de bits
necessários para armazenar informação.

Desse modo, em vez de **8 bits** por caracter, por vezes o algoritmo de
Huffmann permite que sejam utilizados apenas **4 bits** ou **5 bits** ou **6
bits** para armazenar 1 caracter, o que reduz o tamanho total de um arquivo
composto por esses caracteres.

É importante, no entanto, levantar algumas observações sobre a compactação
desse tipo de algoritmo. Pela forma como funciona, essa se torna mais
proveitosa para textos/arquivos que utilizam caracteres que se repetem com
muita frequência e que utilizam um número restrito de letras de um alfabeto.

Por exemplo, textos escritos em ingles/portugues em geral utilizam apenas 26
(letras) + 10 (números) caracteres, totalizando 36 caracteres. Nesse sentido,
apenas 36 dos 256 caracteres previstos pela representação ASCII extendida são
em geral usados.  Textos como esses, que utilizam um grupo restrito do conjunto
total de caracteres de uma representação, em geral são melhor "comprimidos"
pelo algoritmo de Huffmann, com a exceção de alguns casos.

Quanto aos arquivos `.comp` gerados pelo algoritmo, esses são arquivos *binários*,
sendo que o formato com que são escritos é o seguinte:

```
<tabela de huffmann na forma de árvore escrita em bits corridos><mensagem codificada segundo
tabela de huffmann>
```

A descompressão dos arquivos `.comp` basicamente segue os 2 passos:
1. remontar a *tabela de huffmann*
    * *tabela de huffmann*: árvore que armazena a correspondência de 1 código de huffmann a 1 caracter da tabela ASCII estendido
2. ler a mensagem em binário e trocar o *código de huffmann* pelo seu correspondente ASCII estendido

*A compactação via algoritmo de Huffmann, ainda que sem perda, não é tão
eficiente quanto outras existentes.*

> tl;dr: em geral, quanto mais vezes alguns dos caracteres de uma codificação
> de caracteres se repetem em um texto, mais eficiente se torna o algoritmo de
> Huffmann.

## Desempenho de compactação

O projeto contem alguns arquivos na pasta `input`, que foram usados para testar o desempenho da compactação para diferentes tipos de arquivo.

  input     | extensao | tamanho original  |  tamanho compactado  | taxa de compressão  |
:------:    | :------: |:----------------: | :------------------: | :----------------:  | 
bible.txt   |   txt    |  4_451_368 bytes  |    2_577_870 bytes   |       +42.78%       |
teste.txt   |   txt    |  97_042 bytes     |    79_227 bytes      |       +18.36%       |
pikachu.gif |   gif    |  4_459_259 bytes  |    4_449_596 bytes   |       +00.22%       |
gatinhu.png |   png    |  160_039 bytes    |    160_361 bytes     |       -00.02%       |
jpg.jpg     |   jpg    |  35_989 bytes     |    36_032 bytes      |       -00.12%       |

## Discussões

É possível perceber que arquivos `.txt` *em geral* se encaixam na regra de que
> quanto mais vezes alguns dos caracteres de uma codificação de caracteres se
> repetem em um texto, mais eficiente se torna o algoritmo de Huffmann.

É notório que alguns arquivos de extensões como `.png`, `.jpg`, `.gif` não
apresentam taxas de compressão consideráveis e, algumas vezes, na verdade
sofrem um efeito contrário àquele esperado de uma compressão de arquivos.

Isso ocorre porque esses textos utilizam praticamente todos os caracteres da tabela ASCII estendido na codificação da informação que ali vai inserida. Para esses arquivos, outros tipos de compactadores, que utilizam estratégias diferentes, são necessários para armazenar a informação de forma eficiente e sem perdas.
