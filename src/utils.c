#include "utils.h"
#include <assert.h>
void mergesort_triangle_merge(triangle_t *a, triangle_t *b, size_t n, size_t start, size_t mid, size_t end)
{
  size_t i = start;
  size_t j = mid;

  for (size_t k = start; k < end; k++)
  {
    if (i < mid && (j >= end || a[i].depth < b[j].depth))
    {
      b[k] = a[i];
      i++;
    }
    else
    {
      b[k] = a[j];
      j++;
    }
  }
}

void mergesort_triangle_partition(triangle_t *a, triangle_t *b, size_t n, size_t start, size_t end)
{
  if (end <= 1 + start) // end - start <= 1, meaning that the partition size is 1
  {
    return;
  }

  size_t mid = (start + end) / 2;

  mergesort_triangle_partition(b, a, n, start, mid);
  mergesort_triangle_partition(b, a, n, mid, end);

  mergesort_triangle_merge(b, a, n, start, mid, end);
}

void mergesort_triangle_by_depth(triangle_t *triangles)
{
  size_t n = array_length(triangles);
  triangle_t *b = (triangle_t *)malloc(sizeof(triangle_t) * n);
  memcpy(b, triangles, sizeof(triangle_t) * n);

  mergesort_triangle_partition(triangles, b, n, 0, n);

  free(b);

  assert(n == array_length(triangles));
  // printf("\n");
  // for (int i = 0; i < n; i++)
  // {
  //   printf("%f\t", triangles[i].depth);
  // }
  // printf("\n\n");
}