#include "core/HuffmanCode.h"
#include "platform/mem.h"
#include "utils/bits.h"

struct HuffmanCode {
  unsigned int key;
  Code value;
  unsigned long weight;
};

HuffmanCode *
huffmanCode_new(unsigned key, unsigned long const weight)
{
  HuffmanCode *c = mem_alloc(sizeof(HuffmanCode));
  c->key = key;
  c->value = (Code){0, 0};
  c->weight = weight;
  return c;
}

HuffmanCode *
huffmanCode_destroy(HuffmanCode *c)
{
  if (!c)
  {
    return NULL;
  }
  mem_free(c);
  return NULL;
}

unsigned
huffmanCode_get_key(const HuffmanCode *const c)
{
  return c ? c->key : 0;
}

unsigned long
huffmanCode_get_weight(const HuffmanCode *const c)
{
  return c ? c->weight : 0;
}

Code
huffmanCode_get_value(const HuffmanCode *const c)
{
  return c ? c->value : (Code){0, 0};
}

HuffmanCode *
huffmanCode_set_key(HuffmanCode *const c, unsigned const key)
{
  if (c)
  {
    c->key = key;
  }
  return c;
}

HuffmanCode *
huffmanCode_set_weight(HuffmanCode *const c, unsigned long const weight)
{
  if (c)
  {
    c->weight = weight;
  }
  return c;
}

HuffmanCode *
huffmanCode_set_value(HuffmanCode *c, unsigned len, unsigned long val)
{
  if (c)
  {
    c->value.code = val;
    c->value.len = len;
  }
  return c;
}

int
huffmanCode_has_value(const HuffmanCode *c)
{
  return c->value.len != 0;
}

char *
huffmanCode_stringify(HuffmanCode *c)
{
  Code *code = &c->value;
  char *route = mem_zalloc((code->len + 1) * sizeof(char));
  for (int i = code->len - 1, j = 0; i >= 0; i--, j++)
  {
    route[j] = '0' + bits_get_size_bit(code->code, i);
  }
  route[code->len] = '\0';
  return route;
}

unsigned
huffmanCode_compare_key(const HuffmanCode *const c, unsigned const *const key)
{
  return c ? c->key == *key : 0;
}

unsigned
huffmanCode_compare_weight(const HuffmanCode *const c1,
                           const HuffmanCode *const c2)
{
  if (c1 && c2)
  {
    return c1->weight >= c2->weight;
  }
  return 0;
}
