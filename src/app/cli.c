#include "app/cli.h"
#include "app/filenaming.h"
#include "core/liman.h"
#include "platform/ArgParser.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/process.h"

/* structs */

typedef struct CallbackData {
  ArgParser *parser;
  char *inputfilename;
  char *outputfilename;
  int flags;
} CallbackData;

typedef struct Command {
  char *inputfilename;
  char *outputfilename;
  int mode;
  int opts;
  void (*handler)(const char *in, const char *out, int opts);
} Command;

typedef struct CLI {
  /* cmdline arguments */
  int argc;
  char **argv;

  /* cmdline parser*/
  ArgParser *parser;

  /* the 'data' argument provided to the parser callback function */
  CallbackData *data;

  /* the cli-parsed command */
  Command cmd;
} CLI;

/* functions */

static CLI cli_new(int argc, char **argv);
static void cli_parse(CLI *cli);
static void cli_execute(CLI *cli);
static void cli_destroy(CLI *cli);
static CallbackData *callback_new(ArgParser *parser);
static CallbackData *callback_destroy(CallbackData *data);
static void callback(int code, char *arg, void *data);

/* data */

static Param compressParam = (Param){
    .name = "compress",
    .handler = callback,
    .data = NULL,
};

static Param decompressParam = (Param){
    .name = "decompress",
    .handler = callback,
    .data = NULL,
};

static Param inspectParam = (Param){
    .name = "inspect",
    .handler = callback,
    .data = NULL,
};

static const Arg args[] = {
    (Arg){'i', "input", ARG_TYPE_ARG, "Set inputfile filename"},
    (Arg){'o', "output", ARG_TYPE_OPTARG, "Set output filename"},
    (Arg){'h', "help", ARG_TYPE_NOARG, "Show this help"},
};

static const Arg inspect_args[] = {
    (Arg){'r', "header", ARG_TYPE_NOARG, "Analyze the header of a .comp file"},
    (Arg){'b', "body", ARG_TYPE_NOARG, "Analyze the body of a .comp file"},
    (Arg){'c', "codes", ARG_TYPE_NOARG,
          "Analyze the code generated for each ASCII letter of a file"},
    (Arg){'t', "tree", ARG_TYPE_NOARG, "Analyze the tree of a .comp file"},
};

static const Command EMPTY_CMD = (Command){
    NULL, NULL, 0, 0, NULL,
};

/* impl */

void cli_init(int argc, char **argv) {
  CLI cli = cli_new(argc, argv);
  cli_parse(&cli);
  cli_execute(&cli);
  cli_destroy(&cli);
}

static CLI cli_new(int argc, char **argv) {
  CLI cli = {.argc = argc,
             .argv = argv,
             .parser = NULL,
             .data = NULL,
             .cmd = EMPTY_CMD};

  // Creating cmd parser
  cli.parser = argParser_new();
  argParser_set_program_name(cli.parser, argv[0]);

  // Setting up the 'data' callback
  cli.data = callback_new(cli.parser);
  compressParam.data = cli.data;
  decompressParam.data = cli.data;
  inspectParam.data = cli.data;

  // Adding positional param
  size_t compress_idx = argParser_add_param(cli.parser, compressParam);
  size_t decompress_idx = argParser_add_param(cli.parser, decompressParam);
  size_t inspect_idx = argParser_add_param(cli.parser, inspectParam);

  // Adding slash params to each positional param
#define ARR_SZ(arr) (sizeof(arr) / sizeof(arr[0]))
  for (int i = 0; i < ARR_SZ(args); i++) {
    argParser_add_arg(cli.parser, compress_idx, args[i]);
    argParser_add_arg(cli.parser, decompress_idx, args[i]);
    argParser_add_arg(cli.parser, inspect_idx, args[i]);
  }

  for (int i = 0; i < ARR_SZ(inspect_args); i++) {
    argParser_add_arg(cli.parser, inspect_idx, inspect_args[i]);
  }
#undef ARR_SZ

  // Setting the default param to be 'compress'
  argParser_set_default_param(cli.parser, compress_idx);

  return cli;
}

