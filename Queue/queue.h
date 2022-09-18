#ifndef QUEUE_H
  #define QUEUE_H

  #include <stdbool.h>
  #include <stddef.h>
  #define initialize_queue(S, T) internal_initialize_queue(S, sizeof(T))
  #define is_empty_queue(S) (((S)->size == 0) ? 1 : 0)
  #define for_each_in_queue(Q, I) for (qnode_t * I = (Q)->head; I != NULL; I = I->next)

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

  void internal_initialize_queue(queue_t * const, size_t);
  bool enqueue(queue_t * const, void *);
  bool dequeue(queue_t * const);
  void * queue_peek(queue_t * const);
  size_t delete_queue(queue_t * const);

#endif
