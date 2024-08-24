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

vec3_t vec3_rotate_z(vec3_t v, float angle);
vec3_t vec3_rotate_y(vec3_t v, float angle);
vec3_t vec3_rotate_x(vec3_t v, float angle);

#endif