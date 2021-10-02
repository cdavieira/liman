PROGRAMA1 = Compactador

PARAMETROSPROGRAMA1 = "./respostas/teste.txt" "./respostas/compactado.bin"

PROGRAMA2 = Descompactador

PARAMETROSPROGRAMA2 = "./respostas/compactado.bin" "./respostas/descompactado.txt"

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
	rm objetos/*.o $(PROGRAMA1) $(PROGRAMA2) && rmdir objetos && rm $(filter-out $(FILTRO_TEXTO),$(wildcard respostas/*.txt respostas/*.bin))

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