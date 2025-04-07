srcdir := src
libdir := lib
objdir := obj
debug := 0
src := $(wildcard $(srcdir)/*.c $(srcdir)/*/*.c)
lib := $(wildcard $(libdir)/*.h $(libdir)/*/*.h)
obj := $(subst $(srcdir),$(objdir),$(src:.c=.o))

compress_bin := huffman
decompress_bin := unhuffman
analyze_bin := huhman
input := ./tests/jpg.jpg

CC := gcc
CFLAGS := -lm -g
ifeq ($(debug),1)
CFLAGS += -DDEBUG
endif

all: ;

all: | $(objdir)

all: $(compress_bin) $(decompress_bin) $(analyze_bin)

$(objdir):
	mkdir $(sort $(dir $(obj)))

$(objdir)/%.o: $(srcdir)/%.c $(lib)
	$(CC) -c $< -o $@ $(CFLAGS) -I $(libdir)

clean:
	rm -rf obj $(decompress_bin) $(compress_bin) $(analyze_bin) *.comp 

echo:
	@echo '.h: ' $(lib)
	@echo '.o: ' $(obj)
	@echo '.c: ' $(src)
	@echo "executables: $(compress_bin) $(decompress_bin) $(analyze_bin)"







## compress
$(compress_bin): $(obj)
	$(CC) $^ -o $@ $(CFLAGS)

compress: $(compress_bin)
	./$< ./$(input)

valc:
	valgrind --leak-check=full -s --track-origins=yes ./$(compress_bin) $(input)









## decompress
$(decompress_bin): $(obj)
	$(CC) $^ -o $@ $(CFLAGS)

decompress: $(decompress_bin)
	./$< /$(input).comp

vald:
	valgrind --leak-check=full -s --track-origins=yes ./$(decompress_bin) $(input).comp







## analyze
$(analyze_bin): $(obj)
	$(CC) $^ -o $@ $(CFLAGS)
