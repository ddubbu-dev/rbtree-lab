#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  p->nil = NIL;
  p->root = NIL;
  NIL->color = RBTREE_BLACK;
  return p;
}

void util_delete_node(rbtree *t, node_t *root) {
  if (root == t->nil) {
    return;
  }
  util_delete_node(t, root->left);
  util_delete_node(t, root->right);
  free(root);
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

void left_rotate(rbtree *t, node_t *x) {
  node_t *y;
  y = x->right;
  x->right = y->left;
  if (y->left != t->nil) {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
  return;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y;
  y = x->left;
  x->left = y->right;
  if (y->right != t->nil) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  } else if (x == x->parent->right) {
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
  return;
}

void rb_insert_Fixup(rbtree *t, node_t *z) {
  node_t *uncle;
  // while ((z != t->root) && (z->color != RBTREE_BLACK) && (z->parent->color ==
  // RBTREE_RED))
  while (z->parent->color == RBTREE_RED) {
    if (z->parent == z->parent->parent->left) {
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
        if (z == z->parent->right) {
          z = z->parent;
          left_rotate(t, z);
        }
        // 경우3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t, z->parent->parent);
      }
    }
    // 반대로
    else {
      uncle = z->parent->parent->left;
      // 경우1
      if (uncle != t->nil && uncle->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      // 경우2
      else {
        if (z == z->parent->left) {
          z = z->parent;
          right_rotate(t, z);
        }
        // 경우3
        if (z != t->root && z->parent != t->root) {
          z->parent->color = RBTREE_BLACK;
          z->parent->parent->color = RBTREE_RED;
          left_rotate(t, z->parent->parent);
        }
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *parent = t->nil;
  node_t *p = t->root;
  while (p != t->nil) {
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
  if (parent == t->nil) {
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

  rb_insert_Fixup(t, new_node);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *r = t->root;
  while (r != t->nil) {
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
  // TODO: implement find
  node_t *r = t->root;
  if (r == t->nil) return r;
  while (r->left != t->nil) {
    r = r->left;
  }
  return r;
}

node_t *tree_minimum(const rbtree *t, node_t *sub_root) {
  // TODO: implement find
  node_t *r = sub_root;
  if (r == t->nil) return r;
  while (r->left != t->nil) {
    r = r->left;
  }
  return r;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *r = t->root;
  if (r == t->nil) return r;
  while (r->right != t->nil) {
    r = r->right;
  }
  return r;
}

void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  } else if (u == u->parent->left) {
    u->parent->left = v;
  } else
    u->parent->right = v;
  v->parent = u->parent;
  return;
}

void rb_delete_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while ((x != t->root) && (x->color == RBTREE_BLACK)) {
    if (x == x->parent->left) {
      w = x->parent->right;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;
  color_t y_orginal_color = y->color;
  node_t *x;
  if (z->left == t->nil) {
    x = z->right;
    rb_transplant(t, z, z->right);
  } else if (z->right == t->nil) {
    x = z->left;
    rb_transplant(t, z, z->left);
  } else {
    y = tree_minimum(t, z->right);
    y_orginal_color = y->color;
    x = y->right;
    if (y->parent == z) {
      x->parent = y;
    } else {
      rb_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rb_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_orginal_color == RBTREE_BLACK) {
    rb_delete_fixup(t, x);
  }
  free(z);
  return 0;
}

int inorder_rbtree(node_t *root, key_t *res, const rbtree *t, int i) {
  if (root == t->nil) {
    return i;
  }

  i = inorder_rbtree(root->left, res, t, i);
  res[i] = root->key;
  i += 1;
  i = inorder_rbtree(root->right, res, t, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  if (t->root == t->nil) {
    return -1;
  }
  inorder_rbtree(t->root, arr, t, 0);
  return 0;
}