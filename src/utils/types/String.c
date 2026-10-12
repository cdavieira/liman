#include "utils/types/String.h"
#include "platform/mem.h"
#include <stdio.h>
#include <string.h>

/* private API */

struct String {
  char *str;
  size_t size;
  size_t capacity; /* maybe use this in the future */
};

static void string_buffer_init(String *s);
static void string_buffer_deinit(String *s);
static void string_buffer_override(String *s, char *buffer);
static void string_buffer_free_and_override(String *s, char *buffer);
static char *string_concat(const char *s1, const char *s2);
static char *string_grow(const char *s, size_t sz);
static int string_has_contents(const String *s);

/* public API impl */

String *
string_new(void)
{
  String *s = mem_alloc(sizeof(struct String));
  string_buffer_init(s);
  return s;
}

String *
string_from_ptr(const char *ptr)
{
  String *s = mem_alloc(sizeof(struct String));
  char *cpy = mem_salloc(ptr);
  string_buffer_override(s, cpy);
  return s;
}

String *
string_destroy(String *s)
{
  if (s->str)
  {
    mem_free(s->str);
  }
  string_buffer_deinit(s);
  mem_free(s);
  return NULL;
}

char *
string_drain(String *s)
{
  char *str = s->str;
  s->str = NULL;
  string_destroy(s);
  return str;
}

void
string_shift(String *s, const char *prefix)
{
  if (!string_has_contents(s))
  {
    return;
  }

  char *new_str = string_concat(prefix, s->str);

  string_buffer_free_and_override(s, new_str);
}

void
string_append(String *s, const char *suffix)
{
  if (!string_has_contents(s))
  {
    return;
  }

  char *new_str = string_concat(s->str, suffix);

  string_buffer_free_and_override(s, new_str);
}

void
string_push_char(String *s, char ch)
{
  if (!string_has_contents(s))
  {
    return;
  }

  char *new_str = string_grow(s->str, 1);
  new_str[s->size] = ch;

  string_buffer_free_and_override(s, new_str);
}

size_t
string_length(const String *s)
{
  return strlen(s->str);
}

void
string_println(String *s)
{
  printf("\"%s\" (%zu)\n", s->str, s->size);
}

/* private API impl */

static void
string_buffer_init(String *s)
{
  s->str = mem_salloc("");
  s->size = 0;
  s->capacity = 0;
}

static void
string_buffer_deinit(String *s)
{
  s->str = NULL;
  s->size = 0;
  s->capacity = 0;
}

static void
string_buffer_override(String *s, char *buffer)
{
  s->str = buffer;
  s->size = strlen(buffer);
  s->capacity = s->size;
}

static void
string_buffer_free_and_override(String *s, char *buffer)
{
  if (s->str)
  {
    mem_free(s->str);
  }
  string_buffer_override(s, buffer);
}

static char *
string_concat(const char *s1, const char *s2)
{
  size_t sz = (strlen(s1) + strlen(s2) + 1) * sizeof(char);
  char *new_str = mem_zalloc(sz);
  strcat(new_str, s1);
  strcat(new_str, s2);
  return new_str;
}

static char *
string_grow(const char *s, size_t sz)
{
  char *new_str = mem_zalloc((strlen(s) + sz + 1) * sizeof(char));
  strcat(new_str, s);
  return new_str;
}

static int
string_has_contents(const String *s)
{
  return s && s->str;
}
