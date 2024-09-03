#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

node_t *util_create_node(rbtree *t, const color_t color, const key_t key) {
  node_t *p = (node_t *)calloc(1, sizeof(node_t));
  p->color = color;
  p->key = key;
  p->parent = p->left = p->right = t->nil;

  return p;
}

void util_link_parent_child_node(node_t *p, node_t *c) {
  if (c->key < p->key) {
    p->left = c;
  } else {
    p->right = c;
  }
  c->parent = p;
}

node_pair_t util_find_target_node(rbtree *t, const key_t key) {
  /**
   * key 값을 가진 노드가
   * - 있다면: 해당 노드(x)와 부모(p) 반환
   * - 없다면: x=NULL, 최종 말단 노드(p) 반환
   */

  node_t *p = t->nil;
  node_t *x = t->root;

  while (x != t->nil) {
    p = x;

    const key_t cur_key = x->key;
    if (cur_key == key) {
      break;
    }

    if (cur_key > key) {
      x = x->right;
    } else {
      x = x->left;
    }
  }

  node_pair_t result;
  result.parent = p;
  result.target_node = x;

  return result;
}

// ✅ initialize struct if needed
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  p->nil = NULL;
  p->root = p->nil;

  return p;
}

// 🏃‍♀️implement insert
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // BST insert
  node_t *new_node = util_create_node(t, RBTREE_RED, key);

  node_t *p = t->root;
  if (t->root == t->nil) {
    t->root = new_node;
    new_node->parent = t->nil;
    new_node->color = RBTREE_BLACK;
  } else {
    node_pair_t pair = util_find_target_node(t, key);
    util_link_parent_child_node(pair.parent, new_node);
  }

  // TODO: REFIXING
  return t->root;
}

// ✅ implement find
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_pair_t pair = util_find_target_node(t, key);
  return pair.target_node;
}

// 🏃‍♀️implement find
node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;

  while (x && x->left != t->nil) {
    x = x->left;
  }

  return x;
}

// 🏃‍♀️implement find
node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;

  while (x && x->right != t->nil) {
    x = x->right;
  }

  return x;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// 🏃‍♀️implement erase
int rbtree_erase(rbtree *t, node_t *p) {
  node_pair_t pair = util_find_target_node(t, p->key);

  // TODO: find p->right min 그리고 successor 반영
  // TODO: REFIXING
}

// 🏃‍♀️implement to_array
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  for (int i = 0; i < n; i++) {
    rbtree_insert(t, arr[i]);
  }

  return 0;
}

void main() {
  rbtree *t = new_rbtree();

  key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12};
  const size_t n = sizeof(entries) / sizeof(entries[0]);
  rbtree_to_array(t, entries, n);

  // node_t *q = rbtree_max(t);
  // printf("(q != NULL) ? %d", q != NULL);
  // printf("(q->key == entries[n - 1]) ? %d", q->key == entries[n - 1]);
}