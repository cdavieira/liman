# this file should not be used by itself.
# this is intended to be used by 'makefile' 

$(decompress_binary_name): | decompress_dirs

decompress_dirs:
	mkdir -p $(decompress_input_folder) $(decompress_output_folder)

$(decompress_binary_name): $(obj_files)
	$(CC) $^ -o $@ $(CFLAGS)

decompress:
	./$(decompress_binary_name) ./$(decompress_input_folder)/$(input)$(compress_filextension)
	mv $(decompress_input_folder)/$(input) $(decompress_output_folder)

vald:
	valgrind --leak-check=full -s --track-origins=yes ./$(decompress_binary_name) ./$(decompress_input_folder)/$(input)$(compress_filextension)
