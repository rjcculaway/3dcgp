#include "clipping.h"

plane_t frustum_planes[NUM_FRUSTUM_PLANES];

void initialize_frustum_planes(float fov, float z_near, float z_far)
{
  float sin_half_angle = sin(fov / 2);
  float cos_half_angle = sin(fov / 2);

  // Left Frustum Plane
  frustum_planes[LEFT_FRUSTUM_PLANE].point = vec3_create(0, 0, 0);
  frustum_planes[LEFT_FRUSTUM_PLANE].normal = vec3_create(cos_half_angle, 0, sin_half_angle);

  // Right Frustum Plane
  frustum_planes[RIGHT_FRUSTUM_PLANE].point = vec3_create(0, 0, 0);
  frustum_planes[RIGHT_FRUSTUM_PLANE].normal = vec3_create(-cos_half_angle, 0, sin_half_angle);

  // Top Frustum Plane
  frustum_planes[TOP_FRUSTUM_PLANE].point = vec3_create(0, 0, 0);
  frustum_planes[TOP_FRUSTUM_PLANE].normal = vec3_create(0, -cos_half_angle, sin_half_angle);

  // Bottom Frustum Plane
  frustum_planes[BOTTOM_FRUSTUM_PLANE]
      .point = vec3_create(0, 0, 0);
  frustum_planes[BOTTOM_FRUSTUM_PLANE].normal = vec3_create(0, cos_half_angle, sin_half_angle);

  // Near Frustum Plane
  frustum_planes[NEAR_FRUSTUM_PLANE].point = vec3_create(0, 0, z_near);
  frustum_planes[NEAR_FRUSTUM_PLANE].normal = vec3_create(0, 0, 1);

  // Far Frustum Plane
  frustum_planes[FAR_FRUSTUM_PLANE].point = vec3_create(0, 0, z_far);
  frustum_planes[FAR_FRUSTUM_PLANE].normal = vec3_create(0, 0, -1);

  return;
}