/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef LINKEDLIST_H
  #define LINKEDLIST_H

  #include <stdbool.h>
  #include <stddef.h>
  #define initialize_list(S, T) initialize_bysize_list(S, sizeof(T))
  #define is_empty_list(S) (((S) != NULL) ? (((S)->size == 0) ? true : false) : true)
  #define for_each_in_list(Q, I) for (node_t * I = ((Q) != NULL) ? (Q)->head : NULL; I != NULL; I = I->next)

  #define initialize_listd(S, T) initialize_bysize_listd(S, sizeof(T))
  #define is_empty_listd(S) (((S) != NULL) ? (((S)->size == 0) ? true : false) : true)
  #define for_each_in_listd(Q, I) for (dnode_t * I = ((Q) != NULL) ? (Q)->head : NULL; I != NULL; I = I->next)

  #define format_data(I, T) (*(T *) (I))

  struct node {
    void * data;
    struct node * next;
  };

  struct node_d {
    void * data;
    struct node_d * next;
    struct node_d * previous;
  };

  struct list {
    struct node * head;
    struct node * tail;
    size_t memsize;
    size_t size;
  };

  struct list_d {
    struct node_d * head;
    struct node_d * tail;
    size_t memsize;
    size_t size;
  };


  typedef struct node node_t;
  typedef struct list list_t;
  typedef struct node_d dnode_t;
  typedef struct list_d listd_t;

  bool initialize_bysize_list(list_t * const restrict, size_t);
  bool pushback_list(list_t * const restrict, void *);
  bool pushfront_list(list_t * const restrict, void *);
  bool pushatindex_list(list_t * const restrict, size_t, void *);
  bool popfront_list(list_t * const restrict);
  bool popback_list(list_t * const restrict);
  bool popatindex_list(list_t * const restrict, size_t);
  size_t clear_list(list_t * const restrict);
  size_t delete_list(list_t * const restrict);
  void * getnode_atindex_list(list_t * const restrict, size_t);
  size_t reverse_list(list_t * const restrict);
  bool is_palindrome_list(list_t * const restrict, int (*)(const void *, const void *));

  bool initialize_bysize_listd(listd_t * const restrict, size_t);
  bool pushback_listd(listd_t * const restrict, void *);
  bool pushfront_listd(listd_t * const restrict, void *);
  bool pushatindex_listd(listd_t * const restrict, size_t, void *);
  bool popfront_listd(listd_t * const restrict);
  bool popback_listd(listd_t * const restrict);
  bool popatindex_listd(listd_t * const restrict, size_t);
  size_t clear_listd(listd_t * const restrict);
  size_t delete_listd(listd_t * const restrict);
  void * getnode_atindex_listd(listd_t * const restrict, size_t);
  size_t reverselist_listd(listd_t * const restrict);
  bool is_palindrome_listd(listd_t * const restrict, int (*)(const void *, const void *));

#endif
