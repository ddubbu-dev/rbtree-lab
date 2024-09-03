#include "rbtree.h"

#include <stdlib.h>

int util_is_nill(const rbtree *t, node_t *x);
int util_is_left_child(const rbtree *t, node_t *x);
int util_is_right_child(const rbtree *t, node_t *x);
void util_delete_node(rbtree *t, node_t *root);
void util_left_rotate(rbtree *t, node_t *x);
void util_right_rotate(rbtree *t, node_t *x);
void util_insert_fixup(rbtree *t, node_t *z);
node_t *util_subtree_min(const rbtree *t, node_t *sub_root);
void util_transplant(rbtree *t, node_t *u, node_t *v);
void util_delete_fixup(rbtree *t, node_t *x);
void util_inorder_traversal(node_t *node, node_t *nil, key_t *arr,
                            size_t *index);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  p->nil = NIL;
  p->root = NIL;
  NIL->color = RBTREE_BLACK;
  return p;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *parent = t->nil;
  node_t *p = t->root;
  while (!util_is_nill(t, p)) {
    parent = p;
    if (p->key > key) {
      p = p->left;
    } else {
      p = p->right;
    }
  }
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));
  new_node->parent = parent;
  // 트리의 첫 노드일 때 - 루트가 된다
  if (util_is_nill(t, parent)) {
    t->root = new_node;
    new_node->color = RBTREE_BLACK;
  }
  // 찾은 부모노드보다 새로운 노드가 작을때 - 왼쪽 자식으로
  else if (key < parent->key) {
    parent->left = new_node;
    new_node->color = RBTREE_RED;
  }
  // 크거나 같을때 - 오른쪽 자식으로
  else {
    parent->right = new_node;
    new_node->color = RBTREE_RED;
  }
  // 새로운 노드 설정
  new_node->key = key;
  new_node->left = t->nil;
  new_node->right = t->nil;

  util_insert_fixup(t, new_node);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *r = t->root;
  while (!util_is_nill(t, r)) {
    if (r->key == key)
      return r;
    else if (r->key < key)
      r = r->right;
    else
      r = r->left;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *r = t->root;
  if (util_is_nill(t, r)) return r;
  while (!util_is_nill(t, r->left)) {
    r = r->left;
  }
  return r;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *r = t->root;
  if (util_is_nill(t, r)) return r;
  while (!util_is_nill(t, r->right)) {
    r = r->right;
  }
  return r;
}

// reclaim the tree nodes's memory
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
    if (y->parent == z) {
      x->parent = y;
    } else {
      util_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
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
  util_inorder_traversal(t->root, t->nil, arr, &index);
  return 0;
}

// ==================== UTILS ====================
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
  node_t *y;
  y = x->right;
  x->right = y->left;
  if (!util_is_nill(t, y->left)) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (util_is_nill(t, x->parent)) {
    t->root = y;
  } else if (util_is_left_child(t, x)) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
  return;
}

void util_right_rotate(rbtree *t, node_t *x) {
  node_t *y;
  y = x->left;
  x->left = y->right;
  if (!util_is_nill(t, y->right)) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (util_is_nill(t, x->parent)) {
    t->root = y;
  } else if (util_is_right_child(t, x)) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
  return;
}

void util_insert_fixup(rbtree *t, node_t *z) {
  node_t *uncle;

  while (z->parent->color == RBTREE_RED) {
    if (util_is_left_child(t, z->parent)) {
      uncle = z->parent->parent->right;
      // 경우1
      if (uncle->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // 경우2
      else {
        if (util_is_right_child(t, z)) {
          z = z->parent;
          util_left_rotate(t, z);
        }
        // 경우3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        util_right_rotate(t, z->parent->parent);
      }
    }
    // 반대로
    else {
      uncle = z->parent->parent->left;
      // 경우1
      if (!util_is_nill(t, uncle) && uncle->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // 경우2
      else {
        if (util_is_left_child(t, z)) {
          z = z->parent;
          util_right_rotate(t, z);
        }
        // 경우3
        if (z != t->root && z->parent != t->root) {
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
  node_t *r = sub_root;
  if (util_is_nill(t, r)) return r;
  while (!util_is_nill(t, r->left)) {
    r = r->left;
  }
  return r;
}

void util_transplant(rbtree *t, node_t *u, node_t *v) {
  if (util_is_nill(t, u->parent)) {
    t->root = v;
  } else if (util_is_left_child(t, u)) {
    u->parent->left = v;
  } else
    u->parent->right = v;
  v->parent = u->parent;
  return;
}

void util_delete_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while ((x != t->root) && (x->color == RBTREE_BLACK)) {
    if (util_is_left_child(t, x)) {
      w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        util_left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          util_right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        util_left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
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
