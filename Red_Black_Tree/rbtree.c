/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rbtree.h"
#define RED true
#define BLACK false

static void clibds_rbt_left_rotate(rbtree_t * const ptr, rbtnode_t * node)
{
  rbtnode_t * temp;

  temp = node->right;
  node->right = temp->left;
  if (temp->left != NULL)
    temp->left->parent = node;

  temp->parent = node->parent;

  if (node->parent == NULL)
    ptr->root = temp;
  else if (node->parent->left == node)
    node->parent->left = temp;
  else
    node->parent->right = temp;

  temp->left = node;
  node->parent = temp;
}

static void clibds_rbt_right_rotate(rbtree_t * const ptr, rbtnode_t * node)
{
  rbtnode_t * temp;

  temp = node->left;
  node->left = temp->right;
  if (temp->right != NULL)
    temp->right->parent = node;

  temp->parent = node->parent;

  if (node->parent == NULL)
    ptr->root = temp;
  else if (node->parent->left == node)
    node->parent->left = temp;
  else
    node->parent->right = temp;

  temp->right = node;
  node->parent = temp;
}

static void clibds_rbt_insert_fix(rbtree_t * const ptr, rbtnode_t * node)
{
  rbtnode_t * temp;

  while ((node->parent != NULL) && node->parent->node_color == RED)
  {
    if (node->parent == node->parent->parent->left)
    {
      temp = node->parent->parent->right;
      if ((temp != NULL) && (temp->node_color == RED))
      {
        node->parent->node_color = BLACK;
        temp->node_color = BLACK;
        node->parent->parent->node_color = RED;
        node = node->parent->parent;
      }
      else
      {
        if (node == node->parent->right)
        {
          node = node->parent;
          clibds_rbt_left_rotate(ptr, node);
        }
        node->parent->node_color = BLACK;
        node->parent->parent->node_color = RED;
        clibds_rbt_right_rotate(ptr, node->parent->parent);
      }
    }
    else
    {
      temp = node->parent->parent->left;
      if ((temp != NULL) && (temp->node_color == RED))
      {
        node->parent->node_color = BLACK;
        temp->node_color = BLACK;
        node->parent->parent->node_color = RED;
        node = node->parent->parent;
      }
      else
      {
        if (node == node->parent->left)
        {
          node = node->parent;
          clibds_rbt_right_rotate(ptr, node);
        }
        node->parent->node_color = BLACK;
        node->parent->parent->node_color = RED;
        clibds_rbt_left_rotate(ptr, node->parent->parent);
      }
    }
  }

  ptr->root->node_color = BLACK;
}

static void clibds_rbt_remove_fix(rbtree_t * ptr, rbtnode_t * node, rbtnode_t * node_parent)
{
  rbtnode_t * node_sibling;

  while ((node != ptr->root) && ((node == NULL) || (node->node_color == BLACK)))
  {
    if (node == node_parent->left)
    {
      node_sibling = node_parent->right;
      if (node_sibling->node_color == RED)
      {
        node_sibling->node_color = BLACK;
        node_parent->node_color = RED;
        clibds_rbt_left_rotate(ptr, node_parent);
        node_sibling = node_parent->right;
      }
      if ((node_sibling->left == NULL || node_sibling->left->node_color == BLACK) &&
          (node_sibling->right == NULL || node_sibling->right->node_color == BLACK))
      {
        node_sibling->node_color = RED;
        node = node_parent;
        node_parent = node->parent;
      }
      else
      {
        if (node_sibling->right == NULL || node_sibling->right->node_color == BLACK)
        {
          node_sibling->left->node_color = BLACK;
          node_sibling->node_color = RED;
          clibds_rbt_right_rotate(ptr, node_sibling);
          node_sibling = node_parent->right;
        }
        node_sibling->node_color = node_parent->node_color;
        node_parent->node_color = BLACK;
        node_sibling->right->node_color = BLACK;
        clibds_rbt_left_rotate(ptr, node_parent);
        node = ptr->root;
      }
    }
    else
    {
      if (node == node_parent->right)
      {
        node_sibling = node_parent->left;
        if (node_sibling->node_color == RED)
        {
          node_sibling->node_color = BLACK;
          node_parent->node_color = RED;
          clibds_rbt_right_rotate(ptr, node_parent);
          node_sibling = node_parent->left;
        }
        if ((node_sibling->left == NULL || node_sibling->left->node_color == BLACK) &&
            (node_sibling->right == NULL || node_sibling->right->node_color == BLACK))
        {
          node_sibling->node_color = RED;
          node = node->parent;
          node_parent = node->parent;
        }
        else
        {
          if (node_sibling->left == NULL || node_sibling->left->node_color == BLACK)
          {
            node_sibling->right->node_color = BLACK;
            node_sibling->node_color = RED;
            clibds_rbt_left_rotate(ptr, node_sibling);
            node_sibling = node_parent->left;
          }
          node_sibling->node_color = node_parent->node_color;
          node_parent->node_color = BLACK;
          node_sibling->left->node_color = BLACK;
          clibds_rbt_right_rotate(ptr, node_parent);
          node = ptr->root;
        }
      }
    }
  }
  if (node != NULL)
    node->node_color = BLACK;
}

