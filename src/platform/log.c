#include "platform/log.h"
#include <stdio.h>

void log_info(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);

  fprintf(stderr, "info: ");
  vfprintf(stderr, fmt, va);
  putc('\n', stderr);

  va_end(va);
}

void log_debug(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);

  fprintf(stderr, "debug: ");
  vfprintf(stderr, fmt, va);
  putc('\n', stderr);

  va_end(va);
}

void log_warning(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);

  fprintf(stderr, "warning: ");
  vfprintf(stderr, fmt, va);
  putc('\n', stderr);

  va_end(va);
}

void log_error(const char *fmt, ...) {
  va_list va;
  va_start(va, fmt);

  fprintf(stderr, "error: ");
  vfprintf(stderr, fmt, va);
  putc('\n', stderr);

  va_end(va);
}
