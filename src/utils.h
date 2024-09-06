#ifndef UTILS
#define UTILS

#include "triangle.h"
#include "array.h"
#include "math.h"
#include <assert.h>

void mergesort_triangle_merge(triangle_t *a, triangle_t *b, size_t n, size_t start, size_t mid, size_t end);
void mergesort_triangle_partition(triangle_t *a, triangle_t *b, size_t n, size_t start, size_t end);
void mergesort_triangle_by_depth(triangle_t *triangles);

float deg_to_rad(float deg);
float rad_to_deg(float rad);

double fclamp(double minimum, double maximum, double value);

#endif