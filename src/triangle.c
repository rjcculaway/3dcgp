#include "triangle.h"
#include "display.h"
#include "texture.h"

void sort_three_vertices_by_y(int vertices[3][2]) // Insertion Sort
{
  for (int slot = 1; slot < 3; slot++)
  {
    int i = slot - 1;
    int x = vertices[i + 1][0];
    int y = vertices[i + 1][1];
    while (i >= 0 && vertices[i][1] > y) // Sort by y-value
    {
      vertices[i + 1][0] = vertices[i][0];
      vertices[i + 1][1] = vertices[i][1];
      i--;
    }
    vertices[i + 1][0] = x;
    vertices[i + 1][1] = y;
  }
}

// Draw a filled triangle with a flat bottom
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color)
{
  float inv_slope1 = ((float)(x1 - x0)) / (y1 - y0); // Inverse slope (run over rise), because in this case y is the independent value
  float inv_slope2 = ((float)(x2 - x0)) / (y2 - y0);

  float x_start = x0;
  float x_end = x0;
  for (int yi = y0; yi <= y2; yi++)
  {
    draw_line(x_start, yi, x_end, yi, color);
    x_start += inv_slope1;
    x_end += inv_slope2;
  }
}

//------------------------------------------------------//
// Draw a filled triangle with a flat top               //
//------------------------------------------------------//
//  (x0,y0)------(x1,y1)                                //
//     \            /                                   //
//      \          /                                    //
//       \        /                                     //
//        \      /                                      //
//         \    /                                       //
//          \  /                                        //
//           \/                                         //
//        (x2,y2)                                       //
//------------------------------------------------------//
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color)
{
  float inv_slope1 = ((float)(x2 - x0)) / (y2 - y0); // Inverse slope (run over rise), because in this case y is the independent value
  float inv_slope2 = ((float)(x2 - x1)) / (y2 - y1);

  float x_start = x2;
  float x_end = x2;
  for (int yi = y2; yi >= y0; yi--)
  {
    draw_line(x_start, yi, x_end, yi, color);
    x_start -= inv_slope1;
    x_end -= inv_slope2;
  }
}

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color)
{
  int vertices[3][2] = {
      {x0, y0},
      {x1, y1},
      {x2, y2}};
  // Sort the vertices
  sort_three_vertices_by_y(vertices);
  x0 = vertices[0][0];
  y0 = vertices[0][1];
  x1 = vertices[1][0];
  y1 = vertices[1][1];
  x2 = vertices[2][0];
  y2 = vertices[2][1];

  // If one of the y values are equal, then that means that the triangle does not need to be divided into two!
  if (y0 == y1)
  {
    fill_flat_top_triangle(x0, y0, x1, y1, x2, y2, color);
  }
  else if (y1 == y2)
  {
    fill_flat_bottom_triangle(x0, y0, x1, y1, x2, y2, color);
  }
  else
  {
    int mx = (((x2 - x0) * (y1 - y0)) / (y2 - y0)) + x0; // Calculate Mx by triangle similarity
    int my = y1;

    fill_flat_bottom_triangle(x0, y0, x1, y1, mx, my, color);
    fill_flat_top_triangle(x1, y1, mx, my, x2, y2, color);
  }
  return;
}

void draw_texel(int xi, int yi,
                vec4_t point_a, vec4_t point_b, vec4_t point_c,
                tex2_t uv_a, tex2_t uv_b, tex2_t uv_c, color_t *texture)
{
  vec2_t p = {.x = xi, .y = yi};
  vec3_t weights = barycentric_weights(vec4_xy(point_a), vec4_xy(point_b), vec4_xy(point_c), p);

  float alpha = weights.x;
  float beta = weights.y;
  float gamma = weights.z;

  // We cannot linearly interpolate w, so we interpolate the reciprocal of w instead which is linear.
  // Get the reciprocal all of the ws of the points
  // w is initially the z component
  // We can extract this part outside of this function to reduce the amount of divisions performed
  float inv_w_a = 1 / point_a.w;
  float inv_w_b = 1 / point_b.w;
  float inv_w_c = 1 / point_c.w;

  // Interpolate using barycentric coordinates, multiplying by 1 / w of the point for correct perspective texture mapping (instead of affine texture mapping)
  float u = uv_a.u * inv_w_a * alpha + uv_b.u * inv_w_b * beta + uv_c.u * inv_w_c * gamma;
  float v = uv_a.v * inv_w_a * alpha + uv_b.v * inv_w_b * beta + uv_c.v * inv_w_c * gamma;
  float inverse_w = inv_w_a * alpha + inv_w_b * beta + inv_w_c * gamma;

  // Divide by the interpolated inverse of w to remove the initial division by w.
  u /= inverse_w;
  v /= inverse_w;

  // Map the UV coordinate to actual texture dimensions
  int texture_x = clamp(0, texture_width, abs((int)(texture_width * u)));
  int texture_y = clamp(0, texture_height, abs((int)(texture_height * v)));

  draw_pixel(xi, yi, texture[texture_width * texture_y + texture_x]);
}

void sort_three_vertices_uv_by_y(triangle_t *triangle) // Insertion Sort, sorts in place
{
  for (int slot = 1; slot < 3; slot++)
  {
    int i = slot - 1;
    vec4_t vertex = triangle->points[i + 1];
    tex2_t uv = triangle->texcoords[i + 1];

    while (i >= 0 && triangle->points[i].y > vertex.y) // Sort by y-value
    {
      triangle->points[i + 1] = triangle->points[i];
      triangle->texcoords[i + 1] = triangle->texcoords[i];
      i--;
    }
    triangle->points[i + 1] = vertex;
    triangle->texcoords[i + 1] = uv;
  }
}

