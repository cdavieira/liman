# this file should not be used by itself.
# this is intended to be used by 'makefile' 
#
$(compress_binary_name): | compress_dirs

compress_dirs:
	mkdir -p $(compress_input_folder) $(compress_output_folder)

$(compress_binary_name): $(obj_files)
	$(CC) $^ -o $@ $(CFLAGS)

compress:
	./$(compress_binary_name) ./$(compress_input_folder)/$(input)
	mv $(compress_input_folder)/*$(compress_filextension) $(compress_output_folder)/$(input)$(compress_filextension)

valc:
	valgrind --leak-check=full -s --track-origins=yes ./$(compress_binary_name) $(compress_input_folder)/$(input)
