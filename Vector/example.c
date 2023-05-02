/*
  Copyright (c) 2023, Arka Mondal. All rights reserved.
  Use of this source code is governed by a BSD-style license that
  can be found in the LICENSE file.
*/

#include <stdio.h>
#include "vector.h"

int main(void)
{
  int arr[6] = {6, 7, 1, 2, 8, 3};
  vector_t new_vec;

  clibds_vec_init(&new_vec, int, NULL);

  for (int i = 0; i < 6; i++)
    clibds_vec_pushback(&new_vec, arr + i);

  clibds_vec_foreach(&new_vec, it)
  {
    printf("%d\n", *(int *) *it);
  }

  clibds_vec_delete(&new_vec);

  return 0;
}
