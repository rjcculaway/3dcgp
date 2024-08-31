#ifndef TRIANGLE_3DCGP
#define TRIANGLE_3DCGP

#include <stdint.h>

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

void draw_flat_bottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_flat_top(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif