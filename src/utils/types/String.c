#include "utils/types/String.h"
#include "platform/mem.h"
#include "utils/cstr.h"
#include <stdio.h>
#include <string.h>

struct String {
  char *str;
  size_t sz;
};

static void
string_set(String *s, char *buffer)
{
  if (s->str)
  {
    mem_free(s->str);
  }
  s->str = buffer;
  s->sz = strlen(buffer);
}

String *
string_new(void)
{
  String *s = mem_zalloc(sizeof(struct String));
  s->str = mem_salloc("");
  return s;
}

String *
string_from_ptr(const char *ptr)
{
  String *s = mem_zalloc(sizeof(struct String));
  s->str = mem_salloc(ptr);
  s->sz = strlen(s->str);
  return s;
}

String *
string_destroy(String *s)
{
  if (s->str)
  {
    mem_free(s->str);
    s->str = NULL;
    s->sz = 0;
  }
  mem_free(s);
  return NULL;
}

char *
string_drain(String *s)
{
  char *str = s->str;
  s->str = NULL;
  s->sz = 0;
  string_destroy(s);
  return str;
}

void
string_shift(String *s, const char *prefix)
{
  if (!s || !s->str)
  {
    return;
  }
  char *new_str = cstr_min2(s->str, prefix);
  strcat(new_str, prefix);
  strcat(new_str, s->str);
  string_set(s, new_str);
}

void
string_append(String *s, const char *suffix)
{
  if (!s || !s->str)
  {
    return;
  }
  char *new_str = cstr_min2(s->str, suffix);
  strcat(new_str, s->str);
  strcat(new_str, suffix);
  string_set(s, new_str);
}

void
string_push_char(String *s, char ch)
{
  if (!s || !s->str)
  {
    return;
  }
  char *new_str = cstr_expand(s->str, 1);
  strcat(new_str, s->str);
  new_str[s->sz] = ch;
  string_set(s, new_str);
}

size_t
string_length(const String *s)
{
  return strlen(s->str);
}

void
string_println(String *s)
{
  printf("\"%s\" (%zu)\n", s->str, s->sz);
}
