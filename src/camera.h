#ifndef CAMERA_3DCGP
#define CAMERA_3DCGP

#include "vector.h"
#include "matrix.h"

typedef struct camera
{
  vec3_t position;
  vec3_t direction;
  float aspect_ratio;
  float fov_angle;
  float z_far;
  float z_near;
} camera_t;

extern camera_t camera;

#endif
