#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"

triangle_t triangles_to_render[N_MESH_FACES];

vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
vec3_t cube_rotation = {.x = 0, .y = 0, .z = 0};

float fov_factor = 360;

bool is_running = false;
uint64_t previous_frame_time = 0;

bool setup(void)
{
  // Allocate memory for the color buffer
  // This is a contiguous block of memory but we will interpret it as a 2D array
  color_buffer = (uint32_t *)malloc(sizeof(uint32_t) * window_width * window_height);

  if (color_buffer == NULL)
  {
    fprintf(stderr, "ERROR: Failed to allocate memory for the color buffer.");
    return false;
  }

  // Create the SDL Texture to display the color buffer
  color_buffer_texture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_ARGB8888,    // Pixel Format
      SDL_TEXTUREACCESS_STREAMING, // Changes will automatically update the texture
      window_width,
      window_height);

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
    if (event.key.keysym.sym == SDLK_ESCAPE)
    {
      is_running = false;
    }
    if (event.key.keysym.sym == SDLK_UP)
    {
      camera_position.z -= 0.1;
    }
    if (event.key.keysym.sym == SDLK_DOWN)
    {
      camera_position.z += 0.1;
    }
    if (event.key.keysym.sym == SDLK_LEFT)
    {
      camera_position.x -= 0.1;
    }
    if (event.key.keysym.sym == SDLK_RIGHT)
    {
      camera_position.x += 0.1;
    }
    break;
  default:
    break;
  }
}

vec2_t parallel_project(vec3_t point)
{
  // Parallel projection (ignore z component)
  vec2_t projected_point = {
      .x = point.x * fov_factor,
      .y = point.y * fov_factor};
  return projected_point;
}

vec2_t project(vec3_t point)
{
  // Perspective projection (apply perspective divide)
  vec2_t projected_point = {
      .x = point.x / point.z * fov_factor,
      .y = point.y / point.z * fov_factor};
  return projected_point;
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

  cube_rotation.x += 0.005;
  cube_rotation.y += 0.01;
  cube_rotation.z += 0.005;

  for (int i = 0; i < N_MESH_FACES; i++)
  {
    face_t face = mesh_faces[i];

    vec3_t face_vertices[3];
    face_vertices[0] = mesh_vertices[face.a - 1];
    face_vertices[1] = mesh_vertices[face.b - 1];
    face_vertices[2] = mesh_vertices[face.c - 1];

    triangle_t projected_triangle;

    // Apply transformations and projection to each vertex of this face
    for (int j = 0; j < 3; j++)
    {
      vec3_t transformed_vertex = face_vertices[j];
      // Rotate
      transformed_vertex = vec3_rotate_x(transformed_vertex, cube_rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, cube_rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, cube_rotation.z);
      // Translate
      transformed_vertex.x -= camera_position.x;
      transformed_vertex.y -= camera_position.y;
      transformed_vertex.z -= camera_position.z;
      // Project
      vec2_t projected_point = project(transformed_vertex);

      // Scale and translate projected points to the middle of the screen
      projected_point.x += window_width / 2;
      projected_point.y += window_height / 2;

      projected_triangle.points[j] = projected_point;
    }
    triangles_to_render[i] = projected_triangle;
  }
}

void render(void)
{
  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
  SDL_RenderClear(renderer);

  draw_grid();
  // draw_rect(0, 0, 200, 200, 0xFF00FF00);
  // Draw all projected points from the cube
  for (int i = 0; i < N_MESH_FACES; i++)
  {
    triangle_t triangle = triangles_to_render[i];
    for (int j = 0; j < 3; j++)
    {
      draw_rect(
          triangle.points[j].x,
          triangle.points[j].y,
          4,
          4,
          0xFFFFFF00);
    }
  }

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
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
  destroy_window();

  return 0;
}