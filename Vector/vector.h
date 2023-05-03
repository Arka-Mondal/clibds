/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef CLIBDS_VECTOR_H
  #define CLIBDS_VECTOR_H

  #include <stdbool.h>
  #include <stddef.h>
  #define DEFAULT_INIT_CAP 32
  #define DEFAULT_EXP_FAC 2
  #define VEC_MAX_CAPACITY ((size_t) -2)
  #define clibds_vec_init(V, T, C) clibds_vec_init_bysize(V, sizeof(T), C)
  #define clibds_vec_size(V) (((V) != NULL) ? ((V)->size) : 0)
  #define clibds_vec_isempty(V) (((V) != NULL) ? (((V)->size == 0) ? true : false) : true)
  #define clibds_vec_foreach(V, I) \
    for (void ** I = (V)->buffer, ** it2 = (V)->buffer + (V)->size; I != it2; I++)

  struct clibds_vector_conf {
    // initial buffer capacity
    size_t init_capacity;
    // expansion factor
    size_t init_exp_factor;

    void * (* conf_mem_alloc)(size_t);
    void (* conf_mem_free)(void *);
  };

  struct clibds_vector {
    void ** buffer;

    // current number of elements in the vector
    size_t size;

    // current capacity of the buffer
    size_t capacity;

    // size of each element of the vector
    size_t memsize;

    // expansion factor
    size_t exp_factor;

    void * (* mem_alloc)(size_t);
    void (* mem_free)(void *);
  };

  typedef struct clibds_vector_conf vector_conf_t;
  typedef struct clibds_vector vector_t;

  bool clibds_vec_init_conf(vector_conf_t * const, size_t, size_t,
                            void * (*)(size_t), void (*)(void *));
  bool clibds_vec_init_bysize(vector_t * const, size_t, vector_conf_t * const);
  bool clibds_vec_push(vector_t * const, void * const);
  bool clibds_vec_push_assumecapacity(vector_t * const, void * const);
  bool clibds_vec_insert(vector_t * const, size_t, void * const);
  bool clibds_vec_insert_assumecapacity(vector_t * const, size_t, void * const);
  bool clibds_vec_pop(vector_t * const);
  bool clibds_vec_remove(vector_t * const, size_t);
  void * clibds_vec_getlast(vector_t * const);
  void * clibds_vec_getatindex(vector_t * const, size_t);
  size_t clibds_vec_clear(vector_t * const);
  size_t clibds_vec_delete(vector_t * const);

#endif
