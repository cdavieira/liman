#include "platform/process.h"
#include "platform/log.h"

#include <stdlib.h>

// exit_*, quit_*, abort_*, interrupt_*, ...

void process_abort(const char *msg) {
  log_error("%s", msg);
  exit(EXIT_FAILURE);
}

void process_quick_abort(void) { exit(EXIT_FAILURE); }

void process_throw(int code, const char *msg) {
  log_error("%s", msg);
  exit(code);
}

void process_abort_at(const char *filename, int line) {
  log_warning("TODO: %s at line %d", filename, line);
  exit(EXIT_FAILURE);
}
