#ifndef LIGHT_3DCGP
#define LIGHT_3DCGP

#include <math.h>
#include "vector.h"
#include "display.h"

extern const color_t ARGB8888_A_BITMASK;
extern const color_t ARGB8888_R_BITMASK;
extern const color_t ARGB8888_G_BITMASK;
extern const color_t ARGB8888_B_BITMASK;

typedef struct sun_light
{
  vec3_t direction;
} sun_light_t;

color_t light_apply_intensity(color_t original_color, float intensity);

float light_lambertian(vec3_t face_normal, vec3_t light_direction);

#endif