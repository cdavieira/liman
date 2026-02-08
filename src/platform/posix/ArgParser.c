#include "platform/posix/ArgParser.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/process.h"
#include "utils/container/Vector.h"
#include "utils/cstr.h"
#include "utils/types/String.h"

#include <stdio.h>
#include <stdlib.h>

#include <getopt.h>

#define ARG_MIN_COUNT (16)
#define PARAM_MIN_COUNT (8)

/* structs */

typedef struct PositionalParam {
  const char *name;
  int param_id;
  Vector *args;
  int argc;
  char **argv;
  void *data;
  void (*handler)(int, char *, void *);
} PositionalParam;

struct ArgParser {
  const char *executable_name;
  size_t default_param;
  Vector *positional_params;
};

/* data */

extern int optopt;
extern int optind;
extern char *optarg;
extern int opterr;
static const struct option BLANK_OPTION = (struct option){0};
static const Arg BLANK_ARG = (Arg){
    .argtype = ARG_TYPE_NOARG,
    .longopt = NULL,
    .shortopt = 0,
};

/* private functions */

static inline int map_argType(enum ArgType argType);
static char *build_optstring(PositionalParam *param);
static struct option *build_longopts(PositionalParam *param);

static Arg *arg_new(Arg arg);
static Arg *arg_destroy(Arg *arg);

static PositionalParam *posParam_new(void);
static PositionalParam *posParam_destroy(PositionalParam *param);
static void posParam_set_args(PositionalParam *param, int argc, char **argv);
static void posParam_set_id(PositionalParam *param, int param_id);
static void posParam_set_name(PositionalParam *param, const char *name);
static void posParam_set_data(PositionalParam *param, void *data);
static void posParam_set_handler(PositionalParam *param,
                                 void (*handler)(int code, char *arg,
                                                 void *param));
static void posParam_add(PositionalParam *param, Arg arg);
static int posParam_pre_process_check(PositionalParam *param);
static void posParam_process(PositionalParam *param);
static void posParam_print(PositionalParam *param, const char *executable,
                           int is_default);

/* impl */

ArgParser *argParser_new(void) {
  ArgParser *parser = mem_alloc(sizeof(struct ArgParser));
  parser->positional_params = vector_new(PARAM_MIN_COUNT);
  parser->executable_name = NULL;
  return parser;
}

ArgParser *argParser_destroy(ArgParser *parser) {
  vector_destroy(parser->positional_params,
                 (void *(*)(void *))posParam_destroy);
  mem_free(parser);
  return NULL;
}

void argParser_set_executable_name(ArgParser *parser, const char *name) {
  parser->executable_name = name;
}

size_t argParser_add_param(ArgParser *parser, Param p) {
  PositionalParam *param = posParam_new();
  posParam_set_name(param, p.name);
  posParam_set_data(param, p.data);
  posParam_set_handler(param, p.handler);
  size_t idx = vector_append(parser->positional_params, param);
  posParam_set_id(param, idx);
  return idx;
}

void argParser_add_arg(ArgParser *parser, size_t idx, Arg arg) {
  PositionalParam *param = vector_get_item(parser->positional_params, idx);
  posParam_add(param, arg);
}

void argParser_set_program_name(ArgParser *parser, const char *name) {
  parser->executable_name = name;
}

void argParser_set_default_param(ArgParser *parser, size_t idx) {
  parser->default_param = idx;
}

static inline int is_positional_param_name(const char *name) {
  return name && name[0] != '-';
}

static int cmp_params(void *s1, void *s2) {
  PositionalParam *p1 = (PositionalParam *)s1;
  PositionalParam *p2 = (PositionalParam *)s2;
  return cstr_equals(p1->name, (p2->name));
}

int argParser_process(ArgParser *parser, int argc, char **argv) {
  if (argc <= 0) {
    return -1;
  }

  PositionalParam *param = NULL;
  const char *paramName = is_positional_param_name(argv[1]) ? argv[1] : NULL;
  if (paramName) {
    PositionalParam key = {
        .name = paramName,
    };
    param = vector_search(parser->positional_params, (void *)&key, cmp_params);
    argc--;
    argv++;
  }

  if (!param) {
    param = vector_get_item(parser->positional_params, parser->default_param);
  }

  if (!param) {
    return -1;
  }

  posParam_set_args(param, argc, argv);

  posParam_process(param);

  return param->param_id;
}

void argParser_print(ArgParser *parser) {
  size_t sz = vector_get_size(parser->positional_params);
  for (size_t i = 0; i < sz; i++) {
    PositionalParam *param = vector_get_item(parser->positional_params, i);
    int is_default = param->param_id == parser->default_param;
    posParam_print(param, parser->executable_name, is_default);
  }
}

