#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

node_t *create_node(rbtree *t, const color_t color, const key_t key) {
  node_t *p = (node_t *)calloc(1, sizeof(node_t));
  p->color = color;
  p->key = key;
  p->parent = p->left = p->right = t->nil;

  return p;
}

typedef enum { LEAF, LEFT_CHILD, RIGHT_CHILD } direction_t;

void link_parent_child_node(node_t *p, node_t *c) {
  if (c->key < p->key) {
    p->left = c;
  } else {
    p->right = c;
  }
  c->parent = p;
}

node_t **find_target_node(rbtree *t, const key_t key) {
  /**
   * key 값을 가진 노드가
   * - 있다면: 해당 노드와 부모 전달
   * - 없다면: 최종 말단 노드(p), cur=NULL로 반환
   */
  node_t *p = t->nil;
  node_t *x = t->root;

  while (x != t->nil) {
    const key_t cur_key = x->key;
    if (cur_key == key) {
      break;
    }

    if (cur_key > key) {
      x = x->right;
    } else {
      x = x->left;
    }

    p = x;
  }

  node_t *result[2] = {p, x};

  return result;
}

// ✅ initialize struct if needed
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));

  p->root = p->nil;
  p->nil = NULL;

  return p;
}

//// fyi. test-rbtree.c 에서 디버그 모드 실행 안돼서 여기만의 main을 만듦
// void main() {
//   rbtree *t = new_rbtree();
//   node_t *p = rbtree_insert(t, 1024);
//   printf("p != NULL=%d\n", p != NULL);
//   printf("t->root == p=%d\n", t->root == p);
//   printf("p->key == key=%d\n", p->key == 1024);
//   printf("p->left == NULL=%d\n", p->left == NULL);
//   printf("p->right == NULL=%d\n", p->right == NULL);
//   printf("p->parent == NULL%d\n", p->parent == NULL);
// }

// 🏃‍♀️implement insert
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // BST insert
  node_t *new_node = create_node(t, RBTREE_RED, key);

  node_t *p = t->root;
  if (t->root == t->nil) {
    t->root = new_node;
    new_node->parent = t->nil;
    new_node->color = RBTREE_BLACK;
  } else {
    node_t **result = find_target_node(t, key);
    node_t *p = result[0];

    link_parent_child_node(p, new_node);
  }

  // TODO: REFIXING
  return t->root;
}

// 🏃‍♀️implement find
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t **result = find_target_node(t, key);
  node_t *x = result[1];
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
