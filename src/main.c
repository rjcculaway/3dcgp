#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>

#include "utils.h"
#include "array.h"
#include "vector.h"
#include "matrix.h"
#include "display.h"
#include "mesh.h"
#include "camera.h"
#include "light.h"

render_method current_render_method = RENDER_TEXTURED_TRIANGLE;
culling_option current_culling_option = CULLING_BACKFACE;

#define MAX_TRIANGLES_PER_MESH 10000
triangle_t triangles_to_render[MAX_TRIANGLES_PER_MESH];
int num_triangles_to_render = 0;

//--------------------------------------------
// Global transformation matrices
//--------------------------------------------
mat4_t world_matrix;
mat4_t view_matrix;
mat4_t projection_matrix;

// Up vector for the view matrix
const vec3_t up = {0, 1, 0};

bool is_running = false;
uint64_t previous_frame_time = 0;

bool setup(void)
{
  // Allocate memory for the color buffer
  // This is a contiguous block of memory but we will interpret it as a 2D array
  color_buffer = (color_t *)malloc(sizeof(color_t) * window_width * window_height);

  if (color_buffer == NULL)
  {
    fprintf(stderr, "ERROR: Failed to allocate memory for the color buffer.");
    return false;
  }

  // Allocate memory for the z-buffer
  // Also a contiguous block of memory interpreted as a 2D array
  z_buffer = (float *)malloc(sizeof(float) * window_width * window_height);

  if (z_buffer == NULL)
  {
    fprintf(stderr, "ERROR: Failed to allocate memory for the z-buffer.");
    return false;
  }

  // Create the SDL Texture to display the color buffer
  color_buffer_texture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_RGBA32,      // Pixel Format
      SDL_TEXTUREACCESS_STREAMING, // Changes will automatically update the texture
      window_width,
      window_height);

  // Load texture data from .png file
  load_png_texture_data("./assets/efa.png");

  // Load mesh data from file
  load_mesh_from_file("./assets/efa.obj");

  printf("vertices: %d, faces: %d, uvs: %d\n", array_length(mesh.vertices), array_length(mesh.faces), array_length(mesh.texcoords));

  // Setup the projection matrix
  float fov = M_PI / 3;
  float aspect = (float)window_height / window_width;
  float z_near = 0.1;
  float z_far = 100.0;
  projection_matrix = mat4_make_perspective(fov, aspect, z_near, z_far);

  return true;
}

void process_input(void)
{
  SDL_Event event;

  SDL_PollEvent(&event);

  switch (event.type)
  {
  case SDL_QUIT: // Close button
    is_running = false;
    break;
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym)
    {
    case SDLK_ESCAPE:
      is_running = false;
      break;
    case SDLK_UP:
      camera.position.z += 0.1;
      break;
    case SDLK_DOWN:
      camera.position.z -= 0.1;
      break;
    case SDLK_LEFT:
      camera.position.x -= 0.1;
      break;
    case SDLK_RIGHT:
      camera.position.x += 0.1;
      break;
    case SDLK_1:
      current_render_method = RENDER_WIREFRAME_DOT;
      break;
    case SDLK_2:
      current_render_method = RENDER_WIREFRAME;
      break;
    case SDLK_3:
      current_render_method = RENDER_TRIANGLE;
      break;
    case SDLK_4:
      current_render_method = RENDER_WIREFRAME_TRIANGLE;
      break;
    case SDLK_5:
      current_render_method = RENDER_TEXTURED_TRIANGLE;
      break;
    case SDLK_6:
      current_render_method = RENDER_TEXTURED_WIREFRAME_TRIANGLE;
      break;
    case SDLK_c:
      current_culling_option = CULLING_BACKFACE;
      break;
    case SDLK_d:
      current_culling_option = CULLING_NONE;
      break;
    }
    break;
  default:
    break;
  }
}

