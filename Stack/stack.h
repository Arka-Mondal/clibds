#ifndef STACK_H
  #define STACK_H

  #include <stdbool.h>
  #include <stddef.h>
  #define initialize_stack(S, T) initialize_bysize_stack(S, sizeof(T))
  #define is_empty_stack(S) (((S)->size == 0) ? true : false)
  #define for_each_in_stack(Q, I) for (snode_t * I = ((Q) != NULL) ? (Q)->head : NULL; I != NULL; I = I->next)

  struct stack_node {
    void * data;
    struct stack_node * next;
  };

  struct stack {
    struct stack_node * head;
    size_t memsize;
    size_t size;
  };

  typedef struct stack_node snode_t;
  typedef struct stack stack_t;

  bool initialize_bysize_stack(stack_t * const, size_t);
  bool push_stack(stack_t * const, void *);
  bool pop_stack(stack_t * const);
  void * peek_stack(stack_t * const);
  size_t delete_stack(stack_t *);

#endif