static void preorder_rcs_rbt(rbtnode_t * node, void (* print_data)(void *))
{
  if (node == NULL)
    return;
  print_data(node->data);
  preorder_rcs_rbt(node->left, print_data);
  preorder_rcs_rbt(node->right, print_data);
}

static void inorder_rcs_rbt(rbtnode_t * node, void (* print_data)(void *))
{
  if (node == NULL)
    return;
  inorder_rcs_rbt(node->left, print_data);
  print_data(node->data);
  inorder_rcs_rbt(node->right, print_data);
}

static void postorder_rcs_rbt(rbtnode_t * node, void (* print_data)(void *))
{
  if (node == NULL)
    return;
  postorder_rcs_rbt(node->left, print_data);
  postorder_rcs_rbt(node->right, print_data);
  print_data(node->data);
}

static void clibds_rbt_reverseorder(void * arr, size_t datasize, void * temp,
                                    size_t left, size_t right)
{
  while (left < right)
  {
    memcpy(temp, ((uint8_t *) arr) + (left * datasize), datasize);    // temp = arr[left]
    memcpy(((uint8_t *) arr) + (left * datasize), ((uint8_t *) arr) + (right * datasize), datasize); // arr[left] = arr[right]
    memcpy(((uint8_t *) arr) + (right * datasize), temp, datasize);   // arr[right] = temp
    left++;
    right--;
  }
}

static size_t clibds_rbt_preorder(rbtree_t * const ptr, void * arr)
{
  rbtnode_t * current, * pred;
  size_t count, datasize;

  current = ptr->root;
  count = 0;
  datasize = ptr->memsize;

  while (current != NULL)
  {
    if (current->left == NULL)
    {
      memcpy(((uint8_t *) arr) + (count * datasize), current->data, datasize);
      count++;
      current = current->right;
    }
    else
    {
      pred = current->left;
      while ((pred->right != NULL) && (pred->right != current))
        pred = pred->right;

      // Make the link between the predecessor node and current node

      if (pred->right == NULL)
      {
        pred->right = current;
        memcpy(((uint8_t *) arr) + (count * datasize), current->data, datasize);
        count++;
        current = current->left;
      }
      else
      {
        /*
         * Restore the structure of the tree. Remove the extra link between
         * predecessor and current node.
         */

        pred->right = NULL;
        current = current->right;
      }
    }
  }

  return count;
}

static size_t clibds_rbt_inorder(rbtree_t * const ptr, void * arr)
{
  rbtnode_t * current, * pred;
  size_t count, datasize;

  current = ptr->root;
  count = 0;
  datasize = ptr->memsize;

  while (current != NULL)
  {
    if (current->left == NULL)
    {
      memcpy(((uint8_t *) arr) + (count * datasize), current->data, datasize);
      count++;
      current = current->right;
    }
    else
    {
      pred = current->left;
      while ((pred->right != NULL) && (pred->right != current))
        pred = pred->right;

      // Make the link between the predecessor node and current node

      if (pred->right == NULL)
      {
        pred->right = current;
        current = current->left;
      }
      else
      {
        /*
         * Restore the structure of the tree. Remove the extra link between
         * predecessor and current node.
         */

        pred->right = NULL;
        memcpy(((uint8_t *) arr) + (count * datasize), current->data, datasize);
        count++;
        current = current->right;
      }
    }
  }

  return count;
}

static size_t clibds_rbt_postorder(rbtree_t * const ptr, void * arr)
{
  void * temp_var;
  rbtnode_t * temp, * current, * pred;
  size_t count, datasize, node_count;

  count = 0;
  datasize = ptr->memsize;

  temp = (rbtnode_t *) ptr->mem_alloc(datasize + sizeof(rbtnode_t));
  if (temp == NULL)
    return count;

  current = temp;
  current->left = ptr->root;
  temp_var = (void *) (temp + 1);

  while (current != NULL)
  {
    if (current->left == NULL)
      current = current->right;
    else
    {
      pred = current->left;

      while ((pred->right != NULL) && (pred->right != current))
        pred = pred->right;


      if (pred->right == NULL)
      {
        pred->right = current;
        current = current->left;
      }
      else
      {
        pred = current->left;
        node_count = 1;

        while ((pred->right != NULL) && (pred->right != current))
        {
          memcpy(((uint8_t *) arr) + (count * datasize), pred->data, datasize);
          count++;
          pred = pred->right;
          node_count++;
        }

        memcpy(((uint8_t *) arr) + (count * datasize), pred->data, datasize);
        count++;

        pred->right = NULL;
        clibds_rbt_reverseorder(arr, datasize, temp_var, count - node_count, count - 1);
        current = current->right;
      }
    }
  }

  ptr->mem_free(temp);

  return count;
}

