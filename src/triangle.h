#ifndef TRIANGLE_RENENGINE_SFW
#define TRIANGLE_RENENGINE_SFW

#include <stdint.h>

#include "vector.h"
#include "display.h"
#include "texture.h"
#include "../upng/upng.h"
#include "utils.h"

typedef struct face
{
  int a;
  int b;
  int c;
  tex2_t a_uv;
  tex2_t b_uv;
  tex2_t c_uv;
  color_t color;
} face_t;

// A barycentric interpolation encapsulates the result of interpolating the UVs and the inverse w.
typedef struct barycentric_interpolation
{
  vec2_t p;
  vec2_t uv;
  vec2_t inv_w;
} barycentric_interpolation_t;

typedef struct triangle
{
  vec4_t points[3];
  tex2_t texcoords[3];
  color_t color;
  upng_t *texture;
} triangle_t;

void fill_flat_bottom_triangle(triangle_t triangle, color_t color);
void fill_flat_top_triangle(triangle_t triangle, color_t color);
void draw_filled_triangle(triangle_t triangle, color_t color);

void draw_triangle_pixel(int xi, int yi,
                         vec4_t point_a, vec4_t point_b, vec4_t point_c,
                         float inv_w_a, float inv_w_b, float inv_w_c,
                         color_t color);
void draw_texel(int xi, int yi,
                vec4_t point_a, vec4_t point_b, vec4_t point_c,
                float inv_w_a, float inv_w_b, float inv_w_c,
                tex2_t uv_a, tex2_t uv_b, tex2_t uv_c, upng_t *texture);
void sort_three_vertices_uv_by_y(triangle_t *triangle);
void draw_textured_triangle(
    triangle_t triangle,
    upng_t *texture);

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);

vec3_t compute_triangle_normal(vec4_t points[3]);

#endif