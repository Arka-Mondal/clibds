/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef REDBLACK_TREE_H
  #define REDBLACK_TREE_H

  #include <stdbool.h>
  #include <stddef.h>
  #include <pthread.h>

  #define PREORDER 0
  #define INORDER 1
  #define POSTORDER 2

  struct clibds_rbt_conf {
    int (* conf_comparator)(void *, void *);

    void * (* conf_mem_alloc)(size_t);
    void (* conf_mem_free)(void *);
  };

  struct clibds_rbt_node {
    void * data;
    struct clibds_rbt_node * parent;
    struct clibds_rbt_node * left;
    struct clibds_rbt_node * right;
    bool node_color;
  };

  struct clibds_rbtree {
    int (* comparator)(void *, void *);
    void * (* mem_alloc)(size_t);
    void (* mem_free)(void *);

    struct clibds_rbt_node * root;
    size_t memsize;
    size_t size;

    pthread_mutex_t rbt_mtx;
  };

  typedef struct clibds_rbt_conf rbtree_conf_t;
  typedef struct clibds_rbt_node rbtnode_t;
  typedef struct clibds_rbtree rbtree_t;

  #define clibds_rbt_initialize(R, T, C, ...)\
    clibds_rbt_initialize_bysize(R, sizeof(T),\
                    &(rbtree_conf_t){.conf_comparator = C __VA_OPT__(,)  __VA_ARGS__})

  bool clibds_rbt_initialize_bysize(rbtree_t * const, size_t, rbtree_conf_t * const);
  bool clibds_rbt_insert(rbtree_t * const, void *);
  size_t clibds_rbt_insert_from_array(rbtree_t * const, void *, size_t);
  bool clibds_rbt_remove(rbtree_t * const, void *);
  size_t clibds_rbt_delete(rbtree_t * const);
  void * clibds_rbt_search(rbtree_t * const, void *);
  [[deprecated]] bool clibds_rbt_viewdata(rbtree_t * const, void (*)(void *), int);
  size_t clibds_rbt_getdata(rbtree_t * const, void *, int);
  void * clibds_rbt_minimum(rbtree_t * const);
  void * clibds_rbt_maximum(rbtree_t * const);

#endif
