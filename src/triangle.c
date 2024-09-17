#include "triangle.h"
#include "display.h"
#include "texture.h"
#include "light.h"

float edge_cross(vec2_t a, vec2_t b, vec2_t p)
{
  vec2_t ab = vec2_sub(b, a);
  vec2_t ap = vec2_sub(p, a);

  return vec2_cross(ab, ap);
}

// Draw a filled triangle with a flat bottom
void fill_flat_bottom_triangle_scanline(triangle_t triangle, color_t color)
{
  int x0 = triangle.points[0].x;
  int y0 = triangle.points[0].y;
  int x1 = triangle.points[1].x;
  int y1 = triangle.points[1].y;
  int x2 = triangle.points[2].x;
  int y2 = triangle.points[2].y;

  vec2_t v0_xy = vec4_xy(triangle.points[0]);
  vec2_t v1_xy = vec4_xy(triangle.points[1]);
  vec2_t v2_xy = vec4_xy(triangle.points[2]);

  float inv_slope1 = y1 != y0 ? ((float)(x1 - x0)) / abs((int)y1 - (int)y0) : 0; // Inverse slope (run over rise), because in this case y is the independent value
  float inv_slope2 = y2 != y0 ? ((float)(x2 - x0)) / abs((int)y2 - (int)y0) : 0;

  // Area of the parallelogram to compute factors for barycentric coordinates
  float area_parallelogram = edge_cross(v0_xy, v1_xy, v2_xy);

  // We cannot linearly interpolate w, so we interpolate the reciprocal of w instead which is linear.
  // Get the reciprocal all of the ws of the points
  // w is initially the z component
  float inv_w_a = 1 / triangle.points[0].w;
  float inv_w_b = 1 / triangle.points[1].w;
  float inv_w_c = 1 / triangle.points[2].w;

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
        vec2_t p = vec2_create(xi, yi);
        vec3_t barycentric_unnormalized = compute_barycentric_unnormalized(v0_xy, v1_xy, v2_xy, p);
        float w0 = barycentric_unnormalized.x;
        float w1 = barycentric_unnormalized.y;

        // Barycentric interpolation weights
        float alpha = w0 / area_parallelogram;
        float beta = w1 / area_parallelogram;
        float gamma = 1.0 - alpha - beta;

        // We cannot use draw_line here as we need to sample the color of the texture
        draw_triangle_pixel(xi, yi, alpha, beta, gamma, inv_w_a, inv_w_b, inv_w_c, color);
      }
    }
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
void fill_flat_top_triangle_scanline(triangle_t triangle, color_t color)
{
  int x0 = triangle.points[0].x;
  int y0 = triangle.points[0].y;
  int x1 = triangle.points[1].x;
  int y1 = triangle.points[1].y;
  int x2 = triangle.points[2].x;
  int y2 = triangle.points[2].y;

  float inv_slope1 = y2 != y1 ? ((float)(x2 - x1)) / abs(y2 - y1) : 0; // Inverse slope (run over rise), because in this case y is the independent value
  float inv_slope2 = y2 != y0 ? ((float)(x2 - x0)) / abs(y2 - y0) : 0;

  vec2_t v0_xy = vec4_xy(triangle.points[0]);
  vec2_t v1_xy = vec4_xy(triangle.points[1]);
  vec2_t v2_xy = vec4_xy(triangle.points[2]);

  // We cannot linearly interpolate w, so we interpolate the reciprocal of w instead which is linear.
  // Get the reciprocal all of the ws of the points
  // w is initially the z component
  float inv_w_a = 1 / triangle.points[0].w;
  float inv_w_b = 1 / triangle.points[1].w;
  float inv_w_c = 1 / triangle.points[2].w;

  // Area of the parallelogram to compute factors for barycentric coordinates
  float area_parallelogram = edge_cross(v0_xy, v1_xy, v2_xy);

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
      vec2_t p = vec2_create(xi, yi);
      vec3_t barycentric_unnormalized = compute_barycentric_unnormalized(v0_xy, v1_xy, v2_xy, p);
      float w0 = barycentric_unnormalized.x;
      float w1 = barycentric_unnormalized.y;

      // Barycentric interpolation weights
      float alpha = w0 / area_parallelogram;
      float beta = w1 / area_parallelogram;
      float gamma = 1.0 - alpha - beta;

      // We cannot use draw_line here as we need to sample the color of the texture
      draw_triangle_pixel(xi, yi, alpha, beta, gamma, inv_w_a, inv_w_b, inv_w_c, color);
    }
    x_start -= inv_slope1;
    x_end -= inv_slope2;
  }
}

