#ifndef TRIANGLE_3DCGP
#define TRIANGLE_3DCGP

#include <stdint.h>

#include "vector.h"
#include "display.h"

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

void sort_three_vertices_by_y(int vertices[3][2]);
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);

#endif