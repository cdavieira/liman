#pragma once

#include "utils/types/common.h"

typedef struct String String;

String *string_new(void);
String *string_from_ptr(const char *ptr);
String *string_empty_from_ptrs(size_t size, const char *ptr[size]);

String *string_destroy(String *s);
char *string_drain(String *s);

void string_shift(String *s, const char *prefix);
void string_append(String *s, const char *suffix);
void string_push_char(String *s, char ch);

size_t string_length(const String *s);

void string_println(String *s);