// Highly parallelizable triangle rasterizations, algorithm from Juan Pineda's 1988 paper
// https://www.cs.drexel.edu/~deb39/Classes/Papers/comp175-06-pineda.pdf
void draw_filled_triangle(triangle_t triangle, color_t color)
{
  // Vertices
  vec4_t v0 = triangle.points[0];
  vec4_t v1 = triangle.points[1];
  vec4_t v2 = triangle.points[2];

  vec2_t v0_xy = vec4_xy(v0);
  vec2_t v1_xy = vec4_xy(v1);
  vec2_t v2_xy = vec4_xy(v2);

  // Used to compute barycentric coordinates
  float area_parallelogram = edge_cross(
      v0_xy,
      v1_xy,
      v2_xy);

  // Used for perspective-correct barycentric interpolation
  float inv_w_v0 = 1.0 / v0.w;
  float inv_w_v1 = 1.0 / v1.w;
  float inv_w_v2 = 1.0 / v2.w;

  // Get the bounding box
  int x_min = fmin(v0.x, fmin(v1.x, v2.x));
  int x_max = fmax(v0.x, fmax(v1.x, v2.x));
  int y_min = fmin(v0.y, fmin(v1.y, v2.y));
  int y_max = fmax(v0.y, fmax(v1.y, v2.y));

  // Compute the delta for the w's per column and per row, since these deltas are constant.
  // This is because the barycentric coordinate is parallel across the edges.
  // To derive these formulas, use FOIL expansion on the cross product formula
  float delta_w0_col = v1.y - v2.y;
  float delta_w1_col = v2.y - v0.y;
  float delta_w2_col = v0.y - v1.y;

  float delta_w0_row = v2.x - v1.x;
  float delta_w1_row = v0.x - v2.x;
  float delta_w2_row = v1.x - v0.x;

  // Bias factors for precedence in rasterization
  int bias0 = is_top_left(&v1_xy, &v2_xy) ? 0 : -1;
  int bias1 = is_top_left(&v2_xy, &v0_xy) ? 0 : -1;
  int bias2 = is_top_left(&v0_xy, &v1_xy) ? 0 : -1;

  // Compute the initial values for the edge functions
  vec2_t p0 = {x_min, y_min};
  vec3_t barycentric_unnormalized_row = compute_barycentric_unnormalized(v0_xy, v1_xy, v2_xy, p0);
  // Add the bias
  barycentric_unnormalized_row = vec3_add(barycentric_unnormalized_row, vec3_create(bias0, bias1, bias2));
  float w0_row = barycentric_unnormalized_row.x;
  float w1_row = barycentric_unnormalized_row.y;
  float w2_row = barycentric_unnormalized_row.z;

  for (int yi = y_min; yi <= y_max; yi++)
  {
    float w0 = w0_row;
    float w1 = w1_row;
    float w2 = w2_row;
    for (int xi = x_min; xi <= x_max; xi++)
    {

      /**
       *  The point p is inside the triangle if it is to the right of all three edges of the triangle.
       *  Hello, cross product! We can use the cross product's sign to determine if an edge is in correct direction.
       *  We can't use the cross product in 2D, but we "cheat" by using the z-axis as the result.
       *
       *  "Cross products", or 'edge functions' according to Juan Pineda.
       *  These are actually the barycentric coordinates of the triangle, but not divided by the area of the parallelogram formed by the triangle.
       *  Adding a bias essentially "shrinks" the non-top-left edges of the triangle.
       */

      // We actually DON'T have to compute the w's every iteration, because the delta of the cross
      // products is CONSTANT per iteration. So we can just compute an initial value for the w's,
      // then increment by a computed delta.
      /*
      vec2_t p = {xi, yi};

      vec3_t barycentric_unnormalized = compute_barycentric_unnormalized(v0_xy, v1_xy, v2_xy, p);

      float w0 = barycentric_unnormalized.x;
      float w1 = barycentric_unnormalized.y;
      float w2 = barycentric_unnormalized.z;
      */

      float alpha = w0 / area_parallelogram;
      float beta = w1 / area_parallelogram;
      float gamma = 1.0 - alpha - beta;

      if (is_point_inside_triangle(w0, w1, w2, bias0, bias1, bias2))
      {
        draw_triangle_pixel(xi, yi,
                            alpha, beta, gamma,
                            inv_w_v0, inv_w_v1, inv_w_v2,
                            color);
      }
      w0 += delta_w0_col;
      w1 += delta_w1_col;
      w2 += delta_w2_col;
    }
    w0_row += delta_w0_row;
    w1_row += delta_w1_row;
    w2_row += delta_w2_row;
  }
  return;
}

