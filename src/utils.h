#ifndef UTILS
#define UTILS

#include "array.h"
#include "math.h"
#include <assert.h>

#define MILLISECONDS_PER_SECOND 1000

float deg_to_rad(float deg);
float rad_to_deg(float rad);

double fclamp(double minimum, double maximum, double value);
int clamp(int minimum, int maximum, int value);

#endif