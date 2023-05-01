/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdlib.h>
#include <string.h>
#include "bstree.h"

static void preorder_rcs_bst(bstnode_t * nodeptr, void (* print_data)(void *))
{
  if (nodeptr == NULL)
    return;
  print_data(nodeptr->data);
  preorder_rcs_bst(nodeptr->left, print_data);
  preorder_rcs_bst(nodeptr->right, print_data);
}

static void inorder_rcs_bst(bstnode_t * nodeptr, void (* print_data)(void *))
{
  if (nodeptr == NULL)
    return;
  inorder_rcs_bst(nodeptr->left, print_data);
  print_data(nodeptr->data);
  inorder_rcs_bst(nodeptr->right, print_data);
}

static void postorder_rcs_bst(bstnode_t * nodeptr, void (* print_data)(void *))
{
  if (nodeptr == NULL)
    return;
  postorder_rcs_bst(nodeptr->left, print_data);
  postorder_rcs_bst(nodeptr->right, print_data);
  print_data(nodeptr->data);
}

static void clibds_bst_reverseorder(void * arr, size_t datasize, void * temp,
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

static size_t clibds_bst_preorder(binaryst_t * const ptr, void * arr)
{
  bstnode_t * current, * pred;
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

static size_t clibds_bst_inorder(binaryst_t * const ptr, void * arr)
{
  bstnode_t * current, * pred;
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

static size_t clibds_bst_postorder(binaryst_t * const ptr, void * arr)
{
  void * temp_var;
  bstnode_t * temp, * current, * pred;
  size_t count, datasize, node_count;

  count = 0;
  datasize = ptr->memsize;

  temp = (bstnode_t *) malloc(datasize + sizeof(bstnode_t));
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
        clibds_bst_reverseorder(arr, datasize, temp_var, count - node_count, count - 1);
        current = current->right;
      }
    }
  }

  free(temp);

  return count;
}

bool clibds_bst_initialize_bysize(binaryst_t * const ptr, size_t size_given,
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

bool clibds_bst_insert(binaryst_t * const ptr, void * data_given)
{
  void * dataptr;
  bstnode_t * current, * temp, * parent_node;

  if ((ptr == NULL) || (data_given == NULL))
    return false;

  temp = ptr->root;
  parent_node = NULL;

  current = (bstnode_t *) malloc(ptr->memsize + sizeof(bstnode_t));
  if (current == NULL)
    return false;

  dataptr = (void *) (current + 1);

  memcpy(dataptr, data_given, ptr->memsize);

  current->data = dataptr;
  current->left = NULL;
  current->right = NULL;

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

  ptr->size++;

  return true;
}

bool clibds_bst_viewdata(binaryst_t * ptr, void (* print_data)(void *), int select)
{
  if (print_data == NULL)
    return false;

  if (select == PREORDER)
    preorder_rcs_bst(ptr->root, print_data);
  else if (select == INORDER)
    inorder_rcs_bst(ptr->root, print_data);
  else if (select == POSTORDER)
    postorder_rcs_bst(ptr->root, print_data);
  else
    return false;

  return true;
}

size_t clibds_bst_getdata(binaryst_t * ptr, void * arr, int select)
{
  size_t count;

  if ((ptr == NULL) || (arr == NULL))
    return 0;

  if (select == PREORDER)
    count = clibds_bst_preorder(ptr, arr);
  else if (select == INORDER)
    count = clibds_bst_inorder(ptr, arr);
  else if (select == POSTORDER)
    count = clibds_bst_postorder(ptr, arr);
  else
    count = 0;

  return count;
}

bool clibds_bst_remove(binaryst_t * const ptr, void * dataptr)
{
  bstnode_t * current, * temp, * child_node;

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
    temp = current->left;
    while (temp->right != NULL)
      temp = temp->right;

    memcpy(current->data, temp->data, ptr->memsize);

    current = temp;
  }

  if ((current->left == NULL) && (current->right == NULL))
  {
    if (ptr->root == current)
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
    child_node = current->right;

    if (current->parent == NULL)
      ptr->root = child_node;
    else if (current == current->parent->right)
      current->parent->right = child_node;
    else
      current->parent->left = child_node;

    child_node->parent = current->parent;
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
  }

  free(current);

  ptr->size--;

  return true;
}


size_t clibds_bst_delete(binaryst_t * const ptr)
{
  bstnode_t * current, * pred;
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

void * clibds_bst_minimum(binaryst_t * const ptr)
{
  bstnode_t * current;

  if (ptr == NULL)
    return NULL;

  current = ptr->root;

  while (current->left != NULL)
    current = current->left;

  return current->data;
}

void * clibds_bst_maximum(binaryst_t * const ptr)
{
  bstnode_t * current;

  if (ptr == NULL)
    return NULL;

  current = ptr->root;

  while (current->right != NULL)
    current = current->right;

  return current->data;
}