void draw_textured_triangle(triangle_t triangle, upng_t *texture)
{
  // Vertices
  vec4_t v0 = triangle.points[0];
  vec4_t v1 = triangle.points[1];
  vec4_t v2 = triangle.points[2];

  vec2_t v0_xy = vec4_xy(v0);
  vec2_t v1_xy = vec4_xy(v1);
  vec2_t v2_xy = vec4_xy(v2);

  // Used to compute barycentric coordinates
  float area_parallelogram = edge_cross(
      v0_xy,
      v1_xy,
      v2_xy);

  // Used for perspective-correct barycentric interpolation
  float inv_w_v0 = 1.0 / v0.w;
  float inv_w_v1 = 1.0 / v1.w;
  float inv_w_v2 = 1.0 / v2.w;

  // Get the bounding box
  int x_min = fmin(v0.x, fmin(v1.x, v2.x));
  int x_max = fmax(v0.x, fmax(v1.x, v2.x));
  int y_min = fmin(v0.y, fmin(v1.y, v2.y));
  int y_max = fmax(v0.y, fmax(v1.y, v2.y));

  // Compute the delta for the w's per column and per row, since these deltas are constant.
  // This is because the barycentric coordinate is parallel across the edges.
  // To derive these formulas, use FOIL expansion on the cross product formula
  float delta_w0_col = v1.y - v2.y;
  float delta_w1_col = v2.y - v0.y;
  float delta_w2_col = v0.y - v1.y;

  float delta_w0_row = v2.x - v1.x;
  float delta_w1_row = v0.x - v2.x;
  float delta_w2_row = v1.x - v0.x;

  // Bias factors for precedence in rasterization
  int bias0 = is_top_left(&v1_xy, &v2_xy) ? 0 : -1;
  int bias1 = is_top_left(&v2_xy, &v0_xy) ? 0 : -1;
  int bias2 = is_top_left(&v0_xy, &v1_xy) ? 0 : -1;

  // Compute the initial values for the edge functions
  vec2_t p0 = {x_min, y_min};
  vec3_t barycentric_unnormalized_row = compute_barycentric_unnormalized(v0_xy, v1_xy, v2_xy, p0);
  // Add the bias
  barycentric_unnormalized_row = vec3_add(barycentric_unnormalized_row, vec3_create(bias0, bias1, bias2));
  float w0_row = barycentric_unnormalized_row.x;
  float w1_row = barycentric_unnormalized_row.y;
  float w2_row = barycentric_unnormalized_row.z;

  for (int yi = y_min; yi <= y_max; yi++)
  {
    float w0 = w0_row;
    float w1 = w1_row;
    float w2 = w2_row;
    for (int xi = x_min; xi <= x_max; xi++)
    {

      /**
       *  The point p is inside the triangle if it is to the right of all three edges of the triangle.
       *  Hello, cross product! We can use the cross product's sign to determine if an edge is in correct direction.
       *  We can't use the cross product in 2D, but we "cheat" by using the z-axis as the result.
       *
       *  "Cross products", or 'edge functions' according to Juan Pineda.
       *  These are actually the barycentric coordinates of the triangle, but not divided by the area of the parallelogram formed by the triangle.
       *  Adding a bias essentially "shrinks" the non-top-left edges of the triangle.
       */

      // We actually DON'T have to compute the w's every iteration, because the delta of the cross
      // products is CONSTANT per iteration. So we can just compute an initial value for the w's,
      // then increment by a computed delta.
      /*
      vec2_t p = {xi, yi};

      vec3_t barycentric_unnormalized = compute_barycentric_unnormalized(v0_xy, v1_xy, v2_xy, p);

      float w0 = barycentric_unnormalized.x;
      float w1 = barycentric_unnormalized.y;
      float w2 = barycentric_unnormalized.z;
      */

      float alpha = w0 / area_parallelogram;
      float beta = w1 / area_parallelogram;
      float gamma = 1.0 - alpha - beta;

      if (is_point_inside_triangle(w0, w1, w2, bias0, bias1, bias2))
      {
        draw_texel(xi, yi,
                   alpha, beta, gamma,
                   inv_w_v0, inv_w_v1, inv_w_v2,
                   triangle.texcoords[0], triangle.texcoords[1], triangle.texcoords[2],
                   texture);
      }
      w0 += delta_w0_col;
      w1 += delta_w1_col;
      w2 += delta_w2_col;
    }
    w0_row += delta_w0_row;
    w1_row += delta_w1_row;
    w2_row += delta_w2_row;
  }
  return;
}

