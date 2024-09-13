#include "utils.h"

float inline deg_to_rad(float deg)
{
  return deg / 180 * M_PI;
}

float inline rad_to_deg(float rad)
{
  return rad / M_PI * 180;
}

double fclamp(double minimum, double maximum, double value)
{
  return fmin(fmax(minimum, value), maximum);
}

int clamp(int minimum, int maximum, int value)
{
  int min = value > maximum ? maximum : value;
  return minimum > min ? minimum : min;
}

double fovx_from_fovy(double fovy, double aspect_x)
{
  return atan(tan(fovy / 2) * aspect_x) * 2;
}

double fovy_from_fovx(double fovx, double aspect_y)
{
  return atan(tan(fovx / 2) * aspect_y) * 2;
}

double flerp(double first, double final, double t)
{
  return first + t * (final - first);
}