static void cli_parse(CLI *cli) {
  Command cmd = EMPTY_CMD;

  ArgParser *parser = cli->parser;
  int argc = cli->argc;
  char **argv = cli->argv;

  // Parsing the supplied commandline
  cmd.mode = argParser_process(parser, argc, argv);

  CallbackData opts = *cli->data;
  if (!opts.inputfilename) {
    log_warning("no inputfile detected, nothing will be done.");
    return;
  }

  switch (cmd.mode) {
  case 0:
    cmd.inputfilename = opts.inputfilename;
    cmd.outputfilename = opts.outputfilename;
    if (!cmd.outputfilename) {
      cmd.outputfilename = liman_build_compressed_filename(cmd.inputfilename);
    }
    cmd.opts = opts.flags;
    cmd.handler = compress;
    break;
  case 1:
    cmd.inputfilename = opts.inputfilename;
    cmd.outputfilename = opts.outputfilename;
    if (!cmd.outputfilename) {
      cmd.outputfilename = liman_build_uncompressed_filename(cmd.inputfilename);
    }
    cmd.opts = opts.flags;
    cmd.handler = decompress;
    break;
  case 2:
    cmd.inputfilename = opts.inputfilename;
    if (opts.outputfilename) {
      cmd.outputfilename = opts.outputfilename;
    } else if (opts.flags & HUHMAN_CODES) {
      cmd.outputfilename = liman_get_codes_filename(cmd.inputfilename);
    } else if (opts.flags & HUHMAN_PDF) {
      cmd.outputfilename = liman_get_tree_filename(cmd.inputfilename);
    } else if (opts.flags & HUHMAN_HEADER) {
      cmd.outputfilename = NULL;
    } else if (opts.flags & HUHMAN_BODY) {
      cmd.outputfilename = liman_build_uncompressed_filename(cmd.inputfilename);
    } else {
      opts.flags = HUHMAN_HEADER | HUHMAN_BODY;
      cmd.outputfilename = liman_build_uncompressed_filename(cmd.inputfilename);
    }
    cmd.opts = opts.flags;
    cmd.handler = inspect;
    break;
  }

  cli->cmd = cmd;
}

static void cli_execute(CLI *cli) {
  Command *cmd = &cli->cmd;
  if (cmd->handler && cmd->inputfilename) {
    cmd->handler(cmd->inputfilename, cmd->outputfilename, cmd->opts);
  }
}

static void cli_destroy(CLI *cli) {
  Command *cmd = &cli->cmd;

  mem_free(cmd->outputfilename);
  mem_free(cmd->inputfilename);

  if (cli->data) {
    cli->data = callback_destroy(cli->data);
  }

  if (cli->parser) {
    cli->parser = argParser_destroy(cli->parser);
  }
}

static CallbackData *callback_new(ArgParser *parser) {
  CallbackData *data = mem_alloc(sizeof(CallbackData));
  data->parser = parser;
  data->inputfilename = NULL;
  data->outputfilename = NULL;
  data->flags = 0;
  return data;
}

static CallbackData *callback_destroy(CallbackData *data) {
  mem_free(data);
  return NULL;
}

static void callback(int code, char *arg, void *data) {
  CallbackData *opts = data;
  int opt = 0;

  switch (code) {
  case 'i':
    opts->inputfilename = mem_salloc(arg);
    break;
  case 'o':
    opts->outputfilename = mem_salloc(arg);
    break;
  case 'h':
    argParser_print(opts->parser);
    break;
  case 'b':
    opt = HUHMAN_BODY;
    break;
  case 'c':
    opt = HUHMAN_CODES;
    break;
  case 'r':
    opt = HUHMAN_HEADER;
    break;
  case 't':
    opt = HUHMAN_PDF;
    break;
  }

  opts->flags = opts->flags | opt;
}
