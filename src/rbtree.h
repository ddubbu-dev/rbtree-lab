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
node_pair_t util_find_target_node(rbtree *t, const key_t key);

// TODO
typedef enum { LEAF, LEFT_CHILD, RIGHT_CHILD } direction_t;
// 부모 자식 위치 전환
void transplant();
// 왼쪽회전
void left_rotate();
// 오른쪽 회전
void right_rotate();
void refixing_after_insert();
void refixing_after_delete();

#endif  // _RBTREE_H_