/* impl internal */

static inline int map_argType(enum ArgType argType) {
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

static char *build_optstring(PositionalParam *param) {
  String *s = string_new();
  size_t sz = vector_get_size(param->args);

  for (int i = 0; i < sz; i++) {
    Arg *arg = vector_get_item(param->args, i);
    string_push_char(s, arg->shortopt);

    switch (arg->argtype) {
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

static struct option *build_longopts(PositionalParam *param) {
  size_t sz = vector_get_size(param->args);

  struct option *longopts = mem_alloc((sz + 1) * sizeof(struct option));

  for (int i = 0; i < sz; i++) {
    Arg *arg = vector_get_item(param->args, i);
    longopts[i] = (struct option){
        .has_arg = map_argType(arg->argtype),
        .name = arg->longopt,
        .flag = NULL,
        .val = arg->shortopt,
    };
  }

  longopts[sz] = BLANK_OPTION;

  return longopts;
}

static Arg *arg_new(Arg arg) {
  Arg *a = mem_alloc(sizeof(Arg));
  *a = arg;
  return a;
}

static Arg *arg_destroy(Arg *arg) { return mem_free(arg); }

static PositionalParam *posParam_new(void) {
  PositionalParam *param = mem_alloc(sizeof(struct PositionalParam));

  param->name = NULL;
  param->param_id = -1;
  param->args = vector_new(ARG_MIN_COUNT);
  param->argc = 0;
  param->argv = NULL;
  param->data = NULL;
  param->handler = NULL;

  return param;
}

static PositionalParam *posParam_destroy(PositionalParam *param) {
  param->args = vector_destroy(param->args, (void *(*)(void *))arg_destroy);
  param = mem_free(param);
  return param;
}

static void posParam_set_args(PositionalParam *param, int argc, char **argv) {
  param->argc = argc;
  param->argv = argv;
}

static void posParam_set_id(PositionalParam *param, int param_id) {
  param->param_id = param_id;
}

static void posParam_set_name(PositionalParam *param, const char *name) {
  param->name = name;
}

static void posParam_set_data(PositionalParam *param, void *data) {
  param->data = data;
}

static void posParam_set_handler(PositionalParam *param,
                                 void (*handler)(int code, char *arg,
                                                 void *param)) {
  param->handler = handler;
}

static void posParam_add(PositionalParam *param, Arg arg) {
  vector_append(param->args, arg_new(arg));
}

static int posParam_pre_process_check(PositionalParam *param) {
  if (!param) {
    return -1;
  }

  if (!param->argv) {
    return -2;
  }

  if (param->argc < 1) {
    return -3;
  }

  if (param->handler == NULL) {
    return -4;
  }

  if (param->name == NULL) {
    return -5;
  }

  return 0;
}

static void posParam_process(PositionalParam *param) {
  if (posParam_pre_process_check(param) < 0) {
    return;
  }

  // turn off getopt default error msg
  opterr = 0;

  char *optstring = build_optstring(param);
  struct option *longopts = build_longopts(param);

  int longopt;
  int ch;

  while ((ch = getopt_long(param->argc, param->argv, optstring, longopts,
                           &longopt)) != -1) {
    switch (ch) {
    case '?':
      log_warning("%s: option error: %c", param->name, optopt);
      process_quick_abort();
      break;
    case ':':
      log_warning("%s: missing argument: %d", param->name, ch);
      process_quick_abort();
      break;
    default:
      param->handler(ch, optarg, param->data);
      break;
    }
  }

  mem_free(optstring);
  mem_free(longopts);
}

static void posParam_print(PositionalParam *param, const char *executable,
                           int is_default) {
  printf("SUMMARY\n");
  if (is_default) {
    printf("\t%s [%s]", executable, param->name);
  } else {
    printf("\t%s %s", executable, param->name);
  }

  // print calling summary
  const size_t sz = vector_get_size(param->args);

  if (sz > 0) {
    putchar(' ');

    for (int i = 0; i < sz; i++) {
      Arg *arg = vector_get_item(param->args, i);

      int shortopt = arg->shortopt;
      const char *longopt = arg->longopt;
      enum ArgType opttype = arg->argtype;

      int hasshort = shortopt > 0;
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

      if ((i + 1) < sz) {
        putchar(' ');
      }
    }
  }

  putchar('\n');

  // print description
  if (sz > 0) {
    printf("\nARGUMENTS\n");

    for (int i = 0; i < sz; i++) {
      Arg *arg = vector_get_item(param->args, i);

      int shortopt = arg->shortopt;
      const char *longopt = arg->longopt;
      enum ArgType opttype = arg->argtype;
      const char *desc = arg->description;

      int hasshort = shortopt > 0;
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
