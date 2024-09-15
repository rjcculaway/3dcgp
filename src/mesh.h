#ifndef MESH_RENENGINE_SFW
#define MESH_RENENGINE_SFW

#define MAX_NUM_MESHES 32

#include "vector.h"
#include "triangle.h"
#include "../upng/upng.h"

typedef struct mesh
{
  vec3_t *vertices; // Dynamic
  face_t *faces;    // Dynamic
  upng_t *texture;  // PNG for the texture
  tex2_t *texcoords;
  vec3_t scale;
  vec3_t rotation;
  vec3_t translation;

} mesh_t;

mesh_t *get_mesh(size_t idx);
mesh_t *get_meshes(void);
size_t get_mesh_count(void);
void load_mesh(char *file_name, char *png_texture_file_name, vec3_t scale, vec3_t rotation, vec3_t translation);
mesh_t load_obj_from_file(char *file_name);
upng_t *load_mesh_png(char *file_path);
void free_mesh(mesh_t mesh);
void free_meshes();

#endif