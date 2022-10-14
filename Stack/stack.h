/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef STACK_H
  #define STACK_H

  #include <stdbool.h>
  #include <stddef.h>
  #define initialize_stack(S, T) initialize_bysize_stack(S, sizeof(T))
  #define is_empty_stack(S) (((S) != NULL) ? (((S)->size == 0) ? true : false) : true)

  #define format_data(I, T) (*(T *) (I))

  struct stack {
    void ** buffer;

    // The current number of the elements in the priority queue
    size_t size;

    // The current capacity of the buffer
    size_t capacity;

    // The size of each element in the priority queue
    size_t memsize;

    // The expansion factor of the buffer
    double exp_factor;
  };

  typedef struct stack stack_t;

  bool initialize_bysize_stack(stack_t * const restrict, size_t);
  bool push_stack(stack_t * const restrict, void *);
  bool pop_stack(stack_t * const restrict);
  void * peek_stack(stack_t * const restrict);
  size_t clear_stack(stack_t * const restrict);
  size_t delete_stack(stack_t * const restrict);

#endif