void draw_textured_triangle(
    triangle_t triangle,
    color_t *texture)
{
  // Sort the vertices
  sort_three_vertices_uv_by_y(&triangle);

  // OBJ file UV coordinates start from the top left and v grows downwards,
  // so we need to change the v component to be make it start from the bottom left instead.
  for (int i = 0; i < 3; i++)
  {
    triangle.texcoords[i].v = 1.0 - triangle.texcoords[i].v;
  }

  // Unpack values
  vec4_t point_a = triangle.points[0];
  tex2_t uv_a = triangle.texcoords[0];
  vec4_t point_b = triangle.points[1];
  tex2_t uv_b = triangle.texcoords[1];
  vec4_t point_c = triangle.points[2];
  tex2_t uv_c = triangle.texcoords[2];

  int x0 = point_a.x;
  int y0 = point_a.y;

  int x1 = point_b.x;
  int y1 = point_b.y;

  int x2 = point_c.x;
  int y2 = point_c.y;

  // Draw flat top
  float inv_slope1 = y2 != y1 ? ((float)(x2 - x1)) / abs(y2 - y1) : 0; // Inverse slope (run over rise), because in this case y is the independent value
  float inv_slope2 = y2 != y0 ? ((float)(x2 - x0)) / abs(y2 - y0) : 0;

  if (y2 != y1)
  {
    for (int yi = y1; yi <= y2; yi++)
    {
      int x_start = x1 + (yi - y1) * inv_slope1;
      int x_end = x0 + (yi - y0) * inv_slope2;
      if (x_start > x_end)
      {
        float temp = x_start;
        x_start = x_end;
        x_end = temp;
      }
      for (int xi = x_start; xi < x_end; xi++)
      {
        // We cannot use draw_line here as we need to sample the color of the texture
        draw_texel(xi, yi, point_a, point_b, point_c, uv_a, uv_b, uv_c, texture);
      }
      x_start -= inv_slope1;
      x_end -= inv_slope2;
    }
  }

  // Draw flat bottom
  inv_slope1 = y1 != y0 ? ((float)(x1 - x0)) / abs((int)y1 - (int)y0) : 0; // Inverse slope (run over rise), because in this case y is the independent value
  inv_slope2 = y2 != y0 ? ((float)(x2 - x0)) / abs((int)y2 - (int)y0) : 0;

  if (y0 != y1)
  {
    for (int yi = y0; yi <= y1; yi++)
    {
      int x_start = x1 + (yi - y1) * inv_slope1;
      int x_end = x0 + (yi - y0) * inv_slope2;
      if (x_start > x_end)
      {
        float temp = x_start;
        x_start = x_end;
        x_end = temp;
      }
      for (int xi = x_start; xi < x_end; xi++)
      {
        // We cannot use draw_line here as we need to sample the color of the texture
        draw_texel(xi, yi, point_a, point_b, point_c, uv_a, uv_b, uv_c, texture);
      }
    }
  }

  return;
}

// Barycentric Coordinates
vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
  vec2_t ac = vec2_sub(c, a);
  vec2_t ab = vec2_sub(b, a);
  vec2_t ap = vec2_sub(p, a);
  vec2_t pc = vec2_sub(c, p);
  vec2_t pb = vec2_sub(b, p);

  // Area of the parallelogram formed by the entire triangle
  // What we are actually computing here is the z component for what is supposed to be the cross product
  float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // || AC x AB ||

  float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;
  float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;
  float gamma = 1 - alpha - beta;

  vec3_t barycentric_coordinates = {
      alpha, beta, gamma};
  return barycentric_coordinates;
}

// Triangle Sorting

void mergesort_triangle_merge(triangle_t *a, triangle_t *b, size_t n, size_t start, size_t mid, size_t end)
{
  size_t i = start;
  size_t j = mid;

  for (size_t k = start; k < end; k++)
  {
    if (i < mid && (j >= end || a[i].depth <= b[j].depth))
    {
      b[k] = a[i];
      i++;
    }
    else
    {
      b[k] = a[j];
      j++;
    }
  }
}

void mergesort_triangle_partition(triangle_t *a, triangle_t *b, size_t n, size_t start, size_t end)
{
  if (end - start <= 1) // end - start <= 1, meaning that the partition size is 1
  {
    return;
  }

  size_t mid = (start + end) / 2;

  mergesort_triangle_partition(b, a, n, start, mid);
  mergesort_triangle_partition(b, a, n, mid, end);

  mergesort_triangle_merge(b, a, n, start, mid, end);
}

void mergesort_triangle_by_depth(triangle_t *triangles)
{
  size_t n = array_length(triangles);
  triangle_t *b = (triangle_t *)malloc(sizeof(triangle_t) * n);
  memcpy(b, triangles, sizeof(triangle_t) * n);

  mergesort_triangle_partition(triangles, b, n, 0, n);

  free(b);

  assert(n == array_length(triangles));
}

void insertion_sort_triangle_by_depth(triangle_t *triangles)
{
  size_t n = array_length(triangles);
  for (int slot = 1; slot < n; slot++)
  {
    int i = slot - 1;
    triangle_t x = triangles[i + 1];
    while (i > 0 && x.depth < triangles[i].depth)
    {
      triangles[i + 1] = triangles[i];
      i--;
    }
    triangles[i + 1] = x;
  }
}