#ifndef CAMERA_RENENGINE_SFW
#define CAMERA_RENENGINE_SFW

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
