#pragma once

enum HuhmanOpts {
  HUHMAN_HEADER = 1,
  HUHMAN_BODY = 2,
  HUHMAN_CODES = 4,
  HUHMAN_PDF = 8,
};

void compress(const char *inputfilename, const char *outputfilename, int opts);

void decompress(const char *inputfilename, const char *outputfilename,
                int opts);

void inspect(const char *inputfilename, const char *outputfilename, int opts);
