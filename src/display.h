#ifndef DISPLAY_RENENGINE_SFW
#define DISPLAY_RENENGINE_SFW

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "vector.h"

#define FPS 200
#define FRAME_TIME (1000 / FPS)

/**
 * Display
 * Contains functions for displaying the window and the color buffer.
 */

typedef enum RenderMethod
{
  RENDER_WIREFRAME,
  RENDER_WIREFRAME_DOT,
  RENDER_WIREFRAME_TRIANGLE,
  RENDER_TRIANGLE,
  RENDER_TEXTURED_TRIANGLE,
  RENDER_TEXTURED_WIREFRAME_TRIANGLE,
} RenderMethod;

typedef enum BackfaceCullingOption
{
  CULLING_NONE,
  CULLING_BACKFACE
} BackfaceCullingOption;

typedef uint32_t color_t;

int get_window_width(void);
int get_window_height(void);

RenderMethod get_render_method(void);
BackfaceCullingOption get_backface_culling_option(void);
void set_render_method(RenderMethod render_method);
void set_backface_culling_option(BackfaceCullingOption backface_culling_option);

size_t get_pixel(const size_t i, const size_t j);
bool initialize_window(void);
void destroy_window(void);
void render_color_buffer(void);
void clear_color_buffer(color_t color);
void clear_z_buffer(void);

// Drawing Functions
bool is_valid_pixel(int x, int y);
void draw_pixel(int x, int y, color_t color);
void draw_rect(int x, int y, int width, int height, color_t color);
void draw_grid(void);
void draw_line(int x0, int y0, int x1, int y1, color_t color);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color);

float get_z_buffer_at(int x, int y);
void update_z_buffer_at(int x, int y, float value);
#endif