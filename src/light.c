#include "light.h"

const color_t RGBA32_A_BITMASK = 0xFF000000;
const color_t RGBA32_R_BITMASK = 0x00FF0000;
const color_t RGBA32_G_BITMASK = 0x0000FF00;
const color_t RGBA32_B_BITMASK = 0x000000FF;

static sun_light_t sunlight = {
    .direction = {0.0, 0.0, 0.0}};

void initialize_light(vec3_t direction)
{
  sunlight.direction = direction;
}
sun_light_t get_sun_light()
{
  return sunlight;
}

color_t interpolate(float alpha, float beta, float gamma, color_t c0, color_t c1, color_t c2)
{
  color_t r = (c0 & RGBA32_R_BITMASK) * alpha + (c1 & RGBA32_R_BITMASK) * beta + (c2 & RGBA32_R_BITMASK) * gamma;
  color_t g = (c0 & RGBA32_G_BITMASK) * alpha + (c1 & RGBA32_G_BITMASK) * beta + (c2 & RGBA32_G_BITMASK) * gamma;
  color_t b = (c0 & RGBA32_B_BITMASK) * alpha + (c1 & RGBA32_B_BITMASK) * beta + (c2 & RGBA32_B_BITMASK) * gamma;
  color_t a = (c0 & RGBA32_A_BITMASK) * alpha + (c1 & RGBA32_A_BITMASK) * beta + (c2 & RGBA32_A_BITMASK) * gamma;

  return (a & RGBA32_A_BITMASK) | (b & RGBA32_B_BITMASK) | (g & RGBA32_G_BITMASK) | (r & RGBA32_R_BITMASK);
}

color_t light_apply_intensity(color_t original_color, float intensity)
{
  color_t r = (original_color & RGBA32_R_BITMASK) * intensity;
  color_t g = (original_color & RGBA32_G_BITMASK) * intensity;
  color_t b = (original_color & RGBA32_B_BITMASK) * intensity;
  color_t a = (original_color & RGBA32_A_BITMASK);

  return (a & RGBA32_A_BITMASK) | (b & RGBA32_B_BITMASK) | (g & RGBA32_G_BITMASK) | (r & RGBA32_R_BITMASK);
}

float light_lambertian(vec3_t face_normal, vec3_t light_direction)
{
  return fclamp(0.0, 1.0, -vec3_dot(face_normal, light_direction));
}
