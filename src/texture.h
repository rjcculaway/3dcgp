#ifndef TEXTURE_3DCGP
#define TEXTURE_3DCGP

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

#endif