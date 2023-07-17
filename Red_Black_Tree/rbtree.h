/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef REDBLACK_TREE_H
  #define REDBLACK_TREE_H

  #include <stdbool.h>
  #include <stddef.h>

  #define PREORDER 0
  #define INORDER 1
  #define POSTORDER 2
  #define clibds_rbt_initialize(R, T, C) clibds_rbt_initialize_bysize(R, sizeof(T), C)

  struct red_black_node {
    void * data;
    struct red_black_node * parent;
    struct red_black_node * left;
    struct red_black_node * right;
    bool node_color;
  };

  struct red_black_tree {
    int (* comparator)(void *, void *);
    struct red_black_node * root;
    size_t memsize;
    size_t size;
  };

  typedef struct red_black_node rbtnode_t;
  typedef struct red_black_tree rbtree_t;

  bool clibds_rbt_initialize_bysize(rbtree_t * const, size_t, int (*)(void *, void *));
  bool clibds_rbt_insert(rbtree_t * const, void *);
  bool clibds_rbt_remove(rbtree_t * const, void *);
  size_t clibds_rbt_delete(rbtree_t * const);
  void * clibds_rbt_search(rbtree_t * const, void *);
  bool clibds_rbt_viewdata(rbtree_t * const, void (*)(void *), int);
  size_t clibds_rbt_getdata(rbtree_t * const, void *, int);
  void * clibds_rbt_minimum(rbtree_t * const);
  void * clibds_rbt_maximum(rbtree_t * const);

#endif
