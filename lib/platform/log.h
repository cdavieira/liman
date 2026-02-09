#pragma once

#include <stdarg.h>

void log_info(const char *fmt, ...);
void log_debug(const char *fmt, ...);
void log_warning(const char *fmt, ...);
void log_error(const char *fmt, ...);

#define log_hit log_debug("%s: %d", __FILE__, __LINE__)