bool clibds_rbt_initialize_bysize(rbtree_t * const ptr, size_t size_given,
                                  rbtree_conf_t * const conf_ptr)
{
  if ((ptr == NULL) || (size_given == 0))
    return false;

  if (conf_ptr->conf_comparator == NULL)
    return false;

  if (pthread_mutex_init(&ptr->rbt_mtx, NULL) != 0)
    return false;

  ptr->comparator = conf_ptr->conf_comparator;
  ptr->mem_alloc = (conf_ptr->conf_mem_alloc != NULL) ? conf_ptr->conf_mem_alloc : malloc;
  ptr->mem_free = (conf_ptr->conf_mem_free != NULL) ? conf_ptr->conf_mem_free : free;

  ptr->root = NULL;
  ptr->memsize = size_given;
  ptr->size = 0;

  return true;
}

bool clibds_rbt_insert(rbtree_t * const ptr, void * data_given)
{
  rbtnode_t * current, * temp, * parent_node;
  void * dataptr;

  if ((ptr == NULL) || (data_given == NULL))
    return false;

  parent_node = NULL;

  pthread_mutex_lock(&ptr->rbt_mtx);
  temp = ptr->root;

  current = (rbtnode_t *) ptr->mem_alloc(ptr->memsize + sizeof(rbtnode_t));
  if (current == NULL)
    return false;

  dataptr = (void *) (current + 1);

  memcpy(dataptr, data_given, ptr->memsize);

  current->left = NULL;
  current->right = NULL;
  current->data = dataptr;

  while (temp != NULL)
  {
    parent_node = temp;
    if (ptr->comparator(dataptr, temp->data) == -1)
      temp = temp->left;
    else if (ptr->comparator(dataptr, temp->data) == 1)
      temp = temp->right;
    else
    {
      pthread_mutex_unlock(&ptr->rbt_mtx);
      ptr->mem_free(current);
      return true;
    }
  }

  current->parent = parent_node;
  if (parent_node == NULL)
    ptr->root = current;
  else if (ptr->comparator(dataptr, parent_node->data) == -1)
    parent_node->left = current;
  else
    parent_node->right = current;

  if (ptr->root != NULL)
  {
    current->node_color = RED;
    clibds_rbt_insert_fix(ptr, current);
  }
  else
    current->node_color = BLACK;

  ptr->size++;
  pthread_mutex_unlock(&ptr->rbt_mtx);

  return true;
}

size_t clibds_rbt_insert_from_array(rbtree_t * const ptr, void * arr, size_t nelem)
{
  uint8_t * start, * end;
  size_t count;

  if ((ptr == NULL) || (arr == NULL) || (nelem == 0))
    return 0;

  count = 0;
  end = ((uint8_t *) arr) + (ptr->memsize * nelem);

  for (start = (uint8_t *) arr; start < end; start += ptr->memsize, count++)
    if (!clibds_rbt_insert(ptr, start))
      break;

  return count;
}

bool clibds_rbt_remove(rbtree_t * const ptr, void * dataptr)
{
  bool original_color;
  rbtnode_t * current, * temp, * child_node, * parent_node;

  if ((ptr == NULL) || (dataptr == NULL))
    return false;

  pthread_mutex_lock(&ptr->rbt_mtx);
  current = ptr->root;

  while (current != NULL)
  {
    if (ptr->comparator(dataptr, current->data) == -1)
      current = current->left;
    else if (ptr->comparator(dataptr, current->data) == 1)
      current = current->right;
    else
      break;
  }

  if (current == NULL)
  {
    pthread_mutex_unlock(&ptr->rbt_mtx);
    return false;
  }

  temp = current;
  original_color = temp->node_color;

  if ((current->left != NULL) && (current->right != NULL))
  {
    temp = current->left;
    while (temp->right != NULL)
      temp = temp->right;

    original_color = temp->node_color;
    memcpy(current->data, temp->data, ptr->memsize);

    current = temp;
  }

  if ((current->left == NULL) && (current->right == NULL))
  {
    if (current == ptr->root)
      ptr->root = NULL;
    else
    {
      if (current == current->parent->left)
        current->parent->left = NULL;
      else
        current->parent->right = NULL;
    }
    child_node = NULL;
    parent_node = current->parent;
  }
  else if (current->left == NULL)
  {
    child_node = current->right;

    if (current->parent == NULL)
      ptr->root = child_node;
    else if (current == current->parent->right)
      current->parent->right = child_node;
    else
      current->parent->left = child_node;

    child_node->parent = current->parent;

    parent_node = current->parent;
  }
  else
  {
    child_node = current->left;

    if (current->parent == NULL)
      ptr->root = child_node;
    else if (current == current->parent->right)
      current->parent->right = child_node;
    else
      current->parent->left = child_node;

    child_node->parent = current->parent;

    parent_node = current->parent;
  }

  ptr->mem_free(current);

  if (original_color == BLACK)
    clibds_rbt_remove_fix(ptr, child_node, parent_node);

  ptr->size--;
  pthread_mutex_unlock(&ptr->rbt_mtx);

  return true;
}

