#pragma once

enum LimanOpts {
  LIMAN_OPT_HEADER = 1,
  LIMAN_OPT_BODY = 2,
  LIMAN_OPT_CODES = 4,
  LIMAN_OPT_PDF = 8,
};

static inline int limanOpts_add_opt(int opts, enum LimanOpts opt) {
  return opts | opt;
}

static inline int limanOpts_has_opt(int opts, enum LimanOpts opt) {
  return opts & opt;
}

void compress(const char *inputfilename, const char *outputfilename, int opts);

void decompress(const char *inputfilename, const char *outputfilename,
                int opts);

void inspect(const char *inputfilename, const char *outputfilename, int opts);
