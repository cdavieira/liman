#include "utils/cstr.h"

#include <string.h>

const char *
cstr_find_at_last_char(const char *s, char ch)
{
  return strrchr(s, ch);
}
const char *
cstr_find_at_first_char(const char *s, char ch)
{
  return strchr(s, ch);
}

const char *
cstr_find_after_first_char(const char *s, char ch)
{
  const char *last = cstr_find_at_first_char(s, ch);
  return last ? last + 1 : NULL;
}

const char *
cstr_find_after_last_char(const char *s, char ch)
{
  const char *last = cstr_find_at_last_char(s, ch);
  return last ? last + 1 : NULL;
}

size_t
cstr_count_occurrences(const char *s, char ch)
{
  size_t count = 0;
  for (size_t i = 0; s[i] != '\0'; i++)
  {
    if (s[i] == ch)
    {
      count++;
    }
  }
  return count;
}

int
cstr_equals(const char *s1, const char *s2)
{
  return !strcmp(s1, s2);
}