// Top-left rule for rasterization which determines the precedence of edges and prevents overdraw
bool is_top_left(vec2_t *start, vec2_t *end)
{
  vec2_t edge = vec2_sub(*end, *start);

  bool is_top_edge = edge.y == 0 && edge.x > 0;
  bool is_left_edge = edge.y < 0; // Since we are using a clockwise orientation, the left edge points up and thus is negative.

  return is_top_edge || is_left_edge;
}

bool is_point_inside_triangle(int w0, int w1, int w2, int bias0, int bias1, int bias2)
{
  if (w0 + bias0 < 0 || w1 + bias1 < 0 || w2 + bias2 < 0)
  {
    return false;
  }

  return true;
}

// Draw a filled triangle using scanlines; poorly parallelizable
void draw_filled_triangle_scanline(triangle_t triangle, color_t color)
{
  sort_three_vertices_uv_by_y(&triangle);
  int y0 = triangle.points[0].y;
  int y1 = triangle.points[1].y;
  int y2 = triangle.points[2].y;

  // If one of the y values are equal, then that means that the triangle does not need to be divided into two!
  if (y0 != y1)
  {
    fill_flat_bottom_triangle_scanline(triangle, color);
  }
  if (y1 != y2)
  {
    fill_flat_top_triangle_scanline(triangle, color);
  }
  return;
}

void draw_triangle_pixel(int xi, int yi,
                         float alpha, float beta, float gamma,
                         float inv_w_a, float inv_w_b, float inv_w_c,
                         color_t color)
{
  // Interpolate using barycentric coordinates, multiplying by 1 / w of the point for correct perspective texture mapping (instead of affine texture mapping)
  float inverse_w = inv_w_a * alpha + inv_w_b * beta + inv_w_c * gamma;

  // Using the inverse w as is is incorrect, as nearer objects get lesser inverse w values than farther ones.
  // As such, we need to subtract the inverse w from 1.0.
  float transformed_inverse_w = 1.0 - inverse_w;
  if (transformed_inverse_w < get_z_buffer_at(xi, yi)) // Draw only if the current depth is less than what is in the z-buffer
  {
    draw_pixel(xi, yi, color);
    update_z_buffer_at(xi, yi, transformed_inverse_w);
  }
}

