/*
  Copyright (c) 2022, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#ifndef PRIORITY_QUEUE
  #define PRIORITY_QUEUE

  #include <stdbool.h>
  #include <stddef.h>
  #define initialize_pq(P, T, C) initialize_bysize_pq(P, sizeof(T), C)

  // Returns true if the priority queue is empty.
  #define is_empty_pq(P) (((P) != NULL) ? (((P)->size == 0) ? true : false) : true)

  #define format_data(I, T) (*(T *) (I))

  struct priority_queue {
    void ** buffer;

    // The current number of the elements in the priority queue
    size_t size;

    // The current capacity of the buffer
    size_t capacity;

    // The size of each element in the priority queue
    size_t memsize;

    // The expansion factor of the buffer
    double exp_factor;

    /*
     * "comparator" is a pointer to the function which is used
     * to compare the elements in the priority_queue
    */
    int (* comparator)(const void *, const void *);
  };

  typedef struct priority_queue pqueue_t;

  bool initialize_bysize_pq(pqueue_t * const restrict, size_t, int (*)(const void *, const void *));
  bool push_pq(pqueue_t * const restrict, void *);
  void * peek_pq(pqueue_t * const restrict);
  bool pop_pq(pqueue_t * const restrict);
  size_t clear_pq(pqueue_t * const restrict);
  size_t delete_pq(pqueue_t * const restrict);

#endif
