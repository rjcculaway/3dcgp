#ifndef CLIPPING_RENENGINE_SFW
#define CLIPPING_RENENGINE_SFW

#include "vector.h"
#include <stdbool.h>

#define NUM_FRUSTUM_PLANES 6

typedef enum frustum_plane
{
  LEFT_FRUSTUM_PLANE,
  RIGHT_FRUSTUM_PLANE,
  TOP_FRUSTUM_PLANE,
  BOTTOM_FRUSTUM_PLANE,
  NEAR_FRUSTUM_PLANE,
  FAR_FRUSTUM_PLANE,
};

typedef struct plane
{
  vec3_t point;
  vec3_t normal;
} plane_t;

extern plane_t frustum_planes[NUM_FRUSTUM_PLANES];

void initialize_frustum_planes(float fov, float z_near, float z_far);

#endif
