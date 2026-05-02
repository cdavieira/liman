#include "core/HuffmanTree.h"
#include "core/ByteFrequency.h"
#include "core/HuffmanCode.h"
#include "platform/mem.h"
#include "utils/container/Tree.h"

#include <ctype.h>
#include <stdio.h>

HuffmanTree *huffmanTree_new(unsigned int key, unsigned long weight,
                             HuffmanTree *left, HuffmanTree *right) {
  return (HuffmanTree *)tree_new((void *)huffmanCode_new(key, weight),
                                 (Tree *)left, (Tree *)right);
}

HuffmanTree *huffmanTree_destroy(HuffmanTree *hftree) {
  return hftree ? (HuffmanTree *)tree_destroy((Tree *)hftree,
                                              (void *)huffmanCode_destroy)
                : 0;
}

size_t huffmanTree_get_id(HuffmanTree *node) {
  return tree_get_id((Tree *)node);
}

unsigned int huffmanTree_get_key(HuffmanTree *node) {
  return huffmanCode_get_key((HuffmanCode *)tree_get_item((Tree *)node));
}

unsigned long huffmanTree_get_weight(HuffmanTree *node) {
  return huffmanCode_get_weight((HuffmanCode *)tree_get_item((Tree *)node));
}

HuffmanTree *huffmanTree_get_left(HuffmanTree *hftree) {
  return (HuffmanTree *)tree_get_left((Tree *)hftree);
}

HuffmanTree *huffmanTree_get_right(HuffmanTree *hftree) {
  return (HuffmanTree *)tree_get_right((Tree *)hftree);
}

Code huffmanTree_get_code(HuffmanTree *hftree) {
  return huffmanCode_get_value(tree_get_item((Tree *)hftree));
}

HuffmanCode *huffmanTree_get_value(HuffmanTree *hftree) {
  return tree_get_item((Tree *)hftree);
}

HuffmanTree *huffmanTree_get_child(HuffmanTree *hftree, unsigned lr) {
  return (HuffmanTree *)tree_get_child((Tree *)hftree, lr);
}

HuffmanTree *huffmanTree_set_left(HuffmanTree *hftree, HuffmanTree *filho) {
  return (HuffmanTree *)tree_set_left((Tree *)hftree, (Tree *)filho);
}

HuffmanTree *huffmanTree_set_right(HuffmanTree *hftree, HuffmanTree *filho) {
  return (HuffmanTree *)tree_set_right((Tree *)hftree, (Tree *)filho);
}

unsigned huffmanTree_is_leaf(HuffmanTree *hftree) {
  return tree_is_leaf((Tree *)hftree);
}

unsigned long huffmanTree_get_count(HuffmanTree *hftree) {
  return tree_get_count((Tree *)hftree);
}

unsigned long huffmanTree_get_height(HuffmanTree *hftree) {
  return tree_get_height((Tree *)hftree);
}

unsigned long huffmanTree_get_leaf_count(HuffmanTree *hftree) {
  return tree_get_leaf_count((Tree *)hftree);
}

static void gencodes_callback(Tree *t, unsigned height, unsigned long path) {
  HuffmanCode *code = (HuffmanCode *)tree_get_item(t);
  if (!code) {
    return;
  }
  huffmanCode_set_value(code, height, path);
}

void huffmanTree_gencodes(HuffmanTree *root) {
  tree_visit_leafs((Tree *)root, gencodes_callback);
}

static unsigned huffman_compare_key(void *hufftree1, void *ascii) {
  return (huffmanTree_get_key((HuffmanTree *)hufftree1)) ==
         *((unsigned *)ascii);
}

HuffmanTree *huffmanTree_search_key(HuffmanTree *hftree, unsigned key) {
  return (HuffmanTree *)tree_search((Tree *)hftree, (void *)&key,
                                    huffman_compare_key);
}

static unsigned long huffmanTree_get_msg_size_rec(HuffmanTree *hftree,
                                                  unsigned long height) {
  if (!hftree) {
    return 0;
  }
  if (huffmanTree_is_leaf(hftree)) {
    return height * huffmanTree_get_weight(hftree);
  }
  return huffmanTree_get_msg_size_rec(huffmanTree_get_left(hftree),
                                      height + 1) +
         huffmanTree_get_msg_size_rec(huffmanTree_get_right(hftree),
                                      height + 1);
}

