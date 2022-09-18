#ifndef QUEUE_H
  #define QUEUE_H

  #include <stdbool.h>
  #include <stddef.h>
  #define initialize_queue(S, T) initialize_bysize_queue(S, sizeof(T))
  #define is_empty_queue(S) (((S)->size == 0) ? 1 : 0)
  #define for_each_in_queue(Q, I) for (qnode_t * I = ((Q) != NULL) ? (Q)->head : NULL; I != NULL; I = I->next)

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

  typedef struct queue_node qnode_t;
  typedef struct queue queue_t;

  bool initialize_bysize_queue(queue_t * const, size_t);
  bool push_queue(queue_t * const, void *);
  bool pop_queue(queue_t * const);
  void * peek_queue(queue_t * const);
  size_t delete_queue(queue_t * const);

#endif
