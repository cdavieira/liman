# INSTRUÇÕES BÁSICAS DE USO AO FINAL DO MAKEFILE

PROGRAMA1 = Compactador

PARAMETROSPROGRAMA1 = ./entrada/PERFEITA.png

PROGRAMA2 = Descompactador

PARAMETROSPROGRAMA2 = $(subst entrada,compactados,$(addsuffix .comp,$(PARAMETROSPROGRAMA1)))

DIRETORIOS = source objetos entrada saida compactados

FILTRO_FONTE = 

FILTRO_TEXTO = entrada/teste.txt

ARQUIVOS_FONTE = $(filter-out $(FILTRO_FONTE),$(wildcard source/*.c))

BIBLIOTECAS = $(wildcard source/*.h)

OBJETOS = $(subst source,objetos,$(ARQUIVOS_FONTE:.c=.o))

FLAGS = -lm -g

CC = gcc

all: criarDiretorios $(PROGRAMA1) $(PROGRAMA2)

criarDiretorios:
	mkdir -p $(DIRETORIOS)

$(PROGRAMA1): $(OBJETOS)
	$(CC) $^ -o $@ $(FLAGS)

$(PROGRAMA2): $(OBJETOS)
	$(CC) $^ -o $@ $(FLAGS)

objetos/%.o: source/%.c $(BIBLIOTECAS)
	$(CC) -c $< -o $@ $(FLAGS)

clean:
	rm objetos/*.o $(PROGRAMA1) $(PROGRAMA2) && rmdir objetos && rm compactados/*.comp && rmdir compactados && rm saida/*.* && rmdir saida

run:
	./$(PROGRAMA1) $(PARAMETROSPROGRAMA1)
	mv entrada/*.comp compactados
	./$(PROGRAMA2) $(PARAMETROSPROGRAMA2)
	mv $(subst entrada,compactados,$(PARAMETROSPROGRAMA1)) saida

compactar:
	./$(PROGRAMA1) $(PARAMETROSPROGRAMA1)
	mv entrada/*.comp compactados

descompactar:
	./$(PROGRAMA2) $(PARAMETROSPROGRAMA2)
	mv $(subst entrada,compactados,$(PARAMETROSPROGRAMA1)) saida

valgrind1:
	valgrind --leak-check=full -s --track-origins=yes ./$(PROGRAMA1) $(PARAMETROSPROGRAMA1)

valgrind2:
	valgrind --leak-check=full -s --track-origins=yes ./$(PROGRAMA2) $(PARAMETROSPROGRAMA2)

echo:
	@echo '$(BIBLIOTECAS)' '$(OBJETOS)' '$(ARQUIVOS_FONTE)' '$(filter-out $(FILTRO_TEXTO),$(wildcard respostas/*.txt respostas/*.bin))'
	@echo '$(PARAMETROSPROGRAMA1)' '$(PARAMETROSPROGRAMA2)'
# Modifique a variavel PARAMETROSPROGRAMA1 com o path para o arquivo que você gostaria de compactar/descompactar depois

# Para recriar os arquivos objetos e os executaveis criados: make all

# Para excluir todos os arquivos criados por esse makefile: make clean

# Para compactar e descompactar os arquivos existentes na pasta entrada e compactados (respectivamente) : make run

# Para compactar o arquivo contido na pasta entrada: make compactar

# Para descompactar o arquivo .comp contido na pasta compactados: make descompactar

# Para rodar o valgrind do arquivo Compactador: make valgrind1

# Para rodar o valgrind do arquivo Descompactador: make valgrind2
