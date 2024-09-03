#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>

void util_print_rbtree(node_t *node, node_t *nil, int depth) {
  return;  // TODO: print 불필요

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

node_t *util_create_node(rbtree *t, const key_t key) {
  node_t *p = (node_t *)calloc(1, sizeof(node_t));
  p->key = key;
  p->color = RBTREE_RED;
  p->parent = p->left = p->right = t->nil;

  return p;
}

int util_is_nil(rbtree *t, node_t *x) { return x == t->nil ? 1 : 0; }

int util_is_left_child(rbtree *t, node_t *x) {
  return x == x->parent->left ? 1 : 0;
}

int util_is_right_child(rbtree *t, node_t *x) {
  return x == x->parent->right ? 1 : 0;
}

// ✅ initialize struct if needed
rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  t->nil = (node_t *)calloc(1, sizeof(node_t));
  t->nil->color = RBTREE_BLACK;
  t->root = t->nil;
  return t;
}

void util_left_rotate(rbtree *t, node_t *x) {
  node_t *p = x->parent;
  node_t *gp = p->parent;
  node_t *tmp = x->left;

  if (p == t->root) {
    t->root = x;
  } else {
    if (util_is_left_child(t, p)) {
      gp->left = x;
    } else {
      gp->right = x;
    }
  }

  x->parent = gp;
  p->parent = x;
  x->left = p;
  p->right = tmp;
  tmp->parent = p;
}

void util_right_rotate(rbtree *t, node_t *x) {
  node_t *p = x->parent;
  node_t *gp = p->parent;
  node_t *tmp = x->right;

  if (p == t->root) {
    t->root = x;
  } else {
    if (util_is_left_child(t, p)) {
      gp->left = x;
    } else {
      gp->right = x;
    }
  }

  x->parent = gp;
  p->parent = x;
  x->right = p;
  tmp->parent = p;
  p->left = tmp;
}

void util_exchange_color(node_t *p, node_t *x) {
  color_t tmp = x->color;

  x->color = p->color;
  p->color = tmp;
}

void util_rbtree_insert_fixup(rbtree *t, node_t *x) {
  node_t *p = x->parent;

  // 예외 처리
  if (x == t->root) {
    x->color = RBTREE_BLACK;
    return;
  }

  if (p->color == RBTREE_BLACK) {
    return;
  }

  node_t *gp = p->parent;
  node_t *u = util_is_left_child(t, p) ? gp->right : gp->left;

  // [case1]
  if (u->color == RBTREE_RED) {
    p->color = RBTREE_BLACK;
    u->color = RBTREE_BLACK;
    gp->color = RBTREE_RED;
    util_rbtree_insert_fixup(t, gp);
    return;
  }

  if (util_is_left_child(t, p)) {
    // if (util_is_right_child(t, x)) {
    //   // [case2]
    //   x = p;
    //   util_left_rotate(t, x);
    // }

    // // [case3]
    // util_exchange_color(p, gp);
    // util_right_rotate(t, gp);
    // return;

    if (util_is_left_child(t, x)) {
      util_right_rotate(t, p);
      util_exchange_color(p, p->right);
      return;
    }

    util_left_rotate(t, x);
    util_right_rotate(t, x);
    util_exchange_color(x, x->right);
    return;
  }

  // if (util_is_left_child(t, x)) {
  //   // [case2]
  //   x = p;
  //   util_right_rotate(t, x);
  // }

  // // [case3]
  // util_exchange_color(p, gp);
  // util_right_rotate(t, gp);

  if (util_is_left_child(t, x)) {
    util_right_rotate(t, x);
    util_left_rotate(t, x);
    util_exchange_color(x, x->left);
    return;
  }

  util_left_rotate(t, p);
  util_exchange_color(p, p->left);
}

// ✅ implement insert
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // BST insert
  node_t *new_node = util_create_node(t, key);
  node_t *cur = t->root;

  while (!util_is_nil(t, cur)) {
    if (key < cur->key) {
      if (util_is_nil(t, cur->left)) {
        cur->left = new_node;
        break;
      }
      cur = cur->left;
    } else {
      if (util_is_nil(t, cur->right)) {
        cur->right = new_node;
        break;
      }
      cur = cur->right;
    }
  }

  new_node->parent = cur;

  if (util_is_nil(t, cur)) {
    t->root = new_node;
  }

  // printf("==================\n[Insert]%d\n", key);
  // printf("[before]\n");
  // util_print_rbtree(t->root, t->nil, 0);
  // printf("\n");

  util_rbtree_insert_fixup(t, new_node);
  // printf("[after]\n");
  // util_print_rbtree(t->root, t->nil, 0);
  // printf("\n");

  return new_node;
}

