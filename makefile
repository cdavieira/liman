project := liman
srcdir := src
libdir := lib
objdir := obj
testdir := tests/unit
teste2escript := ./tests/test-e2e.sh

src := $(wildcard $(srcdir)/*.c $(srcdir)/*/*.c $(srcdir)/*/*/*.c)
lib := $(wildcard $(libdir)/*.h $(libdir)/*/*.h $(libdir)/*/*/*.h)
obj := $(subst $(srcdir),$(objdir),$(src:.c=.o))
testsrc := $(wildcard $(testdir)/*.c)
testelf := $(testsrc:.c=)

MKDIR := mkdir
RM := rm -rf
BEAR := bear
CLANG_FORMAT := clang-format
CC := gcc
CFLAGS := -std=c23 \
	  -Wall -Wextra -Wpedantic \
	  -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable \
	  -g -I$(libdir)

.DEFAULT_GOAL := help
.PHONY: tests

all: $(project)

$(project): $(obj)
	$(CC) $^ -o $@

$(objdir):
	$(MKDIR) $(sort $(dir $(obj)))

$(objdir)/%.o: $(srcdir)/%.c $(lib) | $(objdir)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) unhuffman-* inspect-* *.comp $(objdir) $(project) $(testelf)

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

unit: $(testelf)

$(testdir)/%: $(filter-out obj/main.o,$(obj)) $(testdir)/%.c
	$(CC) $(CFLAGS) -o $@ $^

test: teste2e testunit

testunit: unit
	@echo 'Testing unit'
	@for ut in $(testelf) ; do \
		./$$ut ; \
	done
	@echo 'Done!'

teste2e:
	@echo 'Testing e2e'
	$(teste2escript)
	@echo 'Done!'

help:
	@echo 'liman - makefile help'
	@echo ''
	@echo 'Build'
	@echo 'make all  - build the project'
	@echo 'make unit - build unit tests as runnable executables'
	@echo ''
	@echo 'Run'
	@echo 'make run  - show the help message of liman'
	@echo 'make test - run all tests'
	@echo 'make teste2e  - run e2e test'
	@echo 'make testunit - run unit tests'
	@echo ''
	@echo 'Clean'
	@echo 'make clean - cleanup all build artifacts'
	@echo ''
	@echo 'Development'
	@echo 'make dev  - create compile_commands.json for language servers'
	@echo 'make lint - format all the files of this project'
	@echo 'make echo - take a look at build variables'
	@echo 'make help - show this help'
