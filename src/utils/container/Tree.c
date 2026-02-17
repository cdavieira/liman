#include "utils/container/Tree.h"
#include "platform/mem.h"

#include <stdio.h>

struct Tree {
  size_t id;

  void *item;

  Tree *left;
  Tree *right;
};

Tree *tree_new(void *item, Tree *left, Tree *right) {
  static size_t id = 0;
  Tree *t = mem_alloc(sizeof(Tree));
  t->id = id++;
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

void tree_visit_leafs_r(Tree *root,
                        void (*callback)(Tree *leaf, unsigned height,
                                         unsigned long path),
                        unsigned height, unsigned long path) {
  if (!root) {
    return;
  }
  if (tree_is_leaf(root)) {
    callback(root, height, path);
  }
  tree_visit_leafs_r(root->left, callback, height + 1, (path << 1) | 0);
  tree_visit_leafs_r(root->right, callback, height + 1, (path << 1) | 1);
}

void tree_visit_leafs(Tree *root, void (*callback)(Tree *leaf, unsigned height,
                                                   unsigned long path)) {
  tree_visit_leafs_r(root, callback, 0, 0);
}
