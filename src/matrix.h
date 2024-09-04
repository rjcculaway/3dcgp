#ifndef MATRIX_3DCGP
#define MATRIX_3DCGP

#include "vector.h"

typedef struct
{
  float m[4][4];
} mat4_t;

mat4_t mat4_identity(void);
mat4_t mat4_make_scale(float sx, float sy, float sz);
mat4_t inline mat4_make_scale_uniform(float s);
vec4_t mat4_matmul_vec(mat4_t m, vec4_t v);
mat4_t mat4_make_translation(float tx, float ty, float tz);
mat4_t mat4_make_rotation_x(float angle);
mat4_t mat4_make_rotation_y(float angle);
mat4_t mat4_make_rotation_z(float angle);

#endif