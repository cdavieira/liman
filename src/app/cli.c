#include "app/cli.h"
#include "app/filenaming.h"
#include "core/liman.h"
#include "platform/mem.h"
#include "platform/posix/ArgParser.h"
#include "utils/cstr.h"

#include <stdlib.h>

/**
 * this implementation sucks, but it suffices for now
 *
 * it will be improved over time (hopefully) */

typedef struct Opts {
  ArgParser *parser;
  char *inputfilename;
  char *outputfilename;
  int opts;
} Opts;

static void callback(int code, void *data, char *arg);

void cli_init(int argc, char **argv) {
  int mode = 0;
  int argsize = 3;

  // an optional positional param is expected following the executable name
  char **positional_param = argc > 1 ? argv + 1 : NULL;

  Arg args[] = {
      (Arg){'i', "input", ARG_TYPE_ARG, "Set inputfile filename"},
      (Arg){'o', "output", ARG_TYPE_OPTARG, "Set output filename"},
      (Arg){'h', "help", ARG_TYPE_NOARG, "Show this help"},
      (Arg){'r', "header", ARG_TYPE_NOARG,
            "Analyze the header of a .comp file"},
      (Arg){'b', "body", ARG_TYPE_NOARG, "Analyze the body of a .comp file"},
      (Arg){'c', "codes", ARG_TYPE_NOARG,
            "Analyze the code generated for each ASCII letter of a file"},
      (Arg){'t', "tree", ARG_TYPE_NOARG, "Analyze the tree of a .comp file"},
  };

  if (positional_param && cstr_equals(*positional_param, "decompress")) {
    argsize = 3;
    mode = 1;
  } else if (positional_param && cstr_equals(*positional_param, "inspect")) {
    argsize = 7;
    mode = 2;
  }

  if (positional_param && mode) {
    *positional_param = "-";
  }

  ArgParser *parser = argParser_new(argc, argv);

  for (int i = 0; i < argsize; i++) {
    argParser_add_arg(parser, args[i]);
  }

  Opts *myopts = mem_zalloc(sizeof(Opts));
  myopts->parser = parser;
  myopts->opts = 0;
  myopts->inputfilename = NULL;
  myopts->outputfilename = NULL;

  argParser_set_data(parser, myopts);
  argParser_set_handler(parser, callback);

  argParser_process(parser);

  parser = argParser_destroy(parser);

  char *inputfilename = myopts->inputfilename;
  int opts = myopts->opts;

  mem_free(myopts);

  if (!inputfilename) {
    return;
  }

  char *outputfile = NULL;
  switch (mode) {
  case 0:
    outputfile = liman_build_compressed_filename(inputfilename);
    compress(inputfilename, outputfile);
    break;
  case 1:
    outputfile = liman_build_uncompressed_filename(inputfilename);
    decompress(inputfilename, outputfile);
    break;
  case 2:
    if (opts & HUHMAN_CODES) {
      outputfile = liman_get_codes_filename(inputfilename);
      inspect(inputfilename, outputfile, opts);
      mem_free(outputfile);
      outputfile = NULL;
    }
    if (opts & HUHMAN_PDF) {
      outputfile = liman_get_tree_filename(inputfilename);
      inspect(inputfilename, outputfile, opts);
      mem_free(outputfile);
      outputfile = NULL;
    }
    if (opts & HUHMAN_HEADER) {
      inspect(inputfilename, NULL, opts);
    }
    if (opts & HUHMAN_BODY) {
      outputfile = liman_build_uncompressed_filename(inputfilename);
      inspect(inputfilename, outputfile, opts);
    }
    break;
  }

  if (outputfile) {
    mem_free(outputfile);
  }
}

static void callback(int code, void *data, char *arg) {
  Opts *opts = data;
  int opt = 0;

  switch (code) {
  case 'i':
    opts->inputfilename = arg;
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

  opts->opts = opts->opts | opt;
}
