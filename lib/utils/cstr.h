#pragma once

#include "utils/types/common.h"

const char *cstr_find_at_last_char(const char *s, char ch);
const char *cstr_find_after_last_char(const char *s, char ch);
const char *cstr_find_at_first_char(const char *s, char ch);
const char *cstr_find_after_first_char(const char *s, char ch);
size_t cstr_count_occurrences(const char *s, char ch);
int cstr_equals(const char *s1, const char *s2);