unsigned long huffmanTree_get_msg_size(HuffmanTree *hftree) {
  return huffmanTree_get_msg_size_rec(hftree, 0);
}

int huffmanTree_compare_freq(HuffmanTree *map1, HuffmanTree *map2) {
  return huffmanTree_get_weight(map1) > huffmanTree_get_weight(map2);
}

static void huffmanTree_printfp_labels(HuffmanTree *node, FILE *fp) {
  if (!node) {
    return;
  }

  size_t id = huffmanTree_get_id(node);
  unsigned int ch = huffmanTree_get_key(node);

  if (!huffmanTree_is_leaf(node)) {
    fprintf(fp, "    %lu [label=\"\"];\n", id);
    HuffmanTree *left = huffmanTree_get_left(node);
    HuffmanTree *right = huffmanTree_get_right(node);
    huffmanTree_printfp_labels(left, fp);
    huffmanTree_printfp_labels(right, fp);
    return;
  }

  HuffmanCode *code = huffmanTree_get_value(node);
  char *s = huffmanCode_stringify(code);
  if (isprint(ch)) {
    if (ch == '\\' || ch == '"') {
      fprintf(fp, "    %lu [label=\"'\\%c' %s\"];\n", id, ch, s);
    } else {
      fprintf(fp, "    %lu [label=\"'%c' %s\"];\n", id, ch, s);
    }
  } else {
    fprintf(fp, "    %lu [label=\"'%zu' %s\"];\n", id, id, s);
  }
  mem_free(s);
}

static void huffmanTree_printfp_arrows(HuffmanTree *node, FILE *fp) {
  if (!node) {
    return;
  }

  HuffmanTree *left = huffmanTree_get_left(node);
  HuffmanTree *right = huffmanTree_get_right(node);
  size_t id = huffmanTree_get_id(node);

  if (left) {
    size_t lid = huffmanTree_get_id(left);
    fprintf(fp, "    %lu -> %lu;\n", id, lid);
    huffmanTree_printfp_arrows(left, fp);
  }
  if (right) {
    size_t rid = huffmanTree_get_id(right);
    fprintf(fp, "    %lu -> %lu;\n", id, rid);
    huffmanTree_printfp_arrows(right, fp);
  }
}

void huffmanTree_printfp_dot(HuffmanTree *root, FILE *fp) {
  fprintf(fp, "digraph Tree {\n"
              "    node [fontname=\"Arial\"];\n");
  HuffmanTree *left = huffmanTree_get_left(root);
  HuffmanTree *right = huffmanTree_get_right(root);
  size_t id = huffmanTree_get_id(root);
  if (!root) {
    fprintf(fp, "\n");
  } else if (!left && !right) {
    fprintf(fp, "    %lu;\n", id);
  } else {
    huffmanTree_printfp_labels(root, fp);
    huffmanTree_printfp_arrows(root, fp);
  }
  fprintf(fp, "}\n");
}

void huffmanTree_printfp_codes(HuffmanTree *hufftree, FILE *fp) {
  HuffmanTree *t;
  char *const notfound = "not encoded";
  char *strcode;
  size_t setsize = byteFreq_get_set_size();
  ssize_t nodeid;
  HuffmanCode *code;
  int hasCode;
  for (unsigned ch = 0; ch < setsize; ch++) {
    t = huffmanTree_search_key(hufftree, ch);
    if (!t) {
      continue;
    }
    code = huffmanTree_get_value(t);
    hasCode = huffmanCode_has_value(code);
    strcode = hasCode ? huffmanCode_stringify(code) : notfound;
    nodeid = t ? huffmanTree_get_id(t) : -1;
    if (isprint(ch)) {
      fprintf(fp, "(Tree %03ld) %c: %s\n", nodeid, ch, strcode);
    } else {
      fprintf(fp, "(Tree %03ld) %u: %s\n", nodeid, ch, strcode);
    }
    if (hasCode) {
      mem_free(strcode);
    }
  }
}
