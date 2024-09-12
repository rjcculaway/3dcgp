#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mesh.h"
#include "array.h"

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .texcoords = NULL,
    .rotation = {0, 0, 0},
    .scale = {1.0, 1.0, 1.0},
    .translation = {0.0, 0.0, 0.0},
};

void load_mesh_from_file(char *file_name)
{
  FILE *file_handle = fopen(file_name, "r");

  if (file_handle == NULL)
  {
    return;
  }

  char line[1024] = {0};
  while (fgets(line, 1024, file_handle) != NULL)
  {
    if (strncmp("v ", line, 2) == 0) // vertices
    {
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &(vertex.x), &(vertex.y), &(vertex.z));
      array_push(mesh.vertices, vertex);
    }
    if (strncmp("vt ", line, 3) == 0) // texture coordinates
    {
      tex2_t texcoord;
      sscanf(line, "vt %f %f", &(texcoord.u), &(texcoord.v));
      array_push(mesh.texcoords, texcoord);
    }
    if (strncmp("f ", line, 2) == 0) // faces, ignore other values (%*d) for now
    {
      face_t face;
      sscanf(line, "f %d/%d/%*d %d/%d/%*d %d/%d/%*d", &(face.a), &(face.a_uv), &(face.b), &(face.b_uv), &(face.c), &(face.c_uv));
      // Indices start at 1 so we subtract 1 for zero-indexing.
      face.a--;
      face.a_uv--;
      face.b--;
      face.b_uv--;
      face.c--;
      face.c_uv--;
      face.color = 0xFFFFFFFF;
      array_push(mesh.faces, face);
    }
  }
  fclose(file_handle);
}