void update(void)
{
  // Determine if we still have time to wait before the next frame
  uint64_t time_to_wait = FRAME_TIME - (SDL_GetTicks64() - previous_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TIME)
  {
    SDL_Delay(time_to_wait); // Delay update until enough time has passed.
  }

  previous_frame_time = SDL_GetTicks64();

  memset(triangles_to_render, 0, sizeof(triangle_t) * MAX_TRIANGLES_PER_MESH);
  num_triangles_to_render = 0;

  // Mesh Animation
  // mesh.rotation.x += 0.006;
  // mesh.rotation.y += 0.01;
  // mesh.rotation.z += 0.01;

  // mesh.scale.x += 0.002;
  // mesh.scale.y += 0.001;
  // mesh.translation.x += 0.01;
  mesh.translation.z = 4.0;

  // Camera Animation
  camera.position.x += 0.008;
  camera.position.y += 0.008;

  // Create a scale, rotation, translation matrix
  mat4_t scale_mat = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.y);
  mat4_t rotation_x_mat = mat4_make_rotation_x(mesh.rotation.x);
  mat4_t rotation_y_mat = mat4_make_rotation_y(mesh.rotation.y);
  mat4_t rotation_z_mat = mat4_make_rotation_z(mesh.rotation.z);
  mat4_t translation_mat = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

  // Create a world matrix to combine the three transformations
  world_matrix = mat4_identity();

  // Multiply the transformations to the world matrix
  world_matrix = mat4_matmul_mat4(scale_mat, world_matrix);
  world_matrix = mat4_matmul_mat4(rotation_z_mat, world_matrix);
  world_matrix = mat4_matmul_mat4(rotation_y_mat, world_matrix);
  world_matrix = mat4_matmul_mat4(rotation_x_mat, world_matrix);
  world_matrix = mat4_matmul_mat4(translation_mat, world_matrix);

  // Create a view matrix to transform the coordinate system to the camera's
  const vec3_t target = {0, 0, 4.0};
  view_matrix = mat4_look_at(camera.position, target, up);

  // Multiply the world matrix by the view matrix to combine the transformations
  mat4_t view_from_world_matrix = mat4_matmul_mat4(view_matrix, world_matrix);

  for (int i = 0; i < array_length(mesh.faces); i++)
  {
    face_t face = mesh.faces[i];

    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[face.a];
    face_vertices[1] = mesh.vertices[face.b];
    face_vertices[2] = mesh.vertices[face.c];

    vec4_t transformed_vertices[3];

    // Apply transformations and projection to each vertex of this face
    for (int j = 0; j < 3; j++)
    {
      vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

      // Multiply the view-world matrix to the original vector
      transformed_vertex = mat4_matmul_vec(view_from_world_matrix, transformed_vertex);

      transformed_vertices[j] = transformed_vertex;
    }

    bool shouldCull = false;

    // Backface Culling
    /*   A   */
    /*  / \  */
    /* B---C */
    vec3_t vec_a = vec3_from_vec4(transformed_vertices[0]);
    vec3_t vec_b = vec3_from_vec4(transformed_vertices[1]);
    vec3_t vec_c = vec3_from_vec4(transformed_vertices[2]);
    vec3_t vec_ab = vec3_normalize(vec3_sub(vec_b, vec_a)); // B - A
    vec3_t vec_ac = vec3_normalize(vec3_sub(vec_c, vec_a)); // C - A

    // Left-handed coordinate system (+z is away), so the order of the cross product
    // must be b-a x a-b
    vec3_t normal = vec3_normalize(vec3_cross(
        vec_ab,
        vec_ac));
    vec3_t origin = {0, 0, 0};
    // There is no need to use the camera position for the camera ray. By the end of view matrix multiplication, the camera WILL be at the origin.
    vec3_t camera_ray = vec3_sub(origin, vec_a); // Vector from camera to point A

    float dot = vec3_dot(normal, camera_ray);

    switch (current_culling_option)
    {
    case CULLING_NONE:
      break;
    case CULLING_BACKFACE:
      if (dot < 0.0) // If dot < 0, then it should not be rendered
      {
        shouldCull = true;
      }
      break;
    default:
      fprintf(stderr, "WARNING: Invalid culling option selected!");
      break;
    }
    if (shouldCull)
    {
      continue;
    }

    vec4_t projected_points[3];
    for (int j = 0; j < 3; j++)
    {
      // Project
      projected_points[j] = mat4_matmul_vec_project(projection_matrix, transformed_vertices[j]);

      // Move to screen space
      // Scale into the view
      projected_points[j].x *= (window_width / 2.0);
      projected_points[j].y *= (window_height / 2.0);

      // Invert the y values since the y value in screen space grows downward from the top
      projected_points[j].y *= -1;

      // Translate projected points to the middle of the screen
      projected_points[j].x += (window_width / 2.0);
      projected_points[j].y += (window_height / 2.0);
    }

    float light_intensity = light_lambertian(normal, sunlight.direction);
    color_t final_color = light_apply_intensity(face.color, light_intensity);

    triangle_t projected_triangle = {
        .points = {
            {.x = projected_points[0].x, .y = projected_points[0].y, .z = projected_points[0].z, .w = projected_points[0].w},
            {.x = projected_points[1].x, .y = projected_points[1].y, .z = projected_points[1].z, .w = projected_points[1].w},
            {.x = projected_points[2].x, .y = projected_points[2].y, .z = projected_points[2].z, .w = projected_points[2].w}},
        .texcoords = {mesh.texcoords[face.a_uv], mesh.texcoords[face.b_uv], mesh.texcoords[face.c_uv]},
        .color = final_color,
    };
    if (num_triangles_to_render < MAX_TRIANGLES_PER_MESH)
    {
      triangles_to_render[num_triangles_to_render] = projected_triangle;
      num_triangles_to_render++;
    }
  }

  // Without the z-buffer, we would need to sort the triangles by z here (Painter's algorithm).
}

