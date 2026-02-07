#include "platform/posix/ArgParser.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/process.h"
#include "utils/types/String.h"

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#define ARG_COUNT (10)

/**
 * this implementation sucks, but it suffices for now
 *
 * it will be improved over time (hopefully) */

struct ArgParser {
  Arg args[ARG_COUNT];
  int capacity;
  int size;
  int argc;
  char **argv;
  void *data;
  void (*handler)(int, void *, char *);
};

extern int optopt;
extern int optind;
extern char *optarg;
extern int opterr;

static const struct option BLANK_OPTION = (struct option){0};
static int long_catcher = -2;

ArgParser *argParser_new(int argc, char **argv) {
  ArgParser *parser = mem_alloc(sizeof(struct ArgParser));
  parser->capacity = ARG_COUNT;
  parser->size = 0;
  for (int i = 0; i < ARG_COUNT; i++) {
    parser->args[i] = (Arg){
        .argtype = ARG_TYPE_ARG,
        .longopt = NULL,
        .shortopt = 0,
    };
  }
  parser->data = NULL;
  parser->handler = NULL;
  parser->argc = argc;
  parser->argv = argv;
  return parser;
}
ArgParser *argParser_destroy(ArgParser *parser) {
  mem_free(parser);
  return NULL;
}

void argParser_add_arg(ArgParser *parser, Arg arg) {
  if (parser->size >= ARG_COUNT) {
    return;
  }
  parser->args[parser->size++] = arg;
}

void argParser_set_data(ArgParser *parser, void *data) { parser->data = data; }

void argParser_set_handler(ArgParser *parser,
                           void (*handler)(int code, void *data, char *a)) {
  parser->handler = handler;
}

static int argParser_map_argType(enum ArgType argType) {
  switch (argType) {
  case ARG_TYPE_ARG:
    return required_argument;
  case ARG_TYPE_OPTARG:
    return optional_argument;
  case ARG_TYPE_NOARG:
  default:
    return no_argument;
  }
}

static char *argParser_build_optstring(ArgParser *parser) {
  String *s = string_new();

  for (int i = 0; i < parser->size; i++) {
    string_push_char(s, parser->args[i].shortopt);

    switch (parser->args[i].argtype) {
    case ARG_TYPE_ARG:
      string_push_char(s, ':');
      break;
    case ARG_TYPE_OPTARG:
      string_push_char(s, ':');
      string_push_char(s, ':');
      break;
    case ARG_TYPE_NOARG:
      break;
    }
  }

  char *str = string_drain(s);

  return str;
}

static void argParser_build_longopts(ArgParser *parser,
                                     struct option longopts[ARG_COUNT]) {
  for (int i = 0; i < parser->capacity; i++) {
    if (i >= parser->size) {
      longopts[i] = BLANK_OPTION;
      continue;
    }
    longopts[i] = (struct option){
        .has_arg = argParser_map_argType(parser->args[i].argtype),
        .name = parser->args[i].longopt,
        .flag = &long_catcher,
        .val = parser->args[i].shortopt,
    };
  }
}

void argParser_process(ArgParser *parser) {
  if (!parser->handler) {
    return;
  }

  if (parser->argc == 1) {
    argParser_print(parser);
    return;
  }

  // turn off getopt default error msg
  opterr = 0;

  char *optstring = argParser_build_optstring(parser);

  struct option longopts[ARG_COUNT];
  argParser_build_longopts(parser, longopts);

  int longopt;
  int ch;
  while ((ch = getopt_long(parser->argc, parser->argv, optstring, longopts,
                           &longopt)) != -1) {
    switch (ch) {
    case '?':
      log_warning("ArgParser option error: %c", optopt);
      process_quick_abort();
      break;
    case ':':
      log_warning("ArgParser missing argument: %d", ch);
      process_quick_abort();
      break;
    default:
      ch = !ch ? long_catcher : ch;
      parser->handler(ch, parser->data, optarg);
      long_catcher = -2;
      break;
    }
  }

  mem_free(optstring);
}

void argParser_print(ArgParser *parser) {
  printf("SUMMARY\n");
  printf("\t%s", parser->argv[0]);

  // print calling summary
  if (parser->size > 0) {
    putchar(' ');

    for (int i = 0; i < parser->size; i++) {
      int shortopt = parser->args[i].shortopt;
      const char *longopt = parser->args[i].longopt;
      enum ArgType opttype = parser->args[i].argtype;

      int hasshort = shortopt != NO_SHORT_OPT;
      int haslong = longopt ? 1 : 0;
      int hasboth = hasshort && haslong;

      putchar('[');

      if (hasshort) {
        putchar('-');
        putchar(shortopt);
      }

      if (hasboth) {
        putchar('|');
      }

      if (haslong) {
        printf("--%s", longopt);
      }

      switch (opttype) {
      case ARG_TYPE_NOARG:
        break;
      case ARG_TYPE_ARG:
        printf(" <arg>");
        break;
      case ARG_TYPE_OPTARG:
        printf(" (arg)");
        break;
        break;
      }

      putchar(']');

      if ((i + 1) < parser->size) {
        putchar(' ');
      }
    }
  }

  putchar('\n');

  // print description
  if (parser->size > 0) {
    printf("\nARGUMENTS\n");

    for (int i = 0; i < parser->size; i++) {
      int shortopt = parser->args[i].shortopt;
      const char *longopt = parser->args[i].longopt;
      enum ArgType opttype = parser->args[i].argtype;
      const char *desc = parser->args[i].description;

      int hasshort = shortopt != NO_SHORT_OPT;
      int haslong = longopt ? 1 : 0;
      int hasboth = hasshort && haslong;

      putchar('\t');

      if (hasshort) {
        putchar('-');
        putchar(shortopt);
      }

      if (hasboth) {
        putchar('|');
      }

      if (haslong) {
        printf("--%s", longopt);
      }

      switch (opttype) {
      case ARG_TYPE_NOARG:
        break;
      case ARG_TYPE_ARG:
        printf(" <arg>");
        break;
      case ARG_TYPE_OPTARG:
        printf(" (arg)");
        break;
        break;
      }

      printf("\t: %s\n", desc);
    }
  }
}
