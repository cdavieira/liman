#pragma

#include "core/HuffmanTree.h"
#include "utils/container/Bitmap.h"

typedef struct CodeLookup CodeLookup;
typedef struct HuffmanTree HuffmanTree;

CodeLookup *codeLookup_from_huffmanTree(HuffmanTree *ht);

CodeLookup *codeLookup_destroy(CodeLookup *lk);

const Bitmap *codeLookup_get(CodeLookup *lk, int code);
