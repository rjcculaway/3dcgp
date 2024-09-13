#ifndef CLIPPING_RENENGINE_SFW
#define CLIPPING_RENENGINE_SFW

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "vector.h"
#include "triangle.h"

#define MAX_NUM_POLYGON_VERTICES 10
#define MAX_NUM_POLYGON_TRIANGLES 10
#define NUM_FRUSTUM_PLANES 6

typedef enum frustum_plane
{
  LEFT_FRUSTUM_PLANE,
  RIGHT_FRUSTUM_PLANE,
  TOP_FRUSTUM_PLANE,
  BOTTOM_FRUSTUM_PLANE,
  NEAR_FRUSTUM_PLANE,
  FAR_FRUSTUM_PLANE,
} frustum_plane;

typedef struct plane
{
  vec3_t point;
  vec3_t normal;
} plane_t;

typedef struct polygon
{
  vec3_t vertices[MAX_NUM_POLYGON_VERTICES];
  int count;
} polygon_t;

extern plane_t frustum_planes[NUM_FRUSTUM_PLANES];

void initialize_frustum_planes(float fov, float z_near, float z_far);
polygon_t polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2);
void clip_polygon_against_plane(polygon_t *polygon, plane_t plane);
void clip_polygon(polygon_t *polygon);
void triangles_from_polygon(polygon_t *polygon, triangle_t triangles_after_clipping[MAX_NUM_POLYGON_TRIANGLES], int *num_triangles_after_clipping);

#endif
