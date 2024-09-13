#ifndef TEXTURE_RENENGINE_SFW
#define TEXTURE_RENENGINE_SFW

#include <stdint.h>
#include "../upng/upng.h"

typedef struct tex2
{
  float u;
  float v;
} tex2_t;

extern int texture_width;
extern int texture_height;

extern upng_t *png_texture;
extern uint32_t *mesh_texture;

void load_png_texture_data(char *file_path);
// void sample(tex2_t uv);
tex2_t tex2_clone(tex2_t *t);

#endif