void render(void)
{
  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
  SDL_RenderClear(renderer);

  draw_grid();
  // draw_rect(0, 0, 200, 200, 0xFF00FF00);
  // Draw all projected points from the cube
  for (int i = 0; i < num_triangles_to_render; i++)
  {
    triangle_t triangle = triangles_to_render[i];
    int x0 = triangle.points[0].x;
    int y0 = triangle.points[0].y;

    int x1 = triangle.points[1].x;
    int y1 = triangle.points[1].y;

    int x2 = triangle.points[2].x;
    int y2 = triangle.points[2].y;

    switch (current_render_method)
    {
    case RENDER_WIREFRAME:
      draw_triangle(
          x0, y0,
          x1, y1,
          x2, y2,
          triangle.color);
      break;
    case RENDER_WIREFRAME_DOT:
      draw_triangle(
          x0, y0,
          x1, y1,
          x2, y2,
          triangle.color);
      const int point_size = 4;
      draw_rect(x0 - point_size / 2, y0 - point_size / 2, point_size, point_size, 0xFFFF0000);
      draw_rect(x1 - point_size / 2, y1 - point_size / 2, point_size, point_size, 0xFFFF0000);
      draw_rect(x2 - point_size / 2, y2 - point_size / 2, point_size, point_size, 0xFFFF0000);
      break;
    case RENDER_WIREFRAME_TRIANGLE:
      draw_filled_triangle(
          triangle,
          triangle.color);
      draw_triangle(
          x0, y0,
          x1, y1,
          x2, y2,
          0xFF000000);
      break;
    case RENDER_TRIANGLE:
      draw_filled_triangle(
          triangle,
          triangle.color);
      break;
    case RENDER_TEXTURED_TRIANGLE:
      draw_textured_triangle(
          triangle,
          mesh_texture);
      break;
    case RENDER_TEXTURED_WIREFRAME_TRIANGLE:
      draw_textured_triangle(
          triangle,
          mesh_texture);
      draw_triangle(
          x0, y0,
          x1, y1,
          x2, y2,
          0xFF000000);
      break;
    default:
      fprintf(stderr, "WARNING: Invalid culling option selected!");
      break;
    }
  }

  render_color_buffer();
  clear_color_buffer(0xFF000000);
  clear_z_buffer();

  SDL_RenderPresent(renderer);
}

void free_resources(void)
{
  upng_free(png_texture);
  free(z_buffer);
  z_buffer = NULL;
  free(color_buffer);
  color_buffer = NULL;
  array_free(mesh.faces);
  array_free(mesh.vertices);
  array_free(mesh.texcoords);
}

int main(void)
{

  is_running = initialize_window();

  // Game loop:
  // 1. Process input.
  // 2. Update.
  // 3. Render.
  bool is_setup_success = setup();
  if (!is_setup_success)
  {
    {
      fprintf(stderr, "ERROR: Could not perform setup.\n");
      return 1;
    }
  }

  while (is_running)
  {
    process_input();
    update();
    render();
  }

  free_resources();
  destroy_window();

  return 0;
}