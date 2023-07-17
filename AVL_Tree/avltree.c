/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "avltree.h"
#define MAXOF(X, Y) (((X) > (Y)) ? (X) : (Y))

static void clibds_avl_update_node(avlnode_t * node)
{
  long int left_hight, right_hight;

  left_hight = (node->left == NULL) ? -1 : (node->left)->hight;
  right_hight = (node->right == NULL) ? -1 : (node->right)->hight;

  node->hight = 1 + MAXOF(left_hight, right_hight);
  node->balance_factor = right_hight - left_hight;
}

static void clibds_avl_left_rotation(avltree_t * const ptr, avlnode_t * node)
{
  avlnode_t * temp;

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

  clibds_avl_update_node(temp);
  clibds_avl_update_node(node);
}

static void clibds_avl_right_rotation(avltree_t * const ptr, avlnode_t * node)
{
  avlnode_t * temp;

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

  clibds_avl_update_node(temp);
  clibds_avl_update_node(node);
}

static void clibds_avl_balance_node(avltree_t * const ptr, avlnode_t * node)
{
  if (node->balance_factor == -2) // left heavy
  {
    if ((node->left)->balance_factor <= 0)
      clibds_avl_right_rotation(ptr, node);
    else
    {
      clibds_avl_left_rotation(ptr, node->left);
      clibds_avl_right_rotation(ptr, node);
    }
  }
  else if (node->balance_factor == 2) // right heavy
  {
    if ((node->right)->balance_factor >= 0)
      clibds_avl_left_rotation(ptr, node);
    else
    {
      clibds_avl_right_rotation(ptr, node->right);
      clibds_avl_left_rotation(ptr, node);
    }
  }

  // node has the balance_factor {-1, 0, 1}
}

static void preorder_rcs_avl(avlnode_t * node, void (* print_data)(void *))
{
  if (node == NULL)
    return;

  print_data(node->data);
  preorder_rcs_avl(node->left, print_data);
  preorder_rcs_avl(node->right, print_data);
}

static void inorder_rcs_avl(avlnode_t * node, void (* print_data)(void *))
{
  if (node == NULL)
    return;

  inorder_rcs_avl(node->left, print_data);
  print_data(node->data);
  inorder_rcs_avl(node->right, print_data);
}

static void postorder_rcs_avl(avlnode_t * node, void (* print_data)(void *))
{
  if (node == NULL)
    return;

  postorder_rcs_avl(node->left, print_data);
  postorder_rcs_avl(node->right, print_data);
  print_data(node->data);
}

static void clibds_avl_reverseorder(void * arr, size_t datasize, void * temp,
                                    size_t left, size_t right)
{
  while (left < right)
  {
    memcpy(temp, (char *)arr + (left * datasize), datasize);    // temp = arr[left]
    memcpy((char *)arr + (left * datasize), (char *)arr + (right * datasize), datasize); // arr[left] = arr[right]
    memcpy((char *)arr + (right * datasize), temp, datasize);   // arr[right] = temp
    left++;
    right--;
  }
}

static size_t clibds_avl_preorder(avltree_t * const ptr, void * arr)
{
  avlnode_t * current, * pred;
  size_t count, datasize;

  current = ptr->root;
  count = 0;
  datasize = ptr->memsize;

  while (current != NULL)
  {
    if (current->left == NULL)
    {
      memcpy((char *)arr + (count * datasize), current->data, datasize);
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
        memcpy((char *)arr + (count * datasize), current->data, datasize);
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

static size_t clibds_avl_inorder(avltree_t * const ptr, void * arr)
{
  avlnode_t * current, * pred;
  size_t count, datasize;

  current = ptr->root;
  count = 0;
  datasize = ptr->memsize;

  while (current != NULL)
  {
    if (current->left == NULL)
    {
      memcpy((char *)arr + (count * datasize), current->data, datasize);
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
        memcpy((char *)arr + (count * datasize), current->data, datasize);
        count++;
        current = current->right;
      }
    }
  }

  return count;
}

static size_t clibds_avl_postorder(avltree_t * const ptr, void * arr)
{
  void * temp_var;
  avlnode_t * temp, * current, * pred;
  size_t count, datasize, node_count;

  count = 0;
  datasize = ptr->memsize;

  temp = (avlnode_t *) malloc(datasize + sizeof(avlnode_t));
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
          memcpy((char *)arr + (count * datasize), pred->data, datasize);
          count++;
          pred = pred->right;
          node_count++;
        }

        memcpy((char *)arr + (count * datasize), pred->data, datasize);
        count++;

        pred->right = NULL;
        clibds_avl_reverseorder(arr, datasize, temp_var, count - node_count, count - 1);
        current = current->right;
      }
    }
  }

  free(temp);

  return count;
}


bool clibds_avl_initialize_bysize(avltree_t * const ptr, size_t size_given,
                             int (* comparator_given)(void *, void *))
{
  if ((ptr == NULL) || (size_given == 0) || (comparator_given == NULL))
    return false;

  ptr->root = NULL;
  ptr->comparator = comparator_given;
  ptr->memsize = size_given;
  ptr->size = 0;

  return true;
}

