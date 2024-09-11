#ifndef TRIANGLE_3DCGP
#define TRIANGLE_3DCGP

#include <stdint.h>

#include "vector.h"
#include "display.h"
#include "texture.h"
#include "utils.h"

typedef struct face
{
  int a;
  int b;
  int c;
  int a_uv;
  int b_uv;
  int c_uv;
  color_t color;
} face_t;

typedef struct triangle
{
  vec4_t points[3];
  tex2_t texcoords[3];
  color_t color;
  float depth;
} triangle_t;

void mergesort_triangle_merge(triangle_t *a, triangle_t *b, size_t n, size_t start, size_t mid, size_t end);
void mergesort_triangle_partition(triangle_t *a, triangle_t *b, size_t n, size_t start, size_t end);
void mergesort_triangle_by_depth(triangle_t *triangles);
void insertion_sort_triangle_by_depth(triangle_t *triangles);
void bubble_sort_triangle_by_depth(triangle_t *triangles);

void sort_three_vertices_by_y(int vertices[3][2]);
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);
void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);

void draw_texel(int xi, int yi,
                vec4_t point_a, vec4_t point_b, vec4_t point_c,
                tex2_t uv_a, tex2_t uv_b, tex2_t uv_c, color_t *texture);
void sort_three_vertices_uv_by_y(triangle_t *triangle);
void draw_textured_triangle(
    triangle_t triangle,
    color_t *texture);

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);

#endif