project := liman
srcdir := src
libdir := lib
objdir := obj
src := $(wildcard $(srcdir)/*.c $(srcdir)/*/*.c $(srcdir)/*/*/*.c)
lib := $(wildcard $(libdir)/*.h $(libdir)/*/*.h $(libdir)/*/*/*.h)
obj := $(subst $(srcdir),$(objdir),$(src:.c=.o))

CC := gcc
MKDIR := mkdir
RM := rm -rf
BEAR := bear
CLANG_FORMAT := clang-format

CFLAGS := -std=c23 -Wall -Wextra -Wpedantic -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -g -I$(libdir)

all: $(project)

$(project): $(obj)
	$(CC) $^ -o $@

$(objdir):
	$(MKDIR) $(sort $(dir $(obj)))

$(objdir)/%.o: $(srcdir)/%.c $(lib) | $(objdir)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) unhuffman-* inspect-* *.comp $(objdir) $(project)

run:
	./$(project)

dev:
	$(BEAR) -- make

lint:
	$(CLANG_FORMAT) -i $(src) $(lib)

echo:
	@echo '.h: ' $(lib)
	@echo '.o: ' $(obj)
	@echo '.c: ' $(src)