bool clibds_avl_insert(avltree_t * const ptr, void * data_given)
{
  avlnode_t * current, * parent_node, * temp;
  void * dataptr;

  if ((ptr == NULL) || (data_given == NULL))
    return false;

  current = (avlnode_t *) malloc(ptr->memsize + sizeof(avlnode_t));
  if (current == NULL)
    return false;

  dataptr = (void *) (current + 1);

  memcpy(dataptr, data_given, ptr->memsize);

  current->left = NULL;
  current->right = NULL;
  current->hight = 0;
  current->balance_factor = 0;
  current->data = dataptr;

  parent_node = NULL;
  temp = ptr->root;

  while (temp != NULL)
  {
    parent_node = temp;
    if (ptr->comparator(dataptr, temp->data) == -1)
      temp = temp->left;
    else if (ptr->comparator(dataptr, temp->data) == 1)
      temp = temp->right;
    else
    {
      free(current);
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


  temp = current;

  while (temp != NULL)
  {
    clibds_avl_update_node(temp);
    clibds_avl_balance_node(ptr, temp);

    temp = temp->parent;
  }

  ptr->size++;

  return true;
}

bool clibds_avl_remove(avltree_t * const ptr, void * dataptr)
{
  avlnode_t * node_succesor, * current, * parent_node;

  if ((ptr == NULL) || (dataptr == NULL))
    return false;

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
    return false;


  if ((current->left != NULL) && (current->right != NULL))
  {
    if (current->left->hight > current->right->hight)
    {
      node_succesor = current->left;
      while (node_succesor->right != NULL)
        node_succesor = node_succesor->right;

      memcpy(current->data, node_succesor->data, ptr->memsize);

      current = node_succesor;
    }
    else
    {
      node_succesor = current->right;
      while (node_succesor->left != NULL)
        node_succesor = node_succesor->left;

      memcpy(current->data, node_succesor->data, ptr->memsize);

      current = node_succesor;
    }
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
  }
  else if (current->left == NULL)
  {
    if (current == ptr->root)
      ptr->root = current->right;
    else
    {
      if (current == current->parent->left)
        current->parent->left = current->right;
      else
        current->parent->right = current->right;
    }

    current->right->parent = current->parent;
  }
  else
  {
    if (current == ptr->root)
      ptr->root = current->left;
    else
    {
      if (current == current->parent->left)
        current->parent->left = current->left;
      else
        current->parent->right = current->left;
    }

    current->left->parent = current->parent;
  }

  parent_node = current->parent;

  while (parent_node !=  NULL)
  {
    clibds_avl_update_node(parent_node);
    clibds_avl_balance_node(ptr, parent_node);

    parent_node = parent_node->parent;
  }

  free(current);
  ptr->size--;

  return true;
}

size_t clibds_avl_delete(avltree_t * const ptr)
{
  avlnode_t * current, * pred;
  size_t count;

  if (ptr == NULL)
    return 0;

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
    free(pred);
    count++;
  }

  ptr->root = NULL;
  ptr->comparator = NULL;
  ptr->memsize = 0;
  ptr->size = 0;

  return count;
}

bool clibds_avl_viewdata(avltree_t * const ptr, void (* print_data)(void *), int select)
{
  if ((ptr == NULL) || (print_data == NULL))
    return false;

  if (select == PREORDER)
    preorder_rcs_avl(ptr->root, print_data);
  else if (select == INORDER)
    inorder_rcs_avl(ptr->root, print_data);
  else if (select == POSTORDER)
    postorder_rcs_avl(ptr->root, print_data);
  else
    return false;

  return true;
}

size_t clibds_avl_getdata(avltree_t * const ptr, void * arr, int select)
{
  size_t count;

  if ((ptr == NULL) || (arr == NULL))
    return 0;

  if (select == PREORDER)
    count = clibds_avl_preorder(ptr, arr);
  else if (select == INORDER)
    count = clibds_avl_inorder(ptr, arr);
  else if (select == POSTORDER)
    count = clibds_avl_postorder(ptr, arr);
  else
    count = 0;

  return count;
}

void * clibds_avl_search(avltree_t * const ptr, void * dataptr)
{
  avlnode_t * current;

  if ((ptr == NULL) || (dataptr == NULL))
    return NULL;

  current = ptr->root;

  while (current != NULL)
  {
    if (ptr->comparator(dataptr, current->data) == -1)
      current = current->left;
    else if (ptr->comparator(dataptr, current->data) == 1)
      current = current->right;
    else
      return current->data;
  }
  return NULL;
}

void * clibds_avl_minimum(avltree_t * const ptr)
{
  avlnode_t * current;

  if (ptr == NULL)
    return NULL;

  current = ptr->root;

  while (current->left != NULL)
    current = current->left;

  return current->data;
}

void * clibds_avl_maximum(avltree_t * const ptr)
{
  avlnode_t * current;

  if (ptr == NULL)
    return NULL;

  current = ptr->root;

  while (current->right != NULL)
    current = current->right;

  return current->data;
}
