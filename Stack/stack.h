/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef CLIBDS_STACK_H
  #define CLIBDS_STACK_H

  #include <stdbool.h>
  #include <stddef.h>
  #include "vector.h"
  #define clibds_stack_init(S, T, ...) \
    clibds_invoke(S, T __VA_OPT__(,) __VA_ARGS__, clibds_stack_init_withconf, clibds_stack_init_default)\
                  (S, T __VA_OPT__(,) __VA_ARGS__)
  #define clibds_stack_init_withconf(V, T, C) clibds_stack_init_bysize(V, sizeof(T), C)
  #define clibds_stack_init_default(V, T) clibds_stack_init_bysize(V, sizeof(T), NULL)
  #define clibds_stack_isempty(S) clibds_vec_isempty(S)
  #define clibds_stack_size(S) clibds_vec_size(S)
  #define clibds_stack_foreach(S, I) clibds_vec_foreach(S, I)

  #define clibds_stack_fmt_data(I, T) clibds_vec_fmt_data(I, T)

/*
  struct clibds_stack_conf {
    vector_conf_t conf_stack_vec;
  };

  struct clibds_stack {
    vector_t stack_vec;
  };

  typedef struct clibds_stack_conf stack_conf_t;
  typedef struct clibds_stack stack_t;
*/

  typedef vector_t stack_t;
  typedef vector_conf_t stack_conf_t;

  bool clibds_stack_init_conf(stack_conf_t * const restrict, size_t,
                            void * (*)(size_t), void (*)(void *));
  bool clibds_stack_destroy_conf(stack_conf_t * const restrict);
  bool clibds_stack_init_bysize(stack_t * const, size_t, stack_conf_t * const);
  bool clibds_stack_push(stack_t * const, void *);
  size_t clibds_stack_push_from_array(stack_t * const, void * const, size_t);
  bool clibds_stack_pop(stack_t * const);
  void * clibds_stack_peek(stack_t * const);
  size_t clibds_stack_clear(stack_t * const);
  size_t clibds_stack_delete(stack_t * const);
  bool clibds_stack_clone(stack_t * const, stack_t * const);

#endif
