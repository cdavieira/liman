#include "platform/ArgParser.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/process.h"
#include "utils/container/Vector.h"
#include "utils/cstr.h"
#include "utils/types/String.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

enum ArgFormat {
  ARG_FORMAT_SHORT,
  ARG_FORMAT_LONG,
  ARG_FORMAT_INVALID,
};

struct ParsedArg {
  enum ArgFormat fmt;
  char *key;
  char *value;
};

/* private functions */

static Arg *arg_new(Arg arg);
static Arg *arg_destroy(Arg *arg);
static void arg_print(Arg *arg);

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

enum ArgFormat parsedArg_infer_type(const char *s);
char *parsedArg_parse_key(char *token);
char *parsedArg_parse_value(char *token);
static struct ParsedArg *parsedArg_from_str(char *s);
static struct ParsedArg *parsedArg_destroy(struct ParsedArg *a);

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

int posParam_search_handler(void *a1, void *a2) {
  Arg *candidate = a1;
  struct ParsedArg *target = a2;
  if (target->fmt == ARG_FORMAT_SHORT) {
    return candidate->shortopt == target->key[0];
  }
  return cstr_equals(candidate->longopt, target->key);
}

int posParam_is_value(const char *s) {
  return s && (strlen(s) > 0) && s[0] != '-';
}

static void posParam_process(PositionalParam *param) {
  if (posParam_pre_process_check(param) < 0) {
    return;
  }

  char *raw_token;
  Arg *token_arg;
  for (int i = 0; i < param->argc; i++) {
    raw_token = param->argv[i];
    if (!raw_token) {
      continue;
    }

    struct ParsedArg *parsedArg = parsedArg_from_str(raw_token);
    if (!parsedArg->key) {
      parsedArg = parsedArg_destroy(parsedArg);
      continue;
    }

    token_arg = vector_search(param->args, parsedArg, posParam_search_handler);
    if (!token_arg) {
      log_warning("Unregistered token %s", raw_token);
      parsedArg = parsedArg_destroy(parsedArg);
      continue;
    }

    switch (token_arg->argtype) {
    case ARG_TYPE_OPTARG:
      if (parsedArg->value == NULL) {
        if ((i + 1) < param->argc) {
          parsedArg->value = mem_salloc(param->argv[i + 1]);
          i++;
        }
      }
      if (!posParam_is_value(parsedArg->value)) {
        log_error("Invalid argument for param '%c'", token_arg->shortopt);
        parsedArg = parsedArg_destroy(parsedArg);
        process_quick_abort();
      }
      param->handler(token_arg->shortopt, parsedArg->value, param->data);
      parsedArg = parsedArg_destroy(parsedArg);
      break;
    case ARG_TYPE_ARG:
      if (parsedArg->value == NULL) {
        if ((i + 1) < param->argc) {
          parsedArg->value = mem_salloc(param->argv[i + 1]);
          i++;
        }
      }
      if (parsedArg->value == NULL) {
        log_error("Missing argument for param '%c'", token_arg->shortopt);
        parsedArg = parsedArg_destroy(parsedArg);
        process_quick_abort();
      }
      if (!posParam_is_value(parsedArg->value)) {
        log_error("Invalid argument for param '%c'", token_arg->shortopt);
        parsedArg = parsedArg_destroy(parsedArg);
        process_quick_abort();
      }
      param->handler(token_arg->shortopt, parsedArg->value, param->data);
      parsedArg = parsedArg_destroy(parsedArg);
      break;
    case ARG_TYPE_NOARG:
      param->handler(token_arg->shortopt, NULL, param->data);
      parsedArg = parsedArg_destroy(parsedArg);
      break;
    default:
      log_error("Unknown arg type", NULL);
      parsedArg = parsedArg_destroy(parsedArg);
      process_quick_abort();
    }
  }
}

static void arg_print(Arg *arg) {
  int shortopt = arg->shortopt;
  const char *longopt = arg->longopt;
  enum ArgType opttype = arg->argtype;

  int hasshort = shortopt > 0;
  int haslong = longopt ? 1 : 0;
  int hasboth = hasshort && haslong;

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
    printf("=<arg>");
    break;
  case ARG_TYPE_OPTARG:
    printf("=(arg)");
    break;
    break;
  }
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

      putchar('[');

      arg_print(arg);

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

      putchar('\t');

      arg_print(arg);

      if (arg->description) {
        printf("\t: %s\n", arg->description);
      }
    }
  }
}

enum ArgFormat parsedArg_infer_type(const char *s) {
  size_t len = strlen(s);
  if (len <= 1) {
    return ARG_FORMAT_INVALID;
  }
  if (s[0] == '-' && s[1] != '-') {
    return ARG_FORMAT_SHORT;
  }
  if (s[0] == '-' && s[1] == '-') {
    return ARG_FORMAT_LONG;
  }
  return ARG_FORMAT_INVALID;
}

char *parsedArg_parse_key(char *token) {
  enum ArgFormat token_type = parsedArg_infer_type(token);
  String *s;
  char *res = NULL;
  const char *anchor = NULL;

  switch (token_type) {
  case ARG_FORMAT_SHORT:
    s = string_new();
    string_push_char(s, token[1]);
    res = string_drain(s);
    break;
  case ARG_FORMAT_LONG:
    s = string_new();
    anchor = cstr_find_at_first_char(token, '=');
    if (anchor) {
      for (const char *a = token + 2; a != anchor; a++) {
        string_push_char(s, a[0]);
      }
    } else {
      string_append(s, token + 2);
    }
    res = string_drain(s);
    break;
  default:
  case ARG_FORMAT_INVALID:
    break;
  }

  return res;
}

char *parsedArg_parse_value(char *token) {
  enum ArgFormat token_type = parsedArg_infer_type(token);
  String *s;
  char *res = NULL;
  const char *anchor = NULL;

  switch (token_type) {
  case ARG_FORMAT_SHORT:
    if (strlen(token) > 2) {
      s = string_from_ptr(token + 2);
      res = string_drain(s);
      break;
    }
  case ARG_FORMAT_LONG:
    anchor = cstr_find_after_first_char(token, '=');
    if (anchor) {
      s = string_from_ptr(anchor);
      res = string_drain(s);
    }
    break;
  default:
  case ARG_FORMAT_INVALID:
    break;
  }

  return res;
}

static struct ParsedArg *parsedArg_from_str(char *s) {
  int t = parsedArg_infer_type(s);
  struct ParsedArg *a = mem_alloc(sizeof(struct ParsedArg));
  a->fmt = t;
  a->key = parsedArg_parse_key(s);
  a->value = parsedArg_parse_value(s);
  return a;
}

static struct ParsedArg *parsedArg_destroy(struct ParsedArg *a) {
  if (a->key) {
    a->key = mem_free(a->key);
  }
  if (a->value) {
    a->value = mem_free(a->value);
  }
  return mem_free(a);
}
