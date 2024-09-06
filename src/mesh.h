#ifndef MESH_3DCGP
#define MESH_3DCGP

#include "vector.h"
#include "triangle.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 faces, 2 triangles per face

extern vec3_t cube_vertices[N_CUBE_VERTICES];
extern face_t cube_faces[N_CUBE_FACES];

typedef struct mesh
{
  vec3_t *vertices; // Dynamic
  face_t *faces;    // Dynamic
  vec3_t rotation;
  vec3_t scale;
  vec3_t translation;
} mesh_t;

extern mesh_t mesh;

void load_mesh_from_file(char *file_name);
void load_cube_mesh_data(void);

#endif