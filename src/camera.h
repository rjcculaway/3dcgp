#ifndef CAMERA_3DCGP
#define CAMERA_3DCGP

#include "vector.h"
#include "matrix.h"

typedef struct camera
{
  vec3_t position;
  vec3_t direction;
  vec3_t forward_velocity;
  float yaw;
} camera_t;

extern camera_t camera;
extern const vec3_t CAMERA_UP;

#endif
