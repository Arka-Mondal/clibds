/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef CLIBDS_LINKEDLIST_H
  #define CLIBDS_LINKEDLIST_H

  #include <stdbool.h>
  #include <stddef.h>

  typedef struct clibds_list_conf list_conf_t;

  typedef struct clibds_forward_node fnode_t;
  typedef struct clibds_node node_t;
  typedef struct clibds_forward_list flist_t;
  typedef struct clibds_list list_t;

  #include "vector.h"

  #define clibds_flist_init(L, T, C) clibds_flist_init_bysize(L, sizeof(T), C)
  #define clibds_flist_size(L) (((L) != NULL) ? ((L)->size) : 0)
  #define clibds_flist_isempty(L) (((L) != NULL) ? (((L)->size == 0) ? true : false) : true)
  #define clibds_flist_foreach(L, I) for (fnode_t * I = ((L) != NULL) ? (L)->head : NULL; I != NULL; I = I->next)

  #define clibds_list_init(L, T, C) clibds_list_init_bysize(L, sizeof(T), C)
  #define clibds_list_size(L) (((L) != NULL) ? ((L)->size) : 0)
  #define clibds_list_isempty(L) (((L) != NULL) ? (((L)->size == 0) ? true : false) : true)
  #define clibds_list_foreach(L, I) for (node_t * I = ((L) != NULL) ? (L)->head : NULL; I != NULL; I = I->next)

  #define clibds_list_format_data(I, T) (*(T *) ((I)->data))
  #define clibds_flist_format_data(I, T) clibds_list_format_data(I, T)

  struct clibds_list_conf {
    void * (* conf_mem_alloc)(size_t);
    void (* conf_mem_free)(void *);
  };

  struct clibds_forward_node {
    void * data;
    struct clibds_forward_node * next;
  };

  struct clibds_node {
    void * data;
    struct clibds_node * next;
    struct clibds_node * previous;
  };

  struct clibds_forward_list {
    struct clibds_forward_node * head;
    struct clibds_forward_node * tail;
    size_t memsize;
    size_t size;

    void * (* mem_alloc)(size_t);
    void (* mem_free)(void *);
  };

  struct clibds_list {
    struct clibds_node * head;
    struct clibds_node * tail;
    size_t memsize;
    size_t size;

    void * (* mem_alloc)(size_t);
    void (* mem_free)(void *);
  };

  bool clibds_list_init_conf(list_conf_t * const restrict,
                             void * (*)(size_t), void (*)(void *));

  bool clibds_flist_init_bysize(flist_t * const restrict, size_t, list_conf_t * const);
  bool clibds_flist_pushback(flist_t * const restrict, void * const);
  size_t clibds_flist_pushback_from_array(flist_t * const restrict, void * const, size_t);
  size_t clibds_flist_pushfront_from_array(flist_t * const restrict, void * const, size_t);
  size_t clibds_flist_pushback_from_vector(flist_t * const restrict, vector_t * const);
  size_t clibds_flist_pushfront_from_vector(flist_t * const restrict, vector_t * const);
  bool clibds_flist_pushfront(flist_t * const restrict, void * const);
  bool clibds_flist_pushatindex(flist_t * const restrict, size_t, void * const);
  bool clibds_flist_popfront(flist_t * const restrict);
  bool clibds_flist_popback(flist_t * const restrict);
  bool clibds_flist_popatindex(flist_t * const restrict, size_t);
  size_t clibds_flist_clear(flist_t * const restrict);
  size_t clibds_flist_delete(flist_t * const restrict);
  void * clibds_flist_getatindex(flist_t * const restrict, size_t);
  size_t clibds_flist_reverse(flist_t * const restrict);
  bool clibds_flist_ispalindrome(flist_t * const restrict, int (*)(const void *, const void *));

  bool clibds_list_init_bysize(list_t * const restrict, size_t, list_conf_t * const);
  bool clibds_list_pushback(list_t * const restrict, void * const);
  size_t clibds_list_pushback_from_array(list_t * const restrict, void * const, size_t);
  size_t clibds_list_pushfront_from_array(list_t * const restrict, void * const, size_t);
  size_t clibds_list_pushback_from_vector(list_t * const restrict, vector_t * const);
  size_t clibds_list_pushfront_from_vector(list_t * const restrict, vector_t * const);
  bool clibds_list_pushfront(list_t * const restrict, void * const);
  bool clibds_list_pushatindex(list_t * const restrict, size_t, void * const);
  bool clibds_list_popfront(list_t * const restrict);
  bool clibds_list_popback(list_t * const restrict);
  bool clibds_list_popatindex(list_t * const restrict, size_t);
  size_t clibds_list_clear(list_t * const restrict);
  size_t clibds_list_delete(list_t * const restrict);
  void * clibds_list_getatindex(list_t * const restrict, size_t);
  size_t clibds_list_reverse(list_t * const restrict);
  bool clibds_list_ispalindrome(list_t * const restrict, int (*)(const void *, const void *));

#endif
