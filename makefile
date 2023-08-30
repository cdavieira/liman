export
src_folder := src
lib_folder := lib
obj_folder := obj
src_files := $(wildcard $(src_folder)/*.c)
lib_files := $(wildcard $(lib_folder)/*.h)
obj_files := $(subst $(src_folder),$(obj_folder),$(src_files:.c=.o))

compress_binary_name := huffmann
compress_input_folder := input
compress_output_folder := compacted
compress_filextension := .comp

decompress_binary_name := unhuffmann
decompress_input_folder := $(compress_output_folder)
decompress_output_folder := output

default_input_file := jpg.jpg

CFLAGS := -lm -g
CC := gcc

all: ;

include compress.mk decompress.mk

all: | $(obj_folder)

all: $(compress_binary_name) $(decompress_binary_name) 

$(obj_folder):
	mkdir $@

$(obj_folder)/%.o: $(src_folder)/%.c $(lib_files)
	$(CC) -c $< -o $@ $(CFLAGS) -I $(lib_folder)

clean:
	rm -rf $(obj_files) $(decompress_binary_name) $(compress_binary_name) $(compress_output_folder)/*.* $(decompress_output_folder)/*.*
	rmdir $(compress_output_folder) $(decompress_output_folder) $(obj_folder)

echo:
	@echo '$(lib_files)' '$(obj_files)' '$(src_files)'
	@echo "$(compress_binary_name) $(compress_input_folder) $(compress_output_folder)"
	@echo "$(decompress_binary_name) $(decompress_input_folder) $(decompress_output_folder) $(default_inputfile)"

# INSTRUÇÕES DE USO
#
# gerar os executaveis:
# 	> make
#
# compactar um arquivo contido na pasta 'input':
# 	> make compress default_input_file="nome_do_arquivo.txt"
#
# descompactar um arquivo contido na pasta 'compactados':
# 	> make decompress default_input_file="nome_do_arquivo.txt.comp"
#
# limpar todos os objetos/compactados/executaveis:
# 	> make clean
