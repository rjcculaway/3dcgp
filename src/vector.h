#ifndef VECTOR_3DCGP
#define VECTOR_3DCGP

#include <math.h>

typedef struct vec2
{
  float x;
  float y;
} vec2_t;

typedef struct vec3
{
  float x;
  float y;
  float z;
} vec3_t;

// TODO: Declare functions for manipulating vectors
// Vector 2D Functions
float vec2_length_sq(vec2_t v);
float vec2_length(vec2_t v);
vec2_t vec2_add(vec2_t u, vec2_t v);
vec2_t vec2_sub(vec2_t u, vec2_t v);
vec2_t vec2_mul(vec2_t v, float c);
vec2_t vec2_div(vec2_t v, float c);
float vec2_dot(vec2_t u, vec2_t v);
vec2_t vec2_normalize(vec2_t v);

// Vector 3D Functions
vec3_t vec3_rotate_z(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_x(vec3_t v, float angle);
float vec3_length_sq(vec3_t v);
float vec3_length(vec3_t v);
vec3_t vec3_add(vec3_t u, vec3_t v);
vec3_t vec3_sub(vec3_t u, vec3_t v);
vec3_t vec3_div(vec3_t v, float c);
float vec3_dot(vec3_t u, vec3_t v);
vec3_t vec3_cross(vec3_t u, vec3_t v);
vec3_t vec3_normalize(vec3_t v);

#endif