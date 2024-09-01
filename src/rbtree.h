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
node_t *create_node(rbtree *t, const color_t color, const key_t key);
// 부모와 자식 연결
void link_parent_child_node(node_t *p, node_t *x);
// 삽입, 삭제, 순회 등 연산이 필요한 노드까지 도달하는 이진탐색
node_t **find_target_node(rbtree *t, const key_t key);

// TODO
// 부모 자식 위치 전환
void transplant();
// 왼쪽회전
void left_rotate();
// 오른쪽 회전
void right_rotate();
void refixing_after_insert();
void refixing_after_delete();

#endif  // _RBTREE_H_
