#include "light.h"

const color_t ARGB8888_A_BITMASK = 0xFF000000;
const color_t ARGB8888_R_BITMASK = 0x00FF0000;
const color_t ARGB8888_G_BITMASK = 0x0000FF00;
const color_t ARGB8888_B_BITMASK = 0x000000FF;

color_t light_apply_intensity(color_t original_color, float intensity)
{
  // Separate components and multiply by intensity
  color_t a = (ARGB8888_A_BITMASK & original_color);
  color_t r = (ARGB8888_R_BITMASK & original_color) * intensity;
  color_t g = (ARGB8888_G_BITMASK & original_color) * intensity;
  color_t b = (ARGB8888_B_BITMASK & original_color) * intensity;

  return a | (r & ARGB8888_R_BITMASK) | (g & ARGB8888_G_BITMASK) | (b & ARGB8888_B_BITMASK);
}

float light_lambertian(vec3_t face_normal, vec3_t light_direction)
{
  return fmax(0, vec3_dot(face_normal, light_direction));
}