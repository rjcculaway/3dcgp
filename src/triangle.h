#ifndef TRIANGLE_3DCGP
#define TRIANGLE_3DCGP

#include "vector.h"

typedef struct face
{
  int a;
  int b;
  int c;
} face_t;

typedef struct triangle
{
  vec2_t points[3];
} triangle_t;

#endif