#ifndef LIGHT_RENENGINE_SFW
#define LIGHT_RENENGINE_SFW

#include "vector.h"
#include "display.h"
#include "math.h"
#include "utils.h"

extern const color_t RGBA8888_A_BITMASK;
extern const color_t RGBA8888_R_BITMASK;
extern const color_t RGBA8888_G_BITMASK;
extern const color_t RGBA8888_B_BITMASK;

typedef struct sun_light
{
  vec3_t direction;
} sun_light_t;

extern sun_light_t sunlight;

color_t light_apply_intensity(color_t original_color, float intensity);
float light_lambertian(vec3_t face_normal, vec3_t light_direction);

#endif