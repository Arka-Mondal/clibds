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
  #define clibds_stack_init(S, T, C) clibds_stack_init_bysize(S, sizeof(T), C)
  #define clibds_stack_isempty(S) (((S) != NULL) ? (((S)->stack_vec.size == 0) ? true : false) : true)
  #define clibds_stack_size(S) (((S) != NULL) ? ((S)->stack_vec.size) : 0)
  #define clibds_stack_foreach(S, I) clibds_vec_foreach(&(S)->stack_vec, I)

  #define format_data(I, T) (*(T *) (I))

  struct clibds_stack_conf {
    vector_conf_t conf_stack_vec;
  };

  struct clibds_stack {
    vector_t stack_vec;
  };

  typedef struct clibds_stack_conf stack_conf_t;
  typedef struct clibds_stack stack_t;

  bool clibds_stack_init_conf(stack_conf_t * const, size_t, size_t,
                            void * (*)(size_t), void (*)(void *));
  bool clibds_stack_init_bysize(stack_t * const, size_t, stack_conf_t * const);
  bool clibds_stack_push(stack_t * const, void *);
  bool clibds_stack_pop(stack_t * const);
  void * clibds_stack_peek(stack_t * const);
  size_t clibds_stack_clear(stack_t * const);
  size_t clibds_stack_delete(stack_t * const);

#endif
