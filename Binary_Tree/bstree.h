/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef BST_H
  #define BST_H

  #include <stdbool.h>
  #include <stddef.h>

  #define PREORDER 0
  #define INORDER 1
  #define POSTORDER 2
  #define clibds_bst_initialize(B, T, C) clibds_bst_initialize_bysize(B, sizeof(T), C)

  struct bst_node {
    void * data;
    struct bst_node * left;
    struct bst_node * right;
    struct bst_node * parent;
  };

  struct bst {
    int (* comparator)(void *, void *);
    struct bst_node * root;
    size_t memsize;
    size_t size;
  };

  typedef struct bst_node bstnode_t;
  typedef struct bst bstree_t;

  bool clibds_bst_initialize_bysize(bstree_t * const, size_t, int (*)(void *, void *));
  bool clibds_bst_insert(bstree_t * const, void *);
  bool clibds_bst_viewdata(bstree_t *, void (*)(void *), int);
  size_t clibds_bst_getdata(bstree_t *, void *, int);
  bool clibds_bst_remove(bstree_t * const, void *);
  size_t clibds_bst_delete(bstree_t * const);
  void * clibds_bst_search(bstree_t * const, void *);
  void * clibds_bst_minimum(bstree_t * const);
  void * clibds_bst_maximum(bstree_t * const);

#endif
