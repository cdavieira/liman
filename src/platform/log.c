#include "platform/log.h"
#include <stdio.h>
#include <time.h>

#define BUFFER_SIZE 1024

static char timestamp[BUFFER_SIZE];

static void
update_timestamp(void)
{
  time_t t = time(NULL);
  if (t == -1)
  {
    return;
  }

  struct tm *local = localtime(&t);
  if (local == NULL)
  {
    return;
  }

  if (strftime(timestamp, BUFFER_SIZE, "[%Y-%m-%d %H:%M:%S]", local) == 0)
  {
    return;
  }
}

void
log_info(const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);

  update_timestamp();
  fprintf(stderr, "%s info: ", timestamp);
  vfprintf(stderr, fmt, va);
  putc('\n', stderr);

  va_end(va);
}

void
log_debug(const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);

  update_timestamp();
  fprintf(stderr, "%s debug: ", timestamp);
  vfprintf(stderr, fmt, va);
  putc('\n', stderr);

  va_end(va);
}

void
log_warning(const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);

  update_timestamp();
  fprintf(stderr, "%s warning: ", timestamp);
  vfprintf(stderr, fmt, va);
  putc('\n', stderr);

  va_end(va);
}

void
log_error(const char *fmt, ...)
{
  va_list va;
  va_start(va, fmt);

  update_timestamp();
  fprintf(stderr, "%s error: ", timestamp);
  vfprintf(stderr, fmt, va);
  putc('\n', stderr);

  va_end(va);
}
