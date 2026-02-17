#include "core/liman.h"
#include "core/ByteFrequency.h"
#include "core/HuffmanTree.h"
#include "core/TreeList.h"
#include "core/fileformat/CompHeader.h"
#include "core/fileformat/CompReader.h"
#include "core/fileformat/CompWriter.h"
#include "platform/log.h"
#include "platform/process.h"
#include "utils/container/Bitmap.h"
#include "utils/container/Tree.h"
#include "utils/types/common.h"

#include <stdio.h>
#include <stdlib.h>

static HuffmanTree *huffmanAlgorithm(ByteFrequency *f);
static void gencodes(HuffmanTree *root);

// Core
void compress(const char *inputfile, const char *outputfile, int opts) {
  ByteFrequency *freq = byteFreq_from_filename(inputfile);
  HuffmanTree *hufftree = huffmanAlgorithm(freq);

  CompWriter *writer = compWriter_from_huffmanTree(hufftree);
  compWriter_set_inputfile(writer, inputfile);
  compWriter_dump(writer, outputfile);

  const CompWriterStatistics statistics = compWriter_get_statistics(writer);
  log_info("compression finished: %s saved as %s!", inputfile, outputfile);
  log_info("from %zu bytes to %zu bytes (%.2f%% compressed)",
           statistics.uncompressed_size, statistics.compressed_size,
           100.0f * statistics.compression_rate);

  compWriter_destroy(writer);
  byteFreq_destroy(freq);
}

void decompress(const char *inputfile, const char *outputfile, int opts) {
  CompReader *reader = compReader_new(inputfile);
  CompReaderOutput output = compReader_translate(reader, outputfile);

  log_info("decompression finished: %s saved as %s!", inputfile, outputfile);
  log_info("from %zu bytes to %zu bytes", output.input_total_size_bytes,
           output.output_total_size_bytes);

  compReader_destroy(reader);
}

void inspect(const char *compfile, const char *outputfile, int opts) {
  CompReader *reader = compReader_new(compfile);
  CompHeader *original_header = compReader_get_header(reader);

  HuffmanTree *root = compHeader_get_huffmanTree(original_header);
  Bitmap *bm = compHeader_get_bitmap(original_header);

  if (opts & HUHMAN_HEADER) {
    CompHeader *reconstructed_header = compHeader_from_bitmap(bitmapClone(bm));
    HuffmanTree *reconstructed_tree =
        compHeader_get_huffmanTree(reconstructed_header);

    size_t hdr2_height = huffmanTree_get_height(reconstructed_tree);
    size_t hdr2_nodes = huffmanTree_get_count(reconstructed_tree);
    size_t hdr2_leafs = huffmanTree_get_leaf_count(reconstructed_tree);

    size_t hdr_minBits = compHeader_get_min_size_in_bits(original_header);
    size_t hdr_padBits = compHeader_get_padding_size_in_bits(original_header);
    size_t hdr_totalBytes = compHeader_get_total_size_in_bytes(original_header);

    size_t hdr2_minBits = compHeader_get_min_size_in_bits(reconstructed_header);
    size_t hdr2_padBits =
        compHeader_get_padding_size_in_bits(reconstructed_header);
    size_t hdr2_totalBytes =
        compHeader_get_total_size_in_bytes(reconstructed_header);

    printf("About the Huffman Tree:\n"
           "\tHeight: %zu\n"
           "\tNodes : %zu\n"
           "\tLeafs : %zu\n",
           hdr2_height, hdr2_nodes, hdr2_leafs);

    printf("About the header:\n"
           "\tTree size     (read/expected): %zu bits/%lu bits\n"
           "\tPadding added (read/expected): %zu bits/%lu bits\n"
           "\tTotal         (read/expected): %zu bytes/%lu bytes\n",
           hdr_minBits, hdr2_minBits, hdr_padBits, hdr2_padBits, hdr_totalBytes,
           hdr2_totalBytes);

    compHeader_destroy(reconstructed_header);
  }

  if (opts & HUHMAN_BODY) {
    CompReaderOutput output = compReader_translate(reader, outputfile);

    size_t body_minSize = output.output_min_size_bits;
    size_t body_padBits = output.output_pad_bits;
    size_t body_totalSize = output.output_total_size_bytes;

    printf("About the body:\n"
           "\tSize:          %zu bits\n"
           "\tPadding added: %zu bits\n"
           "\tTotal:         %zu bytes\n",
           body_minSize, body_padBits, body_totalSize);
  }

  if (opts & HUHMAN_CODES) {
    FILE *fpo = fopen(outputfile, "w");
    if (fpo) {
      huffmanTree_print_codes(root, fpo);
      printf("Tree codes: %s\n", outputfile);
      fclose(fpo);
    }
  }

  if (opts & HUHMAN_PDF) {
    FILE *fpo = fopen(outputfile, "w");
    if (fpo) {
      huffmanTree_print(root, fpo);
      printf("Tree dotfile: %s\n", outputfile);
      printf("Tip: run 'dot -Tpdf %s -o %s.pdf' and generate its pdf!\n",
             outputfile, outputfile);
      fclose(fpo);
    }
  }

  compHeader_destroy(original_header);
  compReader_destroy(reader);
}