void draw_texel(int xi, int yi,
                float alpha, float beta, float gamma,
                float inv_w_a, float inv_w_b, float inv_w_c,
                tex2_t uv_a, tex2_t uv_b, tex2_t uv_c, upng_t *texture)
{
  // Query texture information
  int texture_width = upng_get_width(texture);
  int texture_height = upng_get_height(texture);
  color_t *texture_buffer = (color_t *)upng_get_buffer(texture);

  // Interpolate using barycentric coordinates, multiplying by 1 / w of the point for correct perspective texture mapping (instead of affine texture mapping)
  float u = uv_a.u * inv_w_a * alpha + uv_b.u * inv_w_b * beta + uv_c.u * inv_w_c * gamma;
  float v = uv_a.v * inv_w_a * alpha + uv_b.v * inv_w_b * beta + uv_c.v * inv_w_c * gamma;
  float inverse_w = inv_w_a * alpha + inv_w_b * beta + inv_w_c * gamma;

  // Divide by the interpolated inverse of w to remove the initial division by w.
  u /= inverse_w;
  v /= inverse_w;

  // Map the UV coordinate to actual texture dimensions, wrapping and clamping to avoid overflow
  int texture_x = clamp(0, texture_width, abs((int)(texture_width * u)) % texture_width);
  int texture_y = clamp(0, texture_height, abs((int)(texture_height * v)) % texture_height);

  // Using the inverse w as is is incorrect, as nearer objects get lesser inverse w values than farther ones.
  // As such, we need to subtract the inverse w from 1.0.
  float transformed_inverse_w = 1.0 - inverse_w;
  if (transformed_inverse_w < get_z_buffer_at(xi, yi)) // Draw only if the current depth is less than what is in the z-buffer
  {
    draw_pixel(xi, yi, texture_buffer[texture_width * texture_y + texture_x]);
    update_z_buffer_at(xi, yi, transformed_inverse_w);
  }
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

void draw_textured_triangle_scanline(
    triangle_t triangle,
    upng_t *texture)
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
  vec2_t v0_xy = vec4_xy(triangle.points[0]);
  tex2_t uv_a = triangle.texcoords[0];
  vec4_t point_b = triangle.points[1];
  vec2_t v1_xy = vec4_xy(triangle.points[1]);
  tex2_t uv_b = triangle.texcoords[1];
  vec4_t point_c = triangle.points[2];
  vec2_t v2_xy = vec4_xy(triangle.points[2]);
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

  // We cannot linearly interpolate w, so we interpolate the reciprocal of w instead which is linear.
  // Get the reciprocal all of the ws of the points
  // w is initially the z component
  float inv_w_a = 1 / point_a.w;
  float inv_w_b = 1 / point_b.w;
  float inv_w_c = 1 / point_c.w;

  float area_parallelogram = edge_cross(v0_xy, v1_xy, v2_xy);

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
        vec2_t p = vec2_create(xi, yi);
        vec3_t barycentric_unnormalized = compute_barycentric_unnormalized(v0_xy, v1_xy, v2_xy, p);

        float w0 = barycentric_unnormalized.x;
        float w1 = barycentric_unnormalized.y;

        float alpha = w0 / area_parallelogram;
        float beta = w1 / area_parallelogram;
        float gamma = 1.0 - alpha - beta;

        // We cannot use draw_line here as we need to sample the color of the texture
        draw_texel(xi, yi, alpha, beta, gamma,
                   inv_w_a, inv_w_b, inv_w_c,
                   uv_a, uv_b, uv_c, texture);
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
        vec2_t p = vec2_create(xi, yi);
        float w0 = edge_cross(v1_xy, v2_xy, p);
        float w1 = edge_cross(v2_xy, v0_xy, p);

        float alpha = w0 / area_parallelogram;
        float beta = w1 / area_parallelogram;
        float gamma = 1.0 - alpha - beta;

        // We cannot use draw_line here as we need to sample the color of the texture
        draw_texel(xi, yi, alpha, beta, gamma,
                   inv_w_a, inv_w_b, inv_w_c,
                   uv_a, uv_b, uv_c, texture);
      }
    }
  }

  return;
}

vec3_t compute_triangle_normal(vec4_t points[3])
{
  // Backface Culling
  /*   A   */
  /*  / \  */
  /* B---C */
  vec3_t vec_a = vec3_from_vec4(points[0]);
  vec3_t vec_b = vec3_from_vec4(points[1]);
  vec3_t vec_c = vec3_from_vec4(points[2]);
  vec3_t vec_ab = vec3_normalize(vec3_sub(vec_b, vec_a)); // B - A
  vec3_t vec_ac = vec3_normalize(vec3_sub(vec_c, vec_a)); // C - A

  // Left-handed coordinate system (+z is away), so the order of the cross product
  // must be b-a x a-b
  vec3_t normal = vec3_normalize(vec3_cross(
      vec_ab,
      vec_ac));
  return normal;
}

vec3_t compute_barycentric_unnormalized(vec2_t v0, vec2_t v1, vec2_t v2, vec2_t p)
{
  return vec3_create(
      edge_cross(v1, v2, p),
      edge_cross(v2, v0, p),
      edge_cross(v0, v1, p));
}