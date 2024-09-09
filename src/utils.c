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
