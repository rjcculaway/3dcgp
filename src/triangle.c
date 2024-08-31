#include "triangle.h"
#include "display.h"

// TODO: Functions for triangles

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
void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
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
void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
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

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
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

  // for (int i = 0; i < 3; i++)
  // {
  //   printf("(%d %d %d)\n", vertices[i][0], vertices[i][1]);
  // }

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