#pragma once

#define NO_SHORT_OPT (-2)

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

ArgParser *argParser_new(int argc, char **argv);
ArgParser *argParser_destroy(ArgParser *parser);

void argParser_add_arg(ArgParser *parser, Arg arg);
/**
 * this pointer should be allocated.
 */
void argParser_set_data(ArgParser *parser, void *data);
void argParser_set_handler(ArgParser *parser,
                           void (*handler)(int code, void *data, char *));

void argParser_process(ArgParser *parser);
void argParser_print(ArgParser *parser);
