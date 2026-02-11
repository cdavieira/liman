#include "utils/container/Tree.h"
#include "platform/mem.h"
#include "utils/bits.h"

#include <stdio.h>

struct Tree {
  size_t id;

  TreeCode code;

  void *item;

  Tree *left;
  Tree *right;
};

Tree *tree_new(void *item, Tree *left, Tree *right) {
  static size_t id = 0;
  Tree *t = mem_alloc(sizeof(Tree));
  t->id = id++;
  t->code = treeCode_init();
  t->item = item;
  t->left = left;
  t->right = right;
  return t;
}

Tree *tree_destroy(Tree *t, void *(*free_item)(void *)) {
  if (!t) {
    return NULL;
  }
  t->left = tree_destroy(t->left, free_item);
  t->right = tree_destroy(t->right, free_item);
  if (free_item) {
    t->item = free_item(t->item);
  }
  mem_free(t);
  return NULL;
}

void *tree_get_item(Tree *t) { return t->item; }

size_t tree_get_id(Tree *t) { return t->id; }

Tree *tree_get_left(Tree *t) { return t ? t->left : NULL; }

Tree *tree_get_child(Tree *root, unsigned lr) {
  switch (lr) {
  case 0:
    return root->left;
  case 1:
    return root->right;
  default:
    return NULL;
  }
  return NULL;
}

TreeCode tree_get_code(Tree *root) {
  return root ? root->code : treeCode_init();
}

Tree *tree_get_right(Tree *t) { return t ? t->right : NULL; }

Tree *tree_set_item(Tree *t, void *item) {
  t->item = item;
  return t;
}

Tree *tree_set_left(Tree *root, Tree *t) {
  root->left = t;
  return root;
}

Tree *tree_set_right(Tree *root, Tree *t) {
  root->right = t;
  return root;
}

static void tree_print_rec(Tree *node, FILE *fp) {
  if (!node) {
    return;
  }

  if (node->left) {
    fprintf(fp, "    %lu -> %lu;\n", node->id, node->left->id);
    tree_print_rec(node->left, fp);
  }
  if (node->right) {
    fprintf(fp, "    %lu -> %lu;\n", node->id, node->right->id);
    tree_print_rec(node->right, fp);
  }
}

void tree_print(Tree *root, FILE *fp) {
  fprintf(fp, "digraph Tree {\n"
              "    node [fontname=\"Arial\"];\n");
  if (!root) {
    fprintf(fp, "\n");
  } else if (!root->left && !root->right) {
    fprintf(fp, "    %lu;\n", root->id);
  } else {
    tree_print_rec(root, fp);
  }
  fprintf(fp, "}\n");
}

unsigned tree_is_leaf(Tree *t) { return t ? !t->right && !t->left : 0; }

Tree *tree_search(Tree *t, void *search, unsigned (*fcmp)(void *, void *)) {
  if (!tree_is_leaf(t)) {
    Tree *ltree = tree_search(t->left, search, fcmp);
    return ltree ? ltree : tree_search(t->right, search, fcmp);
  }
  return fcmp(t, search) ? t : NULL;
}

unsigned long tree_get_count(Tree *t) {
  if (!t) {
    return 0;
  }
  return 1 + tree_get_count(t->left) + tree_get_count(t->right);
}

unsigned long tree_get_leaf_count(Tree *t) {
  if (!t) {
    return 0;
  }
  return tree_is_leaf(t) + tree_get_leaf_count(t->left) +
         tree_get_leaf_count(t->right);
}

unsigned long tree_get_height(Tree *t) {
  if (!t) {
    return 0;
  }
  unsigned long lh = tree_get_height(t->left);
  unsigned long rh = tree_get_height(t->right);
  return !tree_is_leaf(t) + (lh > rh ? lh : rh);
}

unsigned tree_exists(Tree *t, Tree *node) {
  if (!t) {
    return 0;
  }
  return t == node ? 1
                   : tree_exists(t->right, node) || tree_exists(t->left, node);
}

Tree *tree_descend(Tree *node, const TreeCode *code) {
  unsigned char bit;
  for (int i = code->len - 1; node && i >= 0; i--) {
    bit = bits_get_size_bit(code->value, i);
    if (bit == 0) {
      node = node->left;
    } else {
      node = node->right;
    }
  }
  return node;
}

void tree_gen_treeCodes(Tree *root) {
  if (!root) {
    return;
  }

  if (root->left) {
    root->left->code = root->code;
    treeCode_left(&root->left->code);
  }

  if (root->right) {
    root->right->code = root->code;
    treeCode_right(&root->right->code);
  }

  tree_gen_treeCodes(root->left);
  tree_gen_treeCodes(root->right);
}

/**/

TreeCode treeCode_init(void) { return (TreeCode){0, 0}; }

void treeCode_left(TreeCode *code) {
  code->value = (code->value << 1) | 0;
  code->len++;
}

void treeCode_right(TreeCode *code) {
  code->value = (code->value << 1) | 1;
  code->len++;
}

char *treeCode_to_cstr(const TreeCode *code) {
  char *route = mem_zalloc((code->len + 1) * sizeof(char));
  for (int i = code->len - 1, j = 0; i >= 0; i--, j++) {
    route[j] = '0' + bits_get_size_bit(code->value, i);
  }
  route[code->len] = '\0';
  return route;
}
