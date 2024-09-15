#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mesh.h"
#include "array.h"

static mesh_t meshes[MAX_NUM_MESHES] = {};
static size_t mesh_count = 0;

mesh_t *get_mesh(size_t idx)
{
  return &meshes[idx];
}

mesh_t *get_meshes()
{
  return meshes;
}
size_t get_mesh_count()
{
  return mesh_count;
}

void load_mesh(char *file_name, char *png_texture_file_name, vec3_t scale, vec3_t rotation, vec3_t translation)
{
  // Load the .obj file
  if (mesh_count >= MAX_NUM_MESHES)
  {
    fprintf(stderr, "ERROR: Mesh could not be loaded. Maximum number of meshes (%d) is already met.\n", MAX_NUM_MESHES);
    return;
  }
  mesh_t mesh = load_obj_from_file(file_name);
  mesh.scale = scale;
  mesh.rotation = rotation;
  mesh.translation = translation;

  // Load the .png file
  upng_t *png_handle = load_mesh_png(png_texture_file_name);
  mesh.texture = png_handle;

  meshes[mesh_count++] = mesh;
}

mesh_t load_obj_from_file(char *file_name)
{
  FILE *file_handle = fopen(file_name, "r");

  mesh_t mesh = {};

  if (file_handle == NULL)
  {
    fprintf(stderr, "ERROR: Could not load .obj file.\n");
    return mesh;
  }

  char line[1024] = {0};

  // Load the .obj file
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
      int a_uv_idx = 0;
      int b_uv_idx = 0;
      int c_uv_idx = 0;
      face_t face;
      sscanf(line, "f %d/%d/%*d %d/%d/%*d %d/%d/%*d", &(face.a), &(a_uv_idx), &(face.b), &(b_uv_idx), &(face.c), &(c_uv_idx));
      // Indices start at 1 so we subtract 1 for zero-indexing.
      face.a--;
      face.a_uv = mesh.texcoords[a_uv_idx - 1];
      face.b--;
      face.b_uv = mesh.texcoords[b_uv_idx - 1];
      face.c--;
      face.c_uv = mesh.texcoords[c_uv_idx - 1];
      face.color = 0xFFFFFFFF;
      array_push(mesh.faces, face);
    }
  }
  fclose(file_handle);

  return mesh;
}

void free_mesh(mesh_t mesh)
{
  upng_free(mesh.texture);
  array_free(mesh.faces);
  array_free(mesh.vertices);
  array_free(mesh.texcoords);
}
void free_meshes()
{
  for (size_t i = 0; i < mesh_count; i++)
  {
    free_mesh(meshes[i]);
  }
  mesh_count = 0;
}

upng_t *load_mesh_png(char *file_path)
{
  upng_t *png_texture = upng_new_from_file(file_path);
  if (png_texture != NULL)
  {
    upng_decode(png_texture);
    if (upng_get_error(png_texture) != UPNG_EOK)
    {
      fprintf(stderr, "ERROR: Failed to load mesh texture.\n");
      return NULL;
    }
  }
  return png_texture;
}