#pragma once

#include "utils/types/common.h"

typedef struct Param {
  const char *name;
  void *data;
  void (*handler)(int, char *, void *);
} Param;

enum ArgType {
  ARG_TYPE_NOARG,
  ARG_TYPE_ARG,
  ARG_TYPE_OPTARG,
};

typedef struct Arg {
  int shortopt;
  const char *longopt;
  enum ArgType argtype;
  const char *description;
} Arg;

typedef struct ArgParser ArgParser;

ArgParser *argParser_new(void);
ArgParser *argParser_destroy(ArgParser *parser);

size_t argParser_add_param(ArgParser *parser, Param p);
void argParser_add_arg(ArgParser *parser, size_t idx, Arg arg);

void argParser_set_program_name(ArgParser *parser, const char *name);
void argParser_set_default_param(ArgParser *parser, size_t idx);

int argParser_process(ArgParser *parser, int argc, char **argv);

void argParser_print(ArgParser *parser);
void argParser_print_param(ArgParser *parser, size_t idx);
