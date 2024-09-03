
/**
 * [Notice]
 * - Sentinel Node 버전으로 구현함
 * - 과제 수행에 필요한 util 함수 프로토타입 리스트업
 * - 참고 자료 : 'Introduction To Algorithms' 의사코드 & 블로그
 */

#include "rbtree.h"

#include <stdlib.h>

node_t *util_create_node(rbtree *t, const key_t key);
int util_is_nill(const rbtree *t, node_t *x);
int util_is_left_child(const rbtree *t, node_t *x);
int util_is_right_child(const rbtree *t, node_t *x);
void util_delete_node(rbtree *t, node_t *root);
void util_left_rotate(rbtree *t, node_t *x);
void util_right_rotate(rbtree *t, node_t *x);
void util_insert_fixup(rbtree *t, node_t *z);
node_t *util_subtree_min(const rbtree *t, node_t *sub_root);
void util_transplant(rbtree *t, node_t *u, node_t *v);
void util_link_parent_and_child(rbtree *t, node_t *p, node_t *c);
void util_delete_fixup(rbtree *t, node_t *x);
void util_inorder_traversal(node_t *node, node_t *nil, key_t *arr,
                            size_t *index);

rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  t->nil = (node_t *)calloc(1, sizeof(node_t));
  t->nil->color = RBTREE_BLACK;
  t->root = t->nil;
  return t;
}

node_t *rbtree_insert(rbtree *t, const key_t target_key) {
  node_t *new_node = util_create_node(t, target_key);
  node_t *cur = t->root;
  node_t *p = t->nil;

  while (!util_is_nill(t, cur)) {
    p = cur;
    cur = target_key < cur->key ? cur->left : cur->right;
  }

  util_link_parent_and_child(t, p, new_node);
  util_insert_fixup(t, new_node);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t target_key) {
  node_t *cur = t->root;
  while (!util_is_nill(t, cur)) {
    if (cur->key == target_key) return cur;
    cur = target_key < cur->key ? cur->left : cur->right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *cur = t->root;
  if (util_is_nill(t, cur)) return cur;
  while (!util_is_nill(t, cur->left)) {
    cur = cur->left;
  }
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *cur = t->root;
  if (util_is_nill(t, cur)) return cur;
  while (!util_is_nill(t, cur->right)) {
    cur = cur->right;
  }
  return cur;
}

void delete_rbtree(rbtree *t) {
  if (t == NULL) {
    return;
  }
  util_delete_node(t, t->root);
  free(t->nil);
  free(t);
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;
  color_t y_orginal_color = y->color;
  node_t *x;
  if (util_is_nill(t, z->left)) {
    x = z->right;
    util_transplant(t, z, z->right);
  } else if (util_is_nill(t, z->right)) {
    x = z->left;
    util_transplant(t, z, z->left);
  } else {
    y = util_subtree_min(t, z->right);
    y_orginal_color = y->color;
    x = y->right;

    if (y != z->right) {
      util_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    } else {
      x->parent = y;
    }

    util_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_orginal_color == RBTREE_BLACK) {
    util_delete_fixup(t, x);
  }
  free(z);
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t index = 0;
  util_inorder_traversal(t->root, t->nil, arr,
                         &index);  // TODO: 주소값으로 넘겨야하는 이유
  return 0;
}

// ==================== UTILS ====================
node_t *util_create_node(rbtree *t, const key_t key) {
  node_t *p = (node_t *)calloc(1, sizeof(node_t));
  p->key = key;
  p->color = RBTREE_RED;
  p->parent = p->left = p->right = t->nil;
  return p;
}

int util_is_nill(const rbtree *t, node_t *x) { return x == t->nil ? 1 : 0; }

int util_is_left_child(const rbtree *t, node_t *x) {
  return x->parent->left == x ? 1 : 0;
}

int util_is_right_child(const rbtree *t, node_t *x) {
  return x->parent->right == x ? 1 : 0;
}

void util_delete_node(rbtree *t, node_t *root) {
  if (util_is_nill(t, root)) {
    return;
  }
  util_delete_node(t, root->left);
  util_delete_node(t, root->right);
  free(root);
}

void util_left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (!util_is_nill(t, y->left)) {
    y->left->parent = x;
  }
  util_link_parent_and_child(t, x->parent, y);

  // TODO: 왜 연결 안될까?
  // util_link_parent_and_child(t, y, x);
  y->left = x;
  x->parent = y;
  return;
}

void util_right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (!util_is_nill(t, y->right)) {
    y->right->parent = x;
  }
  util_link_parent_and_child(t, x->parent, y);

  y->right = x;
  x->parent = y;
  return;
}

void util_insert_fixup(rbtree *t, node_t *z) {
  node_t *uncle;

  while (z->parent->color == RBTREE_RED) {
    if (util_is_left_child(t, z->parent)) {
      uncle = z->parent->parent->right;
      if (uncle->color == RBTREE_RED) {
        // [case1]
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (util_is_right_child(t, z)) {
          // [case2]
          z = z->parent;
          util_left_rotate(t, z);
        }
        // [case3]
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        util_right_rotate(t, z->parent->parent);
      }
    } else {
      uncle = z->parent->parent->left;
      if (uncle->color == RBTREE_RED) {
        // [case1]
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
        if (util_is_left_child(t, z)) {
          // [case2]
          z = z->parent;
          util_right_rotate(t, z);
        }
        if (z != t->root && z->parent != t->root) {
          // [case3]
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          util_left_rotate(t, z->parent->parent);
        }
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *util_subtree_min(const rbtree *t, node_t *sub_root) {
  node_t *cur = sub_root;
  if (util_is_nill(t, cur)) return cur;
  while (!util_is_nill(t, cur->left)) cur = cur->left;
  return cur;
}

void util_transplant(rbtree *t, node_t *u, node_t *v) {
  /**
   * [명세] u를 끊고, u.p - v 신규 부모 자식으로 연결
   *  u.p
   *    \
   *     u
   *      \
   *       v
   */

  if (util_is_nill(t, u->parent))
    t->root = v;

  else if (util_is_left_child(t, u))
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
  return;
}

void util_link_parent_and_child(rbtree *t, node_t *p, node_t *c) {
  c->parent = p;
  if (util_is_nill(t, p)) {
    t->root = c;
  } else if (c->key < p->key) {
    p->left = c;
  } else {
    p->right = c;
  }
}

void util_delete_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while ((x != t->root) && (x->color == RBTREE_BLACK)) {
    if (util_is_left_child(t, x)) {
      // [case1]
      w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        util_left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        // [case2]
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          // [case3]
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          util_right_rotate(t, w);
          w = x->parent->right;
        }
        // [case4]
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        util_left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
      // [case1]
      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        util_right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        // [case2]
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          // [case3]
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          util_left_rotate(t, w);
          w = x->parent->left;
        }
        // [case4]
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        util_right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

void util_inorder_traversal(node_t *node, node_t *nil, key_t *arr,
                            size_t *index) {
  if (node == nil) {
    return;
  }
  // TODO: 포인터 변수인 이유 찾아보기
  util_inorder_traversal(node->left, nil, arr, index);
  arr[(*index)++] = node->key;
  util_inorder_traversal(node->right, nil, arr, index);
}
