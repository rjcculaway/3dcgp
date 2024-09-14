#include "display.h"

static RenderMethod current_render_method = RENDER_TEXTURED_TRIANGLE;
static BackfaceCullingOption current_backface_culling_option = CULLING_BACKFACE;

static SDL_Renderer *renderer = NULL;
static SDL_Window *window = NULL;

static int window_width = 320;
static int window_height = 200;

static color_t *color_buffer = NULL;             // Raw pixel data
static float *z_buffer = NULL;                   // Depth buffer
static SDL_Texture *color_buffer_texture = NULL; // Texture to be displayed to the render target

int get_window_width(void)
{
  return window_width;
}

int get_window_height(void)
{
  return window_height;
}

RenderMethod get_render_method(void)
{
  return current_render_method;
}
BackfaceCullingOption get_backface_culling_option(void)
{
  return current_backface_culling_option;
}

void set_render_method(RenderMethod render_method)
{
  current_render_method = render_method;
}

void set_backface_culling_option(BackfaceCullingOption backface_culling_option)
{
  current_backface_culling_option = backface_culling_option;
}

size_t inline get_pixel(const size_t i, const size_t j)
{
  return (window_width * j) + i;
}

bool initialize_window(void)
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    fprintf(stderr, "ERROR: Failed initializing SDL.\n");
    return false;
  }
  // Query the screen resolution
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  int full_screen_window_width = display_mode.w;
  int full_screen_window_height = display_mode.h;

  // Create an SDL Window
  window = SDL_CreateWindow(
      NULL,                      // window title
      SDL_WINDOWPOS_CENTERED,    // pos x
      SDL_WINDOWPOS_CENTERED,    // pos y
      full_screen_window_width,  // width
      full_screen_window_height, // height
      SDL_WINDOW_BORDERLESS);

  window_width = full_screen_window_width / 3;
  window_height = full_screen_window_height / 3;
  if (!window)
  {
    fprintf(stderr, "ERROR: Failed reating SDL window.\n");
    return false;
  }

  // Create an SDL renderer
  renderer = SDL_CreateRenderer(
      window, // pointer to window
      -1,     // default display device
      0);
  if (!renderer)
  {
    fprintf(stderr, "ERROR: Failed creating SDL renderer.\n");
    return false;
  }

  // SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

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

  printf("resolution: %d x %d\n", window_width, window_height);

  return true;
}
void destroy_window(void)
{
  free(z_buffer);
  z_buffer = NULL;

  free(color_buffer);
  color_buffer = NULL;

  SDL_DestroyRenderer(renderer);
  renderer = NULL;

  SDL_DestroyWindow(window);
  window = NULL;

  SDL_Quit();
}

void render_color_buffer(void)
{
  SDL_UpdateTexture(
      color_buffer_texture,
      NULL,
      color_buffer,
      (int)(window_width * sizeof(color_t)));
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
  SDL_RenderPresent(renderer);
}
/**
 * Fills the buffer with a color.
 * @param color The color (in RGBA32 format) to fill the color buffer with.
 */
void clear_color_buffer(color_t color)
{
  memset(color_buffer, color, sizeof(color_t) * window_width * window_height);
}

void clear_z_buffer(void)
{
  for (int j = 0; j < window_height; j++)
  {
    for (int i = 0; i < window_width; i++)
    {
      z_buffer[get_pixel(i, j)] = 1.0;
    }
  }
  // memset() isn't possible here since floats are multibyte types.
}

float get_z_buffer_at(int x, int y)
{
  if (!is_valid_pixel(x, y))
  {
    return 1.0;
  }
  return z_buffer[get_pixel(x, y)];
}
void update_z_buffer_at(int x, int y, float value)
{
  if (!is_valid_pixel(x, y))
  {
    return;
  }
  z_buffer[get_pixel(x, y)] = value;
}

bool inline is_valid_pixel(int x, int y)
{
  return x < window_width && y < window_height && x >= 0 && y >= 0;
}
void draw_pixel(int x, int y, color_t color)
{
  if (!is_valid_pixel(x, y))
  {
    return;
  }
  color_buffer[get_pixel(x, y)] = color;
}
void draw_rect(int x, int y, int width, int height, color_t color)
{
  for (int current_x = 0; current_x < width; current_x++)
  {
    for (int current_y = 0; current_y < height; current_y++)
    {
      draw_pixel(current_x + x, current_y + y, color);
    }
  }
}
void draw_grid(void)
{
  for (int y = 0; y < window_height; y++)
  {
    for (int x = 0; x < window_width; x++)
    {
      if (x % 50 == 0 || y % 50 == 0)
      {
        draw_pixel(x, y, 0xFF444444);
      }
    }
  }
}
// Digital Differential Analyzer algorithm for drawing lines
void draw_line(int x0, int y0, int x1, int y1, color_t color)
{
  int delta_x = x1 - x0;
  int delta_y = y1 - y0;

  // Get the longest side
  int longest_side_length = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y);

  // Find out how much we should increment in both x and y each step
  float x_inc = delta_x / (float)longest_side_length;
  float y_inc = delta_y / (float)longest_side_length;

  float x_i = x0;
  float y_i = y0;

  for (int i = 0; i <= longest_side_length; i++)
  {
    draw_pixel(round(x_i), round(y_i), color);
    x_i += x_inc;
    y_i += y_inc;
  }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, color_t color)
{
  draw_line(x0,
            y0,
            x1,
            y1,
            color);
  draw_line(x1,
            y1,
            x2,
            y2, color);
  draw_line(x2,
            y2,
            x0,
            y0, color);
}