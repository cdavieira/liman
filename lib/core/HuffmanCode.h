#pragma once

#include "utils/types/common.h"

typedef struct Code {
  size_t code;
  int len;
} Code;

typedef struct HuffmanCode HuffmanCode;

HuffmanCode *huffmanCode_new(unsigned key, unsigned long weight);
HuffmanCode *huffmanCode_destroy(HuffmanCode *c);

unsigned huffmanCode_get_key(const HuffmanCode *const c);
unsigned long huffmanCode_get_weight(const HuffmanCode *const c);
Code huffmanCode_get_value(const HuffmanCode *const c);

HuffmanCode *huffmanCode_set_key(HuffmanCode *const c, unsigned const key);
HuffmanCode *huffmanCode_set_weight(HuffmanCode *const c,
                                    unsigned long const weight);
HuffmanCode *
huffmanCode_set_value(HuffmanCode *c, unsigned len, unsigned long val);

int huffmanCode_has_value(const HuffmanCode *c);
char *huffmanCode_stringify(HuffmanCode *c);

unsigned huffmanCode_compare_key(const HuffmanCode *const c,
                                 unsigned const *const key);
unsigned huffmanCode_compare_weight(const HuffmanCode *const c1,
                                    const HuffmanCode *const c2);
