#pragma once

// exit_*, quit_*, abort_*, interrupt_*, ...

void abort_throw(int code, const char *msg);

void abort_default(const char *msg);

void abort_fast(void);

void abort_gracefully(void);

void abort_at(const char *filename, int line);
#define todo() abort_at(__FILE_NAME__, __LINE__)
