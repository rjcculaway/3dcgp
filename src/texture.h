#ifndef TEXTURE_RENENGINE_SFW
#define TEXTURE_RENENGINE_SFW

#include <stdint.h>

typedef struct tex2
{
  float u;
  float v;
} tex2_t;

tex2_t tex2_clone(tex2_t *t);

#endif