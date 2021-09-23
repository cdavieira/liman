PROGRAMA1 = Compactador

PROGRAMA2 = Descompactador

DIRETORIOS = source objetos

FILTRO_FONTE = source/bitmaptester.c

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
	rm objetos/*.o $(PROGRAMA1) $(PROGRAMA2) && rmdir objetos

run:
	./$(PROGRAMA1)

valgrind:
	valgrind --leak-check=full -s --track-origins=yes ./$(PROGRAMA1)

echo:
	@echo '$(BIBLIOTECAS)' '$(OBJETOS)' '$(ARQUIVOS_FONTE)'