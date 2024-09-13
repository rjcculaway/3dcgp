#include "clipping.h"

plane_t frustum_planes[NUM_FRUSTUM_PLANES];

void initialize_frustum_planes(float fovx, float fovy, float z_near, float z_far)
{
  float sin_half_angle_y = sin(fovy / 2);
  float sin_half_angle_x = sin(fovx / 2);
  float cos_half_angle_y = sin(fovy / 2);
  float cos_half_angle_x = sin(fovx / 2);

  // Left Frustum Plane
  frustum_planes[LEFT_FRUSTUM_PLANE].point = vec3_create(0, 0, 0);
  frustum_planes[LEFT_FRUSTUM_PLANE].normal = vec3_create(cos_half_angle_x, 0, sin_half_angle_x);

  // Right Frustum Plane
  frustum_planes[RIGHT_FRUSTUM_PLANE].point = vec3_create(0, 0, 0);
  frustum_planes[RIGHT_FRUSTUM_PLANE].normal = vec3_create(-cos_half_angle_x, 0, sin_half_angle_x);

  // Top Frustum Plane
  frustum_planes[TOP_FRUSTUM_PLANE].point = vec3_create(0, 0, 0);
  frustum_planes[TOP_FRUSTUM_PLANE].normal = vec3_create(0, -cos_half_angle_y, sin_half_angle_y);

  // Bottom Frustum Plane
  frustum_planes[BOTTOM_FRUSTUM_PLANE]
      .point = vec3_create(0, 0, 0);
  frustum_planes[BOTTOM_FRUSTUM_PLANE].normal = vec3_create(0, cos_half_angle_y, sin_half_angle_y);

  // Near Frustum Plane
  frustum_planes[NEAR_FRUSTUM_PLANE].point = vec3_create(0, 0, z_near);
  frustum_planes[NEAR_FRUSTUM_PLANE].normal = vec3_create(0, 0, 1);

  // Far Frustum Plane
  frustum_planes[FAR_FRUSTUM_PLANE].point = vec3_create(0, 0, z_far);
  frustum_planes[FAR_FRUSTUM_PLANE].normal = vec3_create(0, 0, -1);

  return;
}

polygon_t polygon_from_triangle(vec3_t v0, vec3_t v1, vec3_t v2)
{
  polygon_t triangle = {
      .count = 3,
      .vertices = {
          v0, v1, v2}};

  return triangle;
}

void clip_polygon_against_plane(polygon_t *polygon, plane_t plane)
{
  vec3_t inside[MAX_NUM_POLYGON_VERTICES];
  int inside_count = 0;

  vec3_t previous_vertex = polygon->vertices[polygon->count - 1]; // Previous vertex starts at the last vertex
  float previous_vertex_dot = vec3_dot(vec3_sub(previous_vertex, plane.point), plane.normal);

  // Loop through each vertex
  for (int i = 0; i < polygon->count; i++)
  {
    vec3_t current_vertex = polygon->vertices[i];
    float current_vertex_dot = vec3_dot(vec3_sub(current_vertex, plane.point), plane.normal);

    // If the previous is outside and the current is inside and vice versa
    bool is_previous_different = previous_vertex_dot * current_vertex_dot < 0;
    if (is_previous_different)
    {
      // Compute the interpolation point t; t = dot(Qp) / (dot(Qp) - dot(Qc))
      float t = previous_vertex_dot / (previous_vertex_dot - current_vertex_dot);
      // Using t, get the intersection point between the plane and the line formed by the two vertices:
      // I = Qp + t(Qc - Qp) -- A linear interpolation between Qp and Qc using t as the interpolation factor
      vec3_t intersection = vec3_add(previous_vertex, vec3_mul(vec3_sub(current_vertex, previous_vertex), t));

      inside[inside_count] = intersection;
      inside_count++;
    }

    bool is_current_vertex_inside = current_vertex_dot > 0.0;
    if (is_current_vertex_inside)
    {
      inside[inside_count] = current_vertex;
      inside_count++;
    }

    previous_vertex = current_vertex;
    previous_vertex_dot = current_vertex_dot;
  }
  polygon->count = inside_count;
  // memset(polygon->vertices, 0, sizeof(vec3_t) * MAX_NUM_POLYGON_VERTICES);
  memcpy(polygon->vertices, inside, sizeof(vec3_t) * inside_count); // Overwrite the current vertices of the polygon with all inside vertices
}

void triangles_from_polygon(polygon_t *polygon, triangle_t triangles_after_clipping[MAX_NUM_POLYGON_TRIANGLES], int *num_triangles_after_clipping)
{
  if (!polygon)
  {
    fprintf(stderr, "polygon not provided\n");
    return;
  }
  if (!triangles_after_clipping)
  {
    fprintf(stderr, "triangles_after_clipping not provided\n");
    return;
  }
  if (!num_triangles_after_clipping)
  {
    fprintf(stderr, "num_triangles_after_clipping not provided\n");
    return;
  }

  vec4_t v0;
  if (polygon->count > 0)
  {
    v0 = vec4_from_vec3(polygon->vertices[0]);
  }
  for (int i = 0; i < polygon->count - 2; i++)
  {
    triangle_t triangle;
    // (v1, v2), (v2, v3), (v3, v4)...
    triangle.points[0] = v0;
    triangle.points[1] = vec4_from_vec3(polygon->vertices[i + 1]);
    triangle.points[2] = vec4_from_vec3(polygon->vertices[i + 2]);
    triangles_after_clipping[i] = triangle;
    (*num_triangles_after_clipping) += 1;
  }
}

void clip_polygon(polygon_t *polygon)
{
  // Previous clipping output is used as input for the next clipping
  clip_polygon_against_plane(polygon, frustum_planes[LEFT_FRUSTUM_PLANE]);
  clip_polygon_against_plane(polygon, frustum_planes[RIGHT_FRUSTUM_PLANE]);
  clip_polygon_against_plane(polygon, frustum_planes[TOP_FRUSTUM_PLANE]);
  clip_polygon_against_plane(polygon, frustum_planes[BOTTOM_FRUSTUM_PLANE]);
  clip_polygon_against_plane(polygon, frustum_planes[NEAR_FRUSTUM_PLANE]);
  clip_polygon_against_plane(polygon, frustum_planes[FAR_FRUSTUM_PLANE]);
}