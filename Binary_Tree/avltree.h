/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef AVL_TREE_H
  #define AVL_TREE_H

  #include <stdbool.h>
  #include <stddef.h>

  #define PREORDER 0
  #define INORDER 1
  #define POSTORDER 2
  #define clibds_avl_initialize(A, T, C) clibds_avl_initialize_bysize(A, sizeof(T), C)

  struct avl_node {
    void * data;
    struct avl_node * parent;
    struct avl_node * left;
    struct avl_node * right;
    long int hight;
    int balance_factor;
  };

  struct avl_tree {
    int (* comparator)(void *, void *);
    struct avl_node * root;
    size_t memsize;
    size_t size;
  };

  typedef struct avl_node avlnode_t;
  typedef struct avl_tree avltree_t;

  bool clibds_avl_initialize_bysize(avltree_t * const, size_t, int (*)(void *, void *));
  bool clibds_avl_insert(avltree_t * const, void *);
  bool clibds_avl_remove(avltree_t * const, void *);
  size_t clibds_avl_delete(avltree_t * const);
  void * clibds_avl_search(avltree_t * const, void *);
  bool clibds_avl_viewdata(avltree_t * const, void (*)(void *), int);
  size_t clibds_avl_getdata(avltree_t * const, void *, int);
  void * clibds_avl_minimum(avltree_t * const);
  void * clibds_avl_maximum(avltree_t * const);

#endif
