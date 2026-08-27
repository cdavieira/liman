#include "core/CodeLookup.h"
#include "core/ByteFrequency.h"
#include "core/HuffmanCode.h"
#include "core/HuffmanTree.h"
#include "platform/mem.h"
#include "utils/types/common.h"

struct CodeLookup {
  HuffmanCode **map;
  size_t size;
};

CodeLookup *
codeLookup_from_huffmanTree(HuffmanTree *ht)
{
  CodeLookup *lk = mem_alloc(sizeof(struct CodeLookup));
  lk->size = byteFreq_get_set_size();
  lk->map = mem_zalloc(lk->size * sizeof(HuffmanCode *));

  HuffmanTree *code;
  for (int i = 0; i < lk->size; i++)
  {
    code = huffmanTree_search_key(ht, i);
    lk->map[i] = code ? huffmanTree_get_value(code) : NULL;
  }

  return lk;
}

CodeLookup *
codeLookup_destroy(CodeLookup *lk)
{
  lk->map = mem_free(lk->map);
  return mem_free(lk);
}

const HuffmanCode *
codeLookup_get(CodeLookup *lk, int code)
{
  if (code < 0 || code >= lk->size)
  {
    return NULL;
  }
  return lk->map[code];
}
