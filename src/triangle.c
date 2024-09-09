#include "triangle.h"
#include "display.h"

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

void sort_three_vertices_uv_by_y(int vertices[3][2], float uv[3][2]) // Insertion Sort
{
  for (int slot = 1; slot < 3; slot++)
  {
    int i = slot - 1;
    int x = vertices[i + 1][0];
    int y = vertices[i + 1][1];
    int u = uv[i + 1][0];
    int v = uv[i + 1][1];
    while (i >= 0 && vertices[i][1] > y) // Sort by y-value
    {
      vertices[i + 1][0] = vertices[i][0];
      vertices[i + 1][1] = vertices[i][1];
      uv[i + 1][0] = uv[i][0];
      uv[i + 1][1] = uv[i][1];
      i--;
    }
    vertices[i + 1][0] = x;
    vertices[i + 1][1] = y;
    uv[i + 1][0] = uv[i][0];
    uv[i + 1][1] = uv[i][1];
  }
}

//--------------------------------------------------------//
// Draw a textured triangle with a flat top               //
//--------------------------------------------------------//
//  (x0,y0)------(x1,y1)                                  //
//     \            /                                     //
//      \          /                                      //
//       \        /                                       //
//        \      /                                        //
//         \    /                                         //
//          \  /                                          //
//           \/                                           //
//        (x2,y2)                                         //
//--------------------------------------------------------//
void fill_textured_flat_top_triangle(int x0, int y0, int u0, int v0, int x1, int y1, int u1, int v1, int x2, int y2, int u2, int v2, color_t *texture)
{
  float inv_slope1 = y2 != y1 ? ((float)(x2 - x1)) / (y2 - y1) : 0; // Inverse slope (run over rise), because in this case y is the independent value
  float inv_slope2 = y2 != y0 ? ((float)(x2 - x0)) / (y2 - y0) : 0;

  if (y2 == y1)
  {
    return;
  }

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
    // We cannot use draw_line here as we need to sample the color of the texture
    // draw_line(x_start, yi, x_end, yi, color);
    for (int xi = x_start; xi < x_end; xi++)
    {
      draw_pixel(xi, yi, 0xFFFF00FF);
    }
    x_start -= inv_slope1;
    x_end -= inv_slope2;
  }
}

// Draw a textured triangle with a flat bottom
void fill_textured_flat_bottom_triangle(int x0, int y0, int u0, int v0, int x1, int y1, int u1, int v1, int x2, int y2, int u2, int v2, color_t *texture)
{
  float inv_slope1 = y1 != y0 ? ((float)(x1 - x0)) / (y1 - y0) : 0; // Inverse slope (run over rise), because in this case y is the independent value
  float inv_slope2 = y2 != y0 ? ((float)(x2 - x0)) / (y2 - y0) : 0;

  if (y0 == y1)
  {
    return;
  }

  for (int yi = y0; yi <= y1; yi++)
  {
    int x_start = x1 + (yi - y1) * inv_slope1;
    int x_end = x0 + (yi - y0) * inv_slope2;
    // We cannot use draw_line here as we need to sample the color of the texture
    // draw_line(x_start, yi, x_end, yi, color);
    if (x_start > x_end)
    {
      float temp = x_start;
      x_start = x_end;
      x_end = temp;
    }
    for (int xi = x_start; xi < x_end; xi++)
    {
      draw_pixel(xi, yi, 0xFFFF00FF);
    }
  }
}

void draw_textured_triangle(
    int x0, int y0, float u0, float v0,
    int x1, int y1, float u1, float v1,
    int x2, int y2, float u2, float v2,
    color_t *texture)
{
  int vertices[3][2] = {
      {x0, y0},
      {x1, y1},
      {x2, y2}};
  float uvs[3][2] = {
      {u0, v0},
      {u1, v1},
      {u2, v2},
  };
  // Sort the vertices
  sort_three_vertices_uv_by_y(vertices, uvs);
  x0 = vertices[0][0];
  y0 = vertices[0][1];
  u0 = uvs[0][0];
  v0 = uvs[0][1];
  x1 = vertices[1][0];
  y1 = vertices[1][1];
  u1 = uvs[1][0];
  v1 = uvs[1][1];
  x2 = vertices[2][0];
  y2 = vertices[2][1];
  u2 = uvs[2][0];
  v2 = uvs[2][1];

  fill_textured_flat_top_triangle(x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, texture);
  fill_textured_flat_bottom_triangle(x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, texture);

  return;
}