#include "light.h"

const color_t ARGB8888_A_BITMASK = 0xFF000000;
const color_t ARGB8888_R_BITMASK = 0x00FF0000;
const color_t ARGB8888_G_BITMASK = 0x0000FF00;
const color_t ARGB8888_B_BITMASK = 0x000000FF;

sun_light_t sunlight = {
    .direction = {0.0, 0.0, 1.0}};

color_t light_apply_intensity(color_t original_color, float intensity)
{
  color_t a = (original_color & ARGB8888_A_BITMASK);
  color_t r = (original_color & ARGB8888_R_BITMASK) * intensity;
  color_t g = (original_color & ARGB8888_G_BITMASK) * intensity;
  color_t b = (original_color & ARGB8888_B_BITMASK) * intensity;

  return a | (r & ARGB8888_R_BITMASK) | (g & ARGB8888_G_BITMASK) | (b & ARGB8888_B_BITMASK);
}

float light_lambertian(vec3_t face_normal, vec3_t light_direction)
{
  return fclamp(0.0, 1.0, -vec3_dot(face_normal, light_direction));
}
