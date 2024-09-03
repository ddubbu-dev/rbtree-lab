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
  // TODO: roatate에서 사용할 수 있도록
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

void util_print_rbtree(node_t *node, node_t *nil, int depth) {
  if (node == nil) {
    return;
  }
  // 오른쪽 자식을 먼저 출력 (트리를 옆으로 눕혀서 출력하는 방식)
  util_print_rbtree(node->right, nil, depth + 1);
  for (int i = 0; i < depth; i++) {
    printf("    ");
  }
  printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");
  // 왼쪽 자식을 출력
  util_print_rbtree(node->left, nil, depth + 1);
}

int is_left_child(rbtree *t, node_t *x) {
  if (x != t->nil && x->parent != t->nil) {
    return x == x->parent->left ? 1 : 0;
  }
  return 0;  // TODO
}

// ✅ initialize struct if needed
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = NULL;
  p->root = p->nil;

  return p;
}

void util_left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil) {
    t->root = y;
  } else if (is_left_child(t, x)) {
    // 주의) 부모-자식 연결은 left,right 변경 불필요
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

int util_is_nil_node(rbtree *t, node_t *x) { return x == t->nil ? 1 : 0; }

// 문제: x가 root->parent 라서 nil, 이럴때는?

void util_right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;  // y 끊기

  // 부모: x, 자식: y-right
  x->left = y->right;
  if (x->right != t->nil) {
    y->right->parent = x;
  }

  // 부모: x-parent, 자식: y
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (is_left_child(t, x)) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  // 부모: y, 자식: x
  y->right = x;
  x->parent = y;
}

void util_rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z->parent != t->nil && z->parent->color == RBTREE_RED) {
    if (is_left_child(t, z->parent)) {
      node_t *y = z->parent->parent->right;
      if (y != t->nil && y->color == RBTREE_RED) {
        // [case1] z의 부모와 삼촌이 모두 레드인가?
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (!is_left_child(t, z)) {
          // [case2] 케이스3으로 전환
          printf("[case2]\n");
          util_print_rbtree(t->root, t->nil, 0);
          z = z->parent;
          util_left_rotate(t, z);
        }
        // [case3]
        printf("[case3]\n");
        util_print_rbtree(t->root, t->nil, 0);
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        // TODO: 여기부터 다시 살펴보기 흐음 rotate 주체가 어렵네
        util_right_rotate(t, z->parent->parent);
        printf("[util_right_rotate]\n");
        util_print_rbtree(t->root, t->nil, 0);
      }
    } else {
      node_t *y = z->parent->parent->left;
      if (y != t->nil && y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (z != t->nil && z == z->parent->left) {
          z = z->parent;
          util_right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        util_left_rotate(t, z->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

// 🏃‍♀️implement insert
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // BST insert
  node_t *z = util_create_node(t, RBTREE_RED, key);
  node_t *x = t->root;  // cur
  node_t *y = t->nil;   // parent

  while (x != t->nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }

  z->parent = y;
  if (y == t->nil) {
    t->root = z;
  } else if (z->key < y->key) {
    y->left = z;
  } else {
    y->right = z;
  }

  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  printf("==================\n[Insert]%d\n", key);
  printf("[before]\n");
  util_print_rbtree(t->root, t->nil, 0);
  printf("\n");

  util_rbtree_insert_fixup(t, z);
  printf("[after]\n");
  util_print_rbtree(t->root, t->nil, 0);
  printf("\n");

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

void in_order_traversal(node_t *node, node_t *nil, key_t *arr, size_t *index) {
  if (node == nil) {
    return;
  }
  // TODO: 포인터 변수인 이유 찾아보기
  in_order_traversal(node->left, nil, arr, index);
  arr[(*index)++] = node->key;
  in_order_traversal(node->right, nil, arr, index);
}

// ✅ implement to_array
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t index = 0;
  in_order_traversal(t->root, t->nil, arr, &index);
  return 0;
}