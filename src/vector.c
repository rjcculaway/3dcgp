#include "vector.h"

// TODO: Implement functions for manipulating vectors
/**
 * Vector Rotation Functions
 * - Freeze one axis, then rotate
 */
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
