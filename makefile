# INSTRUÇÕES BÁSICAS DE USO AO FINAL DO MAKEFILE

PROGRAMA1 = Compactador

PARAMETROSPROGRAMA1 = ./respostas/teste.txt

PROGRAMA2 = Descompactador

PARAMETROSPROGRAMA2 = $(addsuffix .comp,$(PARAMETROSPROGRAMA1))

DIRETORIOS = source objetos

FILTRO_FONTE = 

FILTRO_TEXTO = respostas/teste.txt

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
	rm objetos/*.o $(PROGRAMA1) $(PROGRAMA2) && rmdir objetos && rm $(filter-out $(FILTRO_TEXTO),$(wildcard respostas/*.txt respostas/*.comp))

run:
	./$(PROGRAMA1) $(PARAMETROSPROGRAMA1)
	./$(PROGRAMA2) $(PARAMETROSPROGRAMA2)

compactar:
	./$(PROGRAMA1) $(PARAMETROSPROGRAMA1)

descompactar:
	./$(PROGRAMA2) $(PARAMETROSPROGRAMA2)

valgrind1:
	valgrind --leak-check=full -s --track-origins=yes ./$(PROGRAMA1) $(PARAMETROSPROGRAMA1)

valgrind2:
	valgrind --leak-check=full -s --track-origins=yes ./$(PROGRAMA2) $(PARAMETROSPROGRAMA2)

echo:
	@echo '$(BIBLIOTECAS)' '$(OBJETOS)' '$(ARQUIVOS_FONTE)' '$(filter-out $(FILTRO_TEXTO),$(wildcard respostas/*.txt respostas/*.bin))'


# Para recriar os arquivos objetos e os executaveis criados: make all

# Para excluir todos os arquivos criados por esse makefile: make clean

# Para compactar e descompactar os arquivos existentes na pasta respostas: make run

# Para compactar o arquivo .txt contido na pasta respostas: make compactar

# Para descompactar o arquivo .comp contido na pasta respostas: make descompactar

# Para rodar o valgrind do arquivo Compactador: make valgrind1

# Para rodar o valgrind do arquivo Descompactador: make valgrind2
