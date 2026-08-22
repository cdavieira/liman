#include "core/liman.h"
#include "core/ByteFrequency.h"
#include "core/HuffmanTree.h"
#include "core/TreeList.h"
#include "core/fileformat/CompHeader.h"
#include "core/fileformat/CompReader.h"
#include "core/fileformat/CompWriter.h"
#include "platform/abort.h"
#include "platform/log.h"
#include "platform/mem.h"
#include "platform/unix/process.h"
#include "utils/container/Bitmap.h"
#include "utils/types/String.h"
#include "utils/types/common.h"

#include <stdio.h>
#include <stdlib.h>

static HuffmanTree *huffmanAlgorithm(ByteFrequency *f);
static void liman_print_header(CompHeader *original_header);
static void liman_print_body(CompReader *reader, const char *outputfile);
static void liman_print_codes(HuffmanTree *root, const char *outputfile);
static void liman_print_dot(HuffmanTree *root, const char *outputfile);
static int liman_run_dot(const char *dotfile);

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

  if (limanOpts_has_opt(opts, LIMAN_OPT_HEADER)) {
    liman_print_header(original_header);
  }

  if (limanOpts_has_opt(opts, LIMAN_OPT_BODY)) {
    liman_print_body(reader, outputfile);
  }

  if (limanOpts_has_opt(opts, LIMAN_OPT_CODES)) {
    liman_print_codes(root, outputfile);
  }

  if (limanOpts_has_opt(opts, LIMAN_OPT_PDF)) {
    liman_print_dot(root, outputfile);
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
  huffmanTree_gencodes(hufftree);
  return hufftree;
}

static void liman_print_header(CompHeader *original_header) {
  HuffmanTree *root = compHeader_get_huffmanTree(original_header);
  Bitmap *bm = compHeader_get_bitmap(original_header);

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

static void liman_print_body(CompReader *reader, const char *outputfile) {
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

static void liman_print_codes(HuffmanTree *root, const char *outputfile) {
  FILE *fpo = fopen(outputfile, "w");
  if (!fpo) {
    return;
  }

  huffmanTree_printfp_codes(root, fpo);
  printf("Tree codes: %s\n", outputfile);
  fclose(fpo);
}

static void liman_print_dot(HuffmanTree *root, const char *outputfile) {
  FILE *fpo = fopen(outputfile, "w");
  if (!fpo) {
    return;
  }

  huffmanTree_printfp_dot(root, fpo);
  fclose(fpo);

  printf("Tree dotfile: %s\n", outputfile);

  if (liman_run_dot(outputfile) == 0) {
    printf("Tree PDF: %s.pdf\n", outputfile);
  } else {
    printf("Tip: install 'dot' and run 'dot -Tpdf %s -o %s.pdf' and generate "
           "its pdf!\n",
           outputfile, outputfile);
  };
}

static int liman_run_dot(const char *dotfile) {
  char *dot_to_pdf_cmd[] = {"dot", "-T", "pdf", NULL, "-o", NULL, NULL};
  String *s = NULL;

  char *in_cmd = NULL;
  s = string_from_ptr(dotfile);
  in_cmd = string_drain(s);
  dot_to_pdf_cmd[3] = in_cmd;

  char *out_cmd = NULL;
  s = string_from_ptr(dotfile);
  string_append(s, ".pdf");
  out_cmd = string_drain(s);
  dot_to_pdf_cmd[5] = out_cmd;

  int res = process_dispatch_cmd(dot_to_pdf_cmd);

  mem_free(in_cmd);
  mem_free(out_cmd);

  return res == 0 ? 0 : -1;
}
