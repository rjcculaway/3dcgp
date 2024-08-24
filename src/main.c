#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "display.h"
#include "vector.h"

const int N_POINTS = 9 * 9 * 9;
vec3_t cube_points[N_POINTS]; 
vec2_t projected_points[N_POINTS];

float fov_factor = 360;

bool is_running = false;

bool setup(void) {
  // Allocate memory for the color buffer
  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

  if (color_buffer == NULL) {
    fprintf(stderr, "ERROR: Failed to allocated memory for the color buffer.");
    return false;
  }

  // Create the SDL Texture to display the color buffer
  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888, // Pixel Format
    SDL_TEXTUREACCESS_STREAMING,  // Changes will automatically update the texture
    window_width,
    window_height
  );

  int point_count = 0;
  // Generate points for the cube from -1 to 1
  for (float x = -1; x <= 1; x += 0.25) {
    for (float y = -1; y <= 1; y += 0.25) {
      for (float z = -1; z <= 1; z += 0.25) {
        vec3_t point = {.x = x, .y = y, .z = z};
        cube_points[point_count++] = point;
      }
    }
  }

  return true;
}

void process_input(void) {
  SDL_Event event;

  SDL_PollEvent(&event);
  
  switch (event.type) {
    case SDL_QUIT:  // Close button
      is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_running = false;
      }
      break;
    default:
      break;
  }
}

vec2_t project(vec3_t point) {
  // Parallel projection (ignore z component)
  vec2_t projected_point = { 
    .x = point.x * fov_factor, 
    .y = point.y * fov_factor
  };
  return projected_point;
}

void update(void) {
  for (int i = 0; i < N_POINTS; i++) {
    vec3_t point = cube_points[i];
    
    // Project the point
    vec2_t projected_point = project(point);
    projected_points[i] = projected_point;

  }
}

void render(void) {
  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
  SDL_RenderClear(renderer);

  draw_grid();
  // draw_rect(0, 0, 200, 200, 0xFF00FF00);
  // Draw all projected points from the cube
  for (int i = 0; i < N_POINTS; i++) {
    vec2_t projected_point = projected_points[i];
    draw_rect(
      projected_point.y + window_width / 2,
      projected_point.x + window_height / 2,
      12,
      12,
      0xFFFFFF00
    );
  }

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
}

int main(void) {
  
  is_running = initialize_window();

  // Game loop:
  // 1. Process input.
  // 2. Update.
  // 3. Render.
  bool is_setup_success = setup();
  if (!is_setup_success) {{
    fprintf(stderr, "ERROR: Could not perform setup.\n");
    return 1;
  }}

  while (is_running) {
    process_input();
    update();
    render();
  }
  destroy_window();

  return 0;
}