size_t clibds_rbt_delete(rbtree_t * const ptr)
{
  rbtnode_t * current, * pred;
  size_t count;

  if (ptr == NULL)
    return 0;

  // lock the mutex
  pthread_mutex_lock(&ptr->rbt_mtx);

  current = ptr->root;
  count = 0;

  while (current != NULL)
  {
    if (current->left != NULL)
    {
      pred = current->left;
      while (pred->right != NULL)
        pred = pred->right;

      pred->right = current->right;
      current->right = current->left;
    }

    // using pred pointer to temporarily store the current poisition

    pred = current;
    current = current->right;
    ptr->mem_free(pred);
    count++;
  }

  ptr->root = NULL;
  ptr->comparator = NULL;
  ptr->memsize = 0;
  ptr->size = 0;

  // unlock the mutex
  pthread_mutex_unlock(&ptr->rbt_mtx);
  // destroy the mutex
  pthread_mutex_destroy(&ptr->rbt_mtx);

  return count;
}

void * clibds_rbt_search(rbtree_t * const ptr, void * dataptr)
{
  void * data;
  rbtnode_t * current;

  if ((ptr == NULL) || (dataptr == NULL))
    return NULL;

  data = NULL;

  pthread_mutex_lock(&ptr->rbt_mtx);
  current = ptr->root;

  while (current != NULL)
  {
    if (ptr->comparator(dataptr, current->data) == -1)
      current = current->left;
    else if (ptr->comparator(dataptr, current->data) == 1)
      current = current->right;
    else
    {
      data = current->data;
      break;
    }
  }

  pthread_mutex_unlock(&ptr->rbt_mtx);

  return data;
}

bool clibds_rbt_viewdata(rbtree_t * const ptr, void (* print_data)(void *), int select)
{
  bool status;

  if (print_data == NULL)
    return false;

  status = true;
  pthread_mutex_lock(&ptr->rbt_mtx);

  if (select == PREORDER)
    preorder_rcs_rbt(ptr->root, print_data);
  else if (select == INORDER)
    inorder_rcs_rbt(ptr->root, print_data);
  else if (select == POSTORDER)
    postorder_rcs_rbt(ptr->root, print_data);
  else
    status = false;

  pthread_mutex_unlock(&ptr->rbt_mtx);

  return status;
}

size_t clibds_rbt_getdata(rbtree_t * const ptr, void * arr, int select)
{
  size_t count;

  if ((ptr == NULL) || (arr == NULL))
    return 0;

  pthread_mutex_lock(&ptr->rbt_mtx);

  if (select == PREORDER)
    count = clibds_rbt_preorder(ptr, arr);
  else if (select == INORDER)
    count = clibds_rbt_inorder(ptr, arr);
  else if (select == POSTORDER)
    count = clibds_rbt_postorder(ptr, arr);
  else
    count = 0;

  pthread_mutex_unlock(&ptr->rbt_mtx);

  return count;
}

void * clibds_rbt_minimum(rbtree_t * const ptr)
{
  void * data;
  rbtnode_t * current;

  if (ptr == NULL)
    return NULL;

  data = NULL;

  pthread_mutex_lock(&ptr->rbt_mtx);
  current = ptr->root;

  if (current != NULL)
  {
    while (current->left != NULL)
      current = current->left;

  data = current->data;
  }

  pthread_mutex_unlock(&ptr->rbt_mtx);

  return data;
}

void * clibds_rbt_maximum(rbtree_t * const ptr)
{
  void * data;
  rbtnode_t * current;

  if (ptr == NULL)
    return NULL;

  data = NULL;

  pthread_mutex_lock(&ptr->rbt_mtx);
  current = ptr->root;

  if (current != NULL)
  {
    while (current->right != NULL)
      current = current->right;

    data = current->data;
  }
  pthread_mutex_unlock(&ptr->rbt_mtx);

  return data;
}