// ✅ implement find
node_t *rbtree_find(const rbtree *t, const key_t target_key) {
  node_t *cur = t->root;
  while (!util_is_nil(t, cur)) {
    const key_t cur_key = cur->key;
    if (cur_key == target_key) {
      return cur;
    }

    if (cur_key < target_key) {
      cur = cur->right;
    } else {
      cur = cur->left;
    }
  }
  return NULL;
}

// ✅ implement find
node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;

  while (!util_is_nil(t, x->left)) {
    x = x->left;
  }

  return x;
}

// ✅ implement find
node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;

  while (!util_is_nil(t, x->right)) {
    x = x->right;
  }

  return x;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void util_rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (util_is_nil(t, u->parent)) {
    t->root = v;
  } else if (util_is_left_child(t, u)) {
    u->parent->left = v;
  } else {
    u->parent->right = v;
  }

  if (!util_is_nil(t, v)) {  // TODO: 의사코드랑 다른 부분 이해 필요
    v->parent = u->parent;
  }
}

node_t *util_find_min_node(rbtree *t, node_t *x) {
  while (!util_is_nil(t, x->left)) {
    x = x->left;
  }

  return x;
}
node_t *util_find_max_node(rbtree *t, node_t *x) {
  while (!util_is_nil(t, x->right)) {
    x = x->right;
  }

  return x;
}

void util_rbtree_delete_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while (x != t->root && !util_is_nil(t, x) && x->color == RBTREE_BLACK) {
    if (util_is_left_child(t, x)) {
      w = x->parent->right;
      // [case1]
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        util_left_rotate(t, x->parent);
        w = x->parent->right;
      }
      // [case2]
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }
      // TODO: Q. 왜 여긴 if/else? (case 1<>2 라고 치더라도)
      else {
        // [case3]
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          util_right_rotate(t, w);
          w = x->parent->right;
        }

        // [case4]
        w->color = x->parent->color;  // TODO: Q. 왜 ... 붙음?
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        util_left_rotate(t, x->parent);
        x = t->root;
      }
      continue;
    }
    // right
    w = x->parent->left;
    if (w->color == RBTREE_RED) {
      w->color = RBTREE_BLACK;
      x->parent->color = RBTREE_RED;
      util_right_rotate(t, x->parent);
      w = x->parent->left;
    }
    if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
      w->color = RBTREE_RED;
      x = x->parent;
    } else {
      if (w->left->color == RBTREE_BLACK) {
        w->right->color = RBTREE_BLACK;
        w->color = RBTREE_RED;
        util_left_rotate(t, w);
        w = x->parent->left;
      }

      w->color = x->parent->color;
      x->parent->color = RBTREE_BLACK;
      w->left->color = RBTREE_BLACK;
      util_right_rotate(t, x->parent);
      x = t->root;
    }
  }

  // TODO: Q. 왜? nil 값인 경우는 없나?
  // x->color = RBTREE_BLACK;
}

// ✅ implement erase
int rbtree_erase(rbtree *t, node_t *z) {
  node_t *x;
  node_t *y = z;
  color_t y_original_color = y->color;

  // BST Delete
  if (util_is_nil(t, z->left)) {
    x = z->right;
    util_rbtree_transplant(t, z, z->right);
  } else if (util_is_nil(t, z->right)) {
    x = z->left;
    util_rbtree_transplant(t, z, z->left);
  } else {
    y = util_find_min_node(t, z->right);
    y_original_color = y->color;
    x = y->right;

    if (y->parent != z) {  // TODO: 확인 필요
      util_rbtree_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    } else {
      if (util_is_nil(t, x)) {  // TODO: 확인 필요
        x->parent = y;
      }
    }

    util_rbtree_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }

  if (y_original_color == RBTREE_BLACK) {
    util_rbtree_delete_fixup(t, x);
  }
  // printf("==================\n[Delete]%d", z->key);
  util_print_rbtree(t->root, t->nil, 0);

  return 0;
}

void util_inorder_traversal(node_t *node, node_t *nil, key_t *arr,
                            size_t *index) {
  if (node == nil) {
    return;
  }
  // TODO: Q.포인터 변수인 이유 찾아보기
  util_inorder_traversal(node->left, nil, arr, index);
  arr[(*index)++] = node->key;
  util_inorder_traversal(node->right, nil, arr, index);
}

// ✅ implement to_array
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t index = 0;
  util_inorder_traversal(t->root, t->nil, arr, &index);
  return 0;
}
