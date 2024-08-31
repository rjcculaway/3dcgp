#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

render_method current_render_method = RENDER_TRIANGLE;
culling_option current_culling_option = CULLING_BACKFACE;

triangle_t *triangles_to_render = NULL;

vec3_t camera_position = {.x = 0, .y = 0, .z = 0};

float fov_factor = 360;

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

  // Create the SDL Texture to display the color buffer
  color_buffer_texture = SDL_CreateTexture(
      renderer,
      SDL_PIXELFORMAT_ARGB8888,    // Pixel Format
      SDL_TEXTUREACCESS_STREAMING, // Changes will automatically update the texture
      window_width,
      window_height);

  // Load mesh data from file
  // load_mesh_from_file("./assets/cube.obj");
  load_cube_mesh_data();
  printf("vertices: %d, faces: %d\n", array_length(mesh.vertices), array_length(mesh.faces));

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
      camera_position.z -= 0.1;
      break;
    case SDLK_DOWN:
      camera_position.z += 0.1;
      break;
    case SDLK_LEFT:
      camera_position.x -= 0.1;
      break;
    case SDLK_RIGHT:
      camera_position.x += 0.1;
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

  triangles_to_render = NULL;

  previous_frame_time = SDL_GetTicks64();

  mesh.rotation.x += 0.005;
  mesh.rotation.y += 0.01;
  mesh.rotation.z += 0.005;

  for (int i = 0; i < array_length(mesh.faces); i++)
  {
    face_t face = mesh.faces[i];

    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[face.a - 1];
    face_vertices[1] = mesh.vertices[face.b - 1];
    face_vertices[2] = mesh.vertices[face.c - 1];

    vec3_t transformed_vertices[3];

    // Apply transformations and projection to each vertex of this face
    for (int j = 0; j < 3; j++)
    {
      vec3_t transformed_vertex = face_vertices[j];
      // Rotate
      transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
      transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
      transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);
      // Translate
      transformed_vertex.x += 0;
      transformed_vertex.y += 0;
      transformed_vertex.z += 5;

      transformed_vertices[j] = transformed_vertex;
    }

    bool shouldCull = false;

    // Backface Culling
    /*   A   */
    /*  / \  */
    /* B---C */
    vec3_t vec_ab = vec3_normalize(vec3_sub(transformed_vertices[1], transformed_vertices[0])); // B - A
    vec3_t vec_ac = vec3_normalize(vec3_sub(transformed_vertices[2], transformed_vertices[0])); // C - A

    // Left-handed coordinate system (+z is away), so the order of the cross product
    // must be b-a x a-b
    vec3_t normal = vec3_cross(
        vec_ab,
        vec_ac);
    vec3_t camera_ray = vec3_sub(camera_position, transformed_vertices[0]); // Vector from camera to point A

    // Normalize the face normal
    vec3_t normalized_normal = vec3_normalize(normal);

    float dot = vec3_dot(normalized_normal, camera_ray);

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

    vec2_t projected_points[3];
    for (int j = 0; j < 3; j++)
    {
      // Project
      projected_points[j] = project(transformed_vertices[j]);

      // Scale and translate projected points to the middle of the screen
      projected_points[j].x += window_width / 2;
      projected_points[j].y += window_height / 2;
    }
    triangle_t projected_triangle = {
        .points = {
            {.x = projected_points[0].x, projected_points[0].y},
            {.x = projected_points[1].x, projected_points[1].y},
            {.x = projected_points[2].x, projected_points[2].y}},
        .color = face.color};
    array_push(triangles_to_render, projected_triangle);
  }
}

void render(void)
{
  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
  SDL_RenderClear(renderer);

  draw_grid();
  // draw_rect(0, 0, 200, 200, 0xFF00FF00);
  int n_triangles = array_length(triangles_to_render);
  // Draw all projected points from the cube
  for (int i = 0; i < n_triangles; i++)
  {
    triangle_t triangle = triangles_to_render[i];
    int x0 = triangle.points[0].x;
    int x1 = triangle.points[1].x;
    int x2 = triangle.points[2].x;
    int y0 = triangle.points[0].y;
    int y1 = triangle.points[1].y;
    int y2 = triangle.points[2].y;

    switch (current_render_method)
    {
    case RENDER_WIREFRAME:
      draw_triangle(
          x0,
          y0,
          x1,
          y1,
          x2,
          y2,
          triangle.color);
      break;
    case RENDER_WIREFRAME_DOT:
      draw_triangle(
          x0,
          y0,
          x1,
          y1,
          x2,
          y2,
          triangle.color);
      const int point_size = 4;
      draw_rect(x0 - point_size / 2, y0 - point_size / 2, point_size, point_size, 0xFFFF0000);
      draw_rect(x1 - point_size / 2, y1 - point_size / 2, point_size, point_size, 0xFFFF0000);
      draw_rect(x2 - point_size / 2, y2 - point_size / 2, point_size, point_size, 0xFFFF0000);
      break;
    case RENDER_WIREFRAME_TRIANGLE:
      draw_filled_triangle(
          x0,
          y0,
          x1,
          y1,
          x2,
          y2,
          triangle.color);
      draw_triangle(
          x0,
          y0,
          x1,
          y1,
          x2,
          y2,
          0xFF000000);
      break;
    case RENDER_TRIANGLE:
      draw_filled_triangle(
          x0,
          y0,
          x1,
          y1,
          x2,
          y2,
          triangle.color);
      break;
    default:
      fprintf(stderr, "WARNING: Invalid culling option selected!");
      break;
    }
  }

  // Clear the triangle dynamic array
  array_free(triangles_to_render);

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
}

void free_resources(void)
{
  free(color_buffer);
  color_buffer = NULL;
  array_free(mesh.faces);
  array_free(mesh.vertices);
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