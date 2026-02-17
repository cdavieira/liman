#pragma

#include "core/HuffmanCode.h"
#include "core/HuffmanTree.h"

typedef struct CodeLookup CodeLookup;
typedef struct HuffmanTree HuffmanTree;

CodeLookup *codeLookup_from_huffmanTree(HuffmanTree *ht);

CodeLookup *codeLookup_destroy(CodeLookup *lk);

const HuffmanCode *codeLookup_get(CodeLookup *lk, int code);
