#pragma once

#include "utils/types/common.h"

const char *cstr_find_at_last_char(const char *s, char ch);
const char *cstr_find_after_last_char(const char *s, char ch);
char *cstr_min(const size_t size, const char *s[size]);
char *cstr_min2(const char *s1, const char *s2);
char *cstr_expand(const char *s, size_t sz);

int cstr_equals(const char *s1, const char *s2);
