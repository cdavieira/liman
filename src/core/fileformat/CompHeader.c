#include "core/fileformat/CompHeader.h"
#include "core/ByteFrequency.h"
#include "core/HuffmanTree.h"
#include "platform/mem.h"
#include "platform/posix/BinaryWriter.h"
#include "utils/bits.h"
#include "utils/container/Bitmap.h"

typedef struct CompHeaderMetadata {
  size_t treeHeight;
  size_t treeNodeCount;
  size_t treeLeafCount;
  size_t treeMinBits;
  size_t treePadBits;
  size_t treeTotalBits; // msgTotalBits % 8 == 0
} CompHeaderMetadata;

struct CompHeader {
  Bitmap *bm;
  HuffmanTree *ht;
  CompHeaderMetadata metadata;
};

static CompHeaderMetadata compHeaderMetadata_from_huffmanTree(HuffmanTree *t);
static CompHeaderMetadata compHeaderMetadata_from_bitmap(Bitmap *bm);
static Bitmap *compHeader_fill_bitmap_with_huffmanTree(CompHeader *header);
static void compHeader_fill_bitmap_with_huffmanTree_rec(HuffmanTree *hf,
                                                        Bitmap *header);
static HuffmanTree *compHeader_build_tree_from_bitmap(CompHeader *header);
static HuffmanTree *compHeader_build_tree_from_bitmap_rec(Bitmap *bm,
                                                          unsigned *index);

CompHeader *
compHeader_from_huffmanTree(HuffmanTree *huffmanTree)
{
  CompHeader *h = mem_alloc(sizeof(CompHeader));
  h->ht = huffmanTree;
  h->metadata = compHeaderMetadata_from_huffmanTree(huffmanTree);
  h->bm = compHeader_fill_bitmap_with_huffmanTree(h);
  return h;
}

CompHeader *
compHeader_from_bitmap(Bitmap *bm)
{
  CompHeader *hdr = mem_alloc(sizeof(struct CompHeader));
  hdr->bm = bm;
  hdr->metadata = compHeaderMetadata_from_bitmap(bm);
  hdr->ht = compHeader_build_tree_from_bitmap(hdr);
  return hdr;
}

// TODO: the header seems to own the huffmanTree it has a pointer to. But
// 'CompBody' also stores a pointer to that type. Maybe we should pass the
// CompHeader itself to the CompBody type instead?
CompHeader *
compHeader_destroy(CompHeader *header)
{
  huffmanTree_destroy(header->ht);
  bitmapLibera(header->bm);
  mem_free(header);
  return NULL;
}

Bitmap *
compHeader_get_bitmap(CompHeader *header)
{
  return header->bm;
}

HuffmanTree *
compHeader_get_huffmanTree(CompHeader *header)
{
  return header->ht;
}

size_t
compHeader_get_min_size_in_bits(CompHeader *header)
{
  return header->metadata.treeMinBits;
}

size_t
compHeader_get_padding_size_in_bits(CompHeader *header)
{
  return header->metadata.treePadBits;
}

size_t
compHeader_get_total_size_in_bytes(CompHeader *header)
{
  return bits_toBytes(header->metadata.treeTotalBits);
}

size_t
compHeader_get_total_size_in_bits(CompHeader *header)
{
  return header->metadata.treeTotalBits;
}

void
compHeader_dump_into_binaryWriter(CompHeader *header, BinaryWriter *writer)
{
  binWriter_write_bitmap(writer, header->bm);
}

size_t
compHeader_get_max_theoretical_size_in_bits(void)
{
  const size_t max_leaf_count = byteFreq_get_set_size();
  const size_t max_non_leaf_count = max_leaf_count - 1;
  const size_t max_node_count = max_non_leaf_count + max_leaf_count;
  const size_t max_size_bits = max_leaf_count * 8 + max_node_count;
  return max_size_bits + bits_padding(max_size_bits);
}

size_t
compHeader_get_max_theorical_size(void)
{
  return bits_toBytes(compHeader_get_max_theoretical_size_in_bits());
}

static CompHeaderMetadata
compHeaderMetadata_from_huffmanTree(HuffmanTree *t)
{
  CompHeaderMetadata h;
  h.treeHeight = huffmanTree_get_height(t);
  h.treeNodeCount = huffmanTree_get_count(t);
  h.treeLeafCount = huffmanTree_get_leaf_count(t);
  h.treeMinBits =
      h.treeNodeCount + 8 * h.treeLeafCount; // each Leaf encodes 1 byte,
                                             // therefore 8 additional bits
  h.treePadBits = bits_padding(h.treeMinBits);
  h.treeTotalBits = h.treeMinBits + h.treePadBits;
  return h;
}

static CompHeaderMetadata
compHeaderMetadata_from_bitmap(Bitmap *bm)
{
  CompHeaderMetadata metadata;
  size_t minBits = bm ? bitmapGetLength(bm) : 0;
  metadata.treeMinBits = minBits;
  metadata.treePadBits = bits_padding(minBits);
  metadata.treeTotalBits = minBits + metadata.treePadBits;
  return metadata;
}

static Bitmap *
compHeader_fill_bitmap_with_huffmanTree(CompHeader *header)
{
  Bitmap *bm = bitmapInit(header->metadata.treeTotalBits);
  compHeader_fill_bitmap_with_huffmanTree_rec(header->ht, bm);
  return bm;
}

static void
compHeader_fill_bitmap_with_huffmanTree_rec(HuffmanTree *hf, Bitmap *header)
{
  if (!huffmanTree_is_leaf(hf))
  {
    bitmapAppendLeastSignificantBit(header, 0);
    compHeader_fill_bitmap_with_huffmanTree_rec(huffmanTree_get_left(hf),
                                                header);
    compHeader_fill_bitmap_with_huffmanTree_rec(huffmanTree_get_right(hf),
                                                header);
    return;
  }

  bitmapAppendLeastSignificantBit(header, 1);
  unsigned char ch = huffmanTree_get_key(hf);
  for (int i = 7; i >= 0; i--)
  {
    bitmapAppendLeastSignificantBit(header, bits_get_char_bit(ch, i));
  }
}

static HuffmanTree *
compHeader_build_tree_from_bitmap(CompHeader *header)
{
  if (!header->bm)
  {
    return NULL;
  }
  unsigned idx = 0;
  HuffmanTree *ht = compHeader_build_tree_from_bitmap_rec(header->bm, &idx);
  huffmanTree_gencodes(ht);
  return ht;
}

static HuffmanTree *
compHeader_build_tree_from_bitmap_rec(Bitmap *bm, unsigned *index)
{
  if (*index >= bitmapGetLength(bm))
  {
    return NULL;
  }

  unsigned char bit = bitmapGetBit(bm, (*index)++);
  HuffmanTree *node = NULL;
  if (bit)
  { // Leaf node
    unsigned char ascii = bitmapGetByte(bm, *index);
    *index = *index + 8;
    node = huffmanTree_new(ascii, 0, 0, 0);
  }
  else
  { // Non leaf node
    node = huffmanTree_new(0, 0, 0, 0);
    node = huffmanTree_set_left(
        node, compHeader_build_tree_from_bitmap_rec(bm, index));
    node = huffmanTree_set_right(
        node, compHeader_build_tree_from_bitmap_rec(bm, index));
  }

  return node;
}
