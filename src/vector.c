#include <stdio.h>
#include "vector.h"

// TODO: Implement functions for manipulating vectors
/**
 * Vector 2D
 */
float vec2_length_sq(vec2_t v)
{
  return v.x * v.x + v.y * v.y;
}

float vec2_length(vec2_t v)
{
  return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t u, vec2_t v)
{
  vec2_t result = {
      .x = u.x + v.x,
      .y = u.y + v.y,
  };
  return result;
}

vec2_t vec2_sub(vec2_t u, vec2_t v)
{
  vec2_t result = {
      .x = u.x - v.x,
      .y = u.y - v.y,
  };
  return result;
}

vec2_t vec2_mul(vec2_t v, float c)
{
  vec2_t result = {
      .x = c * v.x,
      .y = c * v.y,
  };

  return result;
}

vec2_t vec2_div(vec2_t v, float c)
{
  vec2_t result = {
      .x = v.x / c,
      .y = v.y / c,
  };

  return result;
}

float vec2_dot(vec2_t u, vec2_t v)
{
  return u.x * v.x + u.y * v.y;
}

vec2_t vec2_normalize(vec2_t v)
{
  return vec2_div(v, vec2_length(v));
}

/**
 * Vector 3D
 */

float vec3_length_sq(vec3_t v)
{
  return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vec3_length(vec3_t v)
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t u, vec3_t v)
{
  vec3_t result = {
      .x = u.x + v.x,
      .y = u.y + v.y,
      .z = u.z + v.z,
  };
  return result;
}

vec3_t vec3_sub(vec3_t u, vec3_t v)
{
  vec3_t result = {
      .x = u.x - v.x,
      .y = u.y - v.y,
      .z = u.z - v.z,
  };
  return result;
}

vec3_t vec3_mul(vec3_t v, float c)
{
  vec3_t result = {
      .x = c * v.x,
      .y = c * v.y,
      .z = c * v.z};

  return result;
}

vec3_t vec3_div(vec3_t v, float c)
{
  vec3_t result = {
      .x = v.x / c,
      .y = v.y / c,
      .z = v.z / c};

  return result;
}

float vec3_dot(vec3_t u, vec3_t v)
{
  return u.x * v.x + u.y * v.y + u.z * v.z;
}

vec3_t vec3_rotate_x(vec3_t v, float angle)
{
  float cos_angle = cos(angle);
  float sin_angle = sin(angle);
  vec3_t rotated_vector = {
      .x = v.x,
      .y = v.y * cos_angle - v.z * sin_angle,
      .z = v.y * sin_angle + v.z * cos_angle};
  return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle)
{
  float cos_angle = cos(angle);
  float sin_angle = sin(angle);
  vec3_t rotated_vector = {
      .x = v.x * cos_angle - v.z * sin_angle,
      .y = v.y,
      .z = v.x * sin_angle + v.z * cos_angle};
  return rotated_vector;
}

vec3_t vec3_rotate_z(vec3_t v, float angle)
{
  float cos_angle = cos(angle);
  float sin_angle = sin(angle);
  vec3_t rotated_vector = {
      .x = v.x * cos_angle - v.y * sin_angle,
      .y = v.x * sin_angle + v.y * cos_angle,
      .z = v.z};
  return rotated_vector;
}

vec3_t vec3_cross(vec3_t u, vec3_t v)
{
  vec3_t result = {
      .x = u.y * v.z - u.z * v.y,
      .y = u.z * v.x - u.x * v.z,
      .z = u.x * v.y - u.y * v.x};

  return result;
}

vec3_t vec3_normalize(vec3_t v)
{
  return vec3_div(v, vec3_length(v));
}

/**
 * Vector 4D
 */

// Converts a vec3_t to a vec4_t
vec4_t vec4_from_vec3(vec3_t v)
{
  vec4_t u = {
      .x = v.x,
      .y = v.y,
      .z = v.z,
      .w = 1.0};
  return u;
}

vec3_t vec3_from_vec4(vec4_t v)
{
  vec3_t u = {
      .x = v.x,
      .y = v.y,
      .z = v.z};
  return u;
}

float vec4_length_sq(vec4_t v)
{
  return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

float vec4_length(vec4_t v)
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

vec4_t vec4_add(vec4_t u, vec4_t v)
{
  vec4_t result = {
      .x = u.x + v.x,
      .y = u.y + v.y,
      .z = u.z + v.z,
      .w = u.w + v.w,
  };
  return result;
}

vec4_t vec4_sub(vec4_t u, vec4_t v)
{
  vec4_t result = {
      .x = u.x - v.x,
      .y = u.y - v.y,
      .z = u.z - v.z,
      .w = u.w - v.w,
  };
  return result;
}

vec4_t vec4_mul(vec4_t v, float c)
{
  vec4_t result = {
      .x = c * v.x,
      .y = c * v.y,
      .z = c * v.z,
      .w = c * v.w};

  return result;
}

vec4_t vec4_div(vec4_t v, float c)
{
  vec4_t result = {
      .x = v.x / c,
      .y = v.y / c,
      .z = v.z / c,
      .w = v.w / c};

  return result;
}

float vec4_dot(vec4_t u, vec4_t v)
{
  return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

vec4_t vec4_rotate_x(vec4_t v, float angle)
{
  float cos_angle = cos(angle);
  float sin_angle = sin(angle);
  vec4_t rotated_vector = {
      .x = v.x,
      .y = v.y * cos_angle - v.z * sin_angle,
      .z = v.y * sin_angle + v.z * cos_angle,
      .w = v.w};
  return rotated_vector;
}

vec4_t vec4_rotate_y(vec4_t v, float angle)
{
  float cos_angle = cos(angle);
  float sin_angle = sin(angle);
  vec4_t rotated_vector = {
      .x = v.x * cos_angle - v.z * sin_angle,
      .y = v.y,
      .z = v.x * sin_angle + v.z * cos_angle,
      .w = v.w};
  return rotated_vector;
}

vec4_t vec4_rotate_z(vec4_t v, float angle)
{
  float cos_angle = cos(angle);
  float sin_angle = sin(angle);
  vec4_t rotated_vector = {
      .x = v.x * cos_angle - v.y * sin_angle,
      .y = v.x * sin_angle + v.y * cos_angle,
      .z = v.z,
      .w = v.w};
  return rotated_vector;
}

vec4_t vec4_cross(vec4_t u, vec4_t v)
{
  vec4_t result = {
      .x = u.y * v.z - u.z * v.y,
      .y = u.z * v.x - u.x * v.z,
      .z = u.x * v.y - u.y * v.x,
      .w = u.w};
  return result;
}

vec4_t vec4_normalize(vec4_t v)
{
  return vec4_div(v, vec4_length(v));
}