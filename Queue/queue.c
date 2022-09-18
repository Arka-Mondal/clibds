#include <stdlib.h>
#include <string.h>
#include "queue.h"

void internal_initialize_queue(queue_t * const ptr, size_t size_given)
{
  ptr->head = NULL;
  ptr->tail = NULL;
  ptr->memsize = size_given;
  ptr->size = 0;
}

bool enqueue(queue_t * const ptr, void * data_given)
{
  qnode_t * current;
  void * dataptr;

  current = (qnode_t *) malloc((ptr->memsize) + sizeof(qnode_t));
  if (current == NULL)
    return false;
  else
  {
    dataptr = (void *)(current + 1);

    memcpy(dataptr, data_given, ptr->memsize);

    current->data = dataptr;
    current->next = NULL;
    ptr->size += 1;

    if (ptr->head == NULL)
    {
      ptr->head = current;
      ptr->tail = current;
    }
    else
    {
      ptr->tail->next = current;
      ptr->tail = current;
    }

    return true;
  }
}

bool dequeue(queue_t * const ptr)
{
  qnode_t * temp;

  temp = ptr->head;

  if (temp != NULL)
  {
    ptr->head = ptr->head->next;

    if (ptr->size == 1)
      ptr->tail = NULL;

    free(temp);
    ptr->size -= 1;

    return true;
  }
  else
    return false;
}

void * queue_peek(queue_t * const ptr)
{
  if (ptr->head != NULL)
    return ptr->head->data;
  else
    return NULL;
}

size_t delete_queue(queue_t * const ptr)
{
  qnode_t * temp;
  size_t count;

  count = 0;

  while (ptr->head != NULL)
  {
    temp = ptr->head;
    ptr->head = ptr->head->next;
    free(temp);
    count++;
  }

  ptr->tail = NULL;
  ptr->memsize = 0;
  ptr->size = 0;

  return count;
}
