# print each byte compressed/decompressed and its huffman code during runtime
debug := 0
# enable two pass decompression (slower)
twopass := 1

srcdir := src
libdir := lib
objdir := obj
src := $(wildcard $(srcdir)/*.c $(srcdir)/*/*.c)
lib := $(wildcard $(libdir)/*.h $(libdir)/*/*.h)
obj := $(subst $(srcdir),$(objdir),$(src:.c=.o))

CC := gcc
CFLAGS := -lm -g
ifeq ($(debug),1)
CFLAGS += -DDEBUG
endif
ifeq ($(twopass),1)
CFLAGS += -DTWO_PASS
endif

all: $(obj)
	$(CC) $^ -o huffman $(CFLAGS)
	$(CC) $^ -o unhuffman $(CFLAGS)
	$(CC) $^ -o huhman $(CFLAGS)

$(objdir):
	mkdir $(sort $(dir $(obj)))

$(objdir)/%.o: $(srcdir)/%.c $(lib) | obj
	$(CC) -c $< -o $@ $(CFLAGS) -I $(libdir)

clean:
	rm -rf obj huffman unhuffman huhman

echo:
	@echo '.h: ' $(lib)
	@echo '.o: ' $(obj)
	@echo '.c: ' $(src)
