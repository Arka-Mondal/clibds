#ifndef QUEUE_H
  #define QUEUE_H

  #include <stdbool.h>
  #include <stddef.h>
  #define initialize_queue(S, T) initialize_bysize_queue(S, sizeof(T))
  #define is_empty_queue(S) (((S) != NULL) ? (((S)->size == 0) ? true : false) : true)
  #define for_each_in_queue(Q, I) for (qnode_t * I = ((Q) != NULL) ? (Q)->head : NULL; I != NULL; I = I->next)

  #define initialize_deque(S, T) initialize_bysize_deque(S, sizeof(T))
  #define is_empty_deque(S) (((S) != NULL) ? (((S)->size == 0) ? true : false) : true)
  #define for_each_in_deque(Q, I) for (qnode_t * I = ((Q) != NULL) ? (Q)->head : NULL; I != NULL; I = I->next)

  struct queue_node {
    void * data;
    struct queue_node * next;
  };

  struct queue {
    struct queue_node * head;
    struct queue_node * tail;
    size_t memsize;
    size_t size;
  };

  struct deque {
    struct queue_node * head;
    struct queue_node * tail;
    size_t memsize;
    size_t size;
  };

  typedef struct queue_node qnode_t;
  typedef struct queue queue_t;
  typedef struct deque deque_t;

  bool initialize_bysize_queue(queue_t * const restrict, size_t);
  bool push_queue(queue_t * const restrict, void *);
  bool pop_queue(queue_t * const restrict);
  void * peek_queue(queue_t * const restrict);
  size_t clear_queue(queue_t * const restrict);
  size_t delete_queue(queue_t * const restrict);

  bool initialize_bysize_deque(deque_t * const restrict, size_t);
  bool pushfront_deque(deque_t * const restrict, void *);
  bool pushback_deque(deque_t * const restrict, void *);
  bool popfront_deque(deque_t * const restrict);
  bool popback_deque(deque_t * const restrict);
  void * peekfront_deque(deque_t * const restrict);
  void * peekback_deque(deque_t * const restrict);
  size_t clear_deque(deque_t * const restrict);
  size_t delete_deque(deque_t * const restrict);

#endif