static HuffmanTree *huffmanAlgorithm(ByteFrequency *f) {
  TreeList *lc = treelist_new();
  int bytes_set_size = byteFreq_get_set_size();
  for (int i = bytes_set_size - 1; i >= 0; i--) {
    unsigned ch = byteFreq_get_byte(f, i);
    unsigned count = byteFreq_get_count(f, i);
    if (count > 0) {
      lc = treelist_shift(lc, huffmanTree_new(ch, count, 0, 0));
    }
  }

#ifdef DEBUG
  log_debug("BEGIN - list to be processed");
  HuffmanTree *debug_tree;
  for (int i = 0; i < treelist_get_size(lc); i++) {
    debug_tree = treelist_get_tree(lc, i);
    log_debug("%u: %lu", huffmanTree_get_ASCII(debug_tree),
              huffmanTree_get_weight(debug_tree));
  }
  log_debug("END");
#endif

  HuffmanTree *ltree;
  HuffmanTree *rtree;
  unsigned long w1;
  unsigned long w2;
  for (size_t sz = treelist_get_size(lc); sz > 1; sz--) {
    ltree = treelist_unshift(lc);
    rtree = treelist_unshift(lc);
    w1 = huffmanTree_get_weight(ltree);
    w2 = huffmanTree_get_weight(rtree);
    lc = treelist_add_inorder(lc, huffmanTree_new(0, w1 + w2, ltree, rtree));
  }

  HuffmanTree *hufftree = treelist_unshift(lc);
  treelist_destroy(lc);
  huffmanTree_gen_treeCodes(hufftree);
  gencodes(hufftree);
  return hufftree;
}

// static and recursive
static void gencodes(HuffmanTree *root) {
  if (!root) {
    return;
  }
  // nodeCode is a byte-encoded number which takes the root tree to one of its
  // leaf nodes. For example: for nodeCode=101 (codeLen=3), then: leaf =
  // root->right->left->right for nodeCode=10001 (codeLen=5), then: leaf =
  // root->right->left->left->left->right
  //
  // Since nodeCode is a 64bit number, the maximum height of the huffmanTree
  // becomes 64.
  //
  // In the original project, nodeCode was built as a string in this
  // function, which allowed the generation of arbitrary code lengths.

  if (huffmanTree_is_leaf(root)) {
    TreeCode code = huffmanTree_get_code(root);
    Bitmap *bm = bitmapInit(code.len);
    for (int i = code.len; i > 0; i--) {
      bitmapAppendLeastSignificantBit(bm, (code.value >> (i - 1)) & 1);
    }
    huffmanTree_set_bitmap(root, bm);
  }
  gencodes(huffmanTree_get_left(root));
  gencodes(huffmanTree_get_right(root));
}
