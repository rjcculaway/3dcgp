#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mesh.h"
#include "array.h"

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1}, // 1
    {.x = -1, .y = 1, .z = -1},  // 2
    {.x = 1, .y = 1, .z = -1},   // 3
    {.x = 1, .y = -1, .z = -1},  // 4
    {.x = 1, .y = 1, .z = 1},    // 5
    {.x = 1, .y = -1, .z = 1},   // 6
    {.x = -1, .y = 1, .z = 1},   // 7
    {.x = -1, .y = -1, .z = 1},  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // Front
    {.a = 1, .b = 2, .c = 3, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFFFFFFFF},
    {.a = 1, .b = 3, .c = 4, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFFFFFFFF},
    // Right
    {.a = 4, .b = 3, .c = 5, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFFFFFFFF},
    {.a = 4, .b = 5, .c = 6, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFFFFFFFF},
    // Back
    {.a = 6, .b = 5, .c = 7, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFFFFFFFF},
    {.a = 6, .b = 7, .c = 8, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFFFFFFFF},
    // Left
    {.a = 8, .b = 7, .c = 2, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFFFFFFFF},
    {.a = 8, .b = 2, .c = 1, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFFFFFFFF},
    // Top
    {.a = 2, .b = 7, .c = 5, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFFFFFFFF},
    {.a = 2, .b = 5, .c = 3, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFFFFFFFF},
    // Bottom
    {.a = 6, .b = 8, .c = 1, .a_uv = {0, 1}, .b_uv = {0, 0}, .c_uv = {1, 0}, .color = 0xFFFFFFFF},
    {.a = 6, .b = 1, .c = 4, .a_uv = {0, 1}, .b_uv = {1, 0}, .c_uv = {1, 1}, .color = 0xFFFFFFFF}};

mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
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
    if (strncmp("f ", line, 2) == 0) // faces, ignore other values (%*d) for now
    {
      face_t face;
      sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &(face.a), &(face.b), &(face.c));
      face.color = 0xFFFFFFFF;
      array_push(mesh.faces, face);
    }
  }
  fclose(file_handle);
}

void load_cube_mesh_data(void)
{
  for (int i = 0; i < N_CUBE_VERTICES; i++)
  {
    vec3_t cube_vertex = cube_vertices[i];
    array_push(mesh.vertices, cube_vertex);
  }
  for (int i = 0; i < N_CUBE_FACES; i++)
  {
    face_t cube_face = cube_faces[i];
    array_push(mesh.faces, cube_face);
  }
}

// TODO: Functions for meshes