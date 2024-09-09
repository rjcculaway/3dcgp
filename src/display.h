#ifndef DISPLAY_3DCGP
#define DISPLAY_3DCGP

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "vector.h"

#define FPS 30
#define FRAME_TIME (1000 / FPS)

/**
 * Display
 * Contains functions for displaying the window and the color buffer.
 */

typedef enum render_method
{
  RENDER_WIREFRAME,
  RENDER_WIREFRAME_DOT,
  RENDER_WIREFRAME_TRIANGLE,
  RENDER_TRIANGLE,
  RENDER_TEXTURED_TRIANGLE,
  RENDER_TEXTURED_WIREFRAME_TRIANGLE,
} render_method;

typedef enum culling_option
{
  CULLING_NONE,
  CULLING_BACKFACE
} culling_option;

typedef uint32_t color_t;

extern int window_width;
extern int window_height;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern color_t *color_buffer;             // Raw pixel data
extern SDL_Texture *color_buffer_texture; // Texture to be displayed to the render target

size_t get_pixel(const size_t i, const size_t j);
bool initialize_window(void);
void destroy_window(void);
void render_color_buffer(void);
void clear_color_buffer(color_t color);

// Drawing Functions
bool is_valid_pixel(int x, int y);
void draw_pixel(int x, int y, color_t color);
void draw_rect(int x, int y, int width, int height, color_t color);
void draw_grid(void);
void draw_line(int x0, int y0, int x1, int y1, color_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);

#endif