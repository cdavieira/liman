#include "platform/abort.h"
#include "platform/log.h"

#include <stdlib.h>
#include <unistd.h>

void abort_default(const char *msg) {
  log_error("%s", msg);
  exit(EXIT_FAILURE);
}

void abort_fast(void) { exit(EXIT_FAILURE); }

void abort_gracefully(void) { exit(EXIT_SUCCESS); }

void abort_throw(int code, const char *msg) {
  log_error("%s", msg);
  exit(code);
}

void abort_at(const char *filename, int line) {
  log_warning("TODO: %s at line %d", filename, line);
  exit(EXIT_FAILURE);
}
