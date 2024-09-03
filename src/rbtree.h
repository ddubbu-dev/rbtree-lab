#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil;  // for sentinel
} rbtree;

// DEFAULT
rbtree *new_rbtree(void);
void delete_rbtree(rbtree *);
node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
int rbtree_erase(rbtree *, node_t *);
int rbtree_to_array(const rbtree *, key_t *, const size_t);

// CUSTOM

typedef struct {
  node_t *parent;
  node_t *target_node;
} node_pair_t;

node_t *util_create_node(rbtree *t, const color_t color, const key_t key);
void util_link_parent_child_node(node_t *p, node_t *c);
void util_left_rotate(rbtree *t, node_t *x);
void util_right_rotate(rbtree *t, node_t *x);
void util_rbtree_insert_fixup(rbtree *t, node_t *z);
void util_rbtree_delete_fixup(rbtree *t, node_t *z);
// TODO: 모아서 rbtree.c 상단에 프로토타입 추가 필요함

#endif  // _RBTREE_H_
