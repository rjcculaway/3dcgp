#ifndef CAMERA_3DCGP
#define CAMERA_3DCGP

#include "vector.h"

typedef struct camera
{
  vec3_t position;
  vec3_t rotation;
  float fov_angle;
} camera_t;

#endif
