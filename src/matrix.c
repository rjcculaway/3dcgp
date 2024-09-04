#include "matrix.h"

mat4_t mat4_identity(void)
{
  // | 1 0 0 0 |
  // | 0 1 0 0 |
  // | 0 0 1 0 |
  // | 0 0 0 1 |
  mat4_t m = {
      .m = {
          {1.0, 0.0, 0.0, 0.0},
          {0.0, 1.0, 0.0, 0.0},
          {0.0, 0.0, 1.0, 0.0},
          {0.0, 0.0, 0.0, 1.0},
      }};

  return m;
}

mat4_t mat4_make_scale(float sx, float sy, float sz)
{
  // | sx 0  0  0 |
  // | 0  sy 0  0 |
  // | 0  0  sz 0 |
  // | 0  0  0  1 |
  mat4_t scale_mat = mat4_identity();

  scale_mat.m[0][0] = sx;
  scale_mat.m[1][1] = sy;
  scale_mat.m[2][2] = sz;

  return scale_mat;
}

mat4_t inline mat4_make_scale_uniform(float s)
{
  // | s 0 0 0 |
  // | 0 s 0 0 |
  // | 0 0 s 0 |
  // | 0 0 0 1 |
  return mat4_make_scale(s, s, s);
}

vec4_t mat4_matmul_vec(mat4_t m, vec4_t v)
{
  vec4_t result;

  result.x = m.m[0][0] * v.x + m.m[0][1] * v.y + m.m[0][2] * v.z + m.m[0][3] * v.w;
  result.y = m.m[1][0] * v.x + m.m[1][1] * v.y + m.m[1][2] * v.z + m.m[1][3] * v.w;
  result.z = m.m[2][0] * v.x + m.m[2][1] * v.y + m.m[2][2] * v.z + m.m[2][3] * v.w;
  result.w = m.m[3][0] * v.x + m.m[3][1] * v.y + m.m[3][2] * v.z + m.m[3][3] * v.w;

  return result;
}