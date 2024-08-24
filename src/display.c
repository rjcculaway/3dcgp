#include "display.h"

int window_width = 800;
int window_height = 600;

bool is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
uint32_t *color_buffer = NULL;            // Raw pixel data
SDL_Texture *color_buffer_texture = NULL; // Texture to be displayed to the render target

size_t get_pixel(const size_t i, const size_t j) {
  return (window_width * i) + j;
}

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }
  // Query the screen resolution
  SDL_DisplayMode display_mode;
  SDL_GetCurrentDisplayMode(0, &display_mode);

  window_width = display_mode.w;
  window_height = display_mode.h;

  // Create an SDL Window
  window = SDL_CreateWindow(
      NULL, // window title 
      SDL_WINDOWPOS_CENTERED, // pos x
      SDL_WINDOWPOS_CENTERED, // pos y
      window_width, // width
      window_height, // height
      SDL_WINDOW_BORDERLESS
    );
  if (!window) {
    fprintf(stderr, "Error creating SDL window.\n");
    return false;
  }

  // Create an SDL renderer
  renderer = SDL_CreateRenderer(
    window, // pointer to window
    -1,  // default display device
    0
  );
  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer.\n");
    return false;
  }

  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

  return true;
}
void destroy_window(void) {
  free(color_buffer);
  color_buffer = NULL;
  
  SDL_DestroyRenderer(renderer);
  renderer = NULL;
  
  SDL_DestroyWindow(window);
  window = NULL;
  
  SDL_Quit();
}

void render_color_buffer(void) {
  SDL_UpdateTexture(
    color_buffer_texture,
    NULL,
    color_buffer,
    (int)(window_width * sizeof(uint32_t))
  );
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
/**
 * Fills the buffer with a color.
 * @param color The color (in ARGB8888 format) to fill the color buffer with.
 */
void clear_color_buffer(uint32_t color) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_width; x++) {
      color_buffer[get_pixel(y, x)] = color;
    }
  }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
  for (int row = y; row < height && row < window_height; row++) {
    for (int col = x; col < width && col < window_width; col++) {
      color_buffer[get_pixel(row, col)] = color;
    }
  }
}
void draw_grid(void) {
  for (int y = 0; y < window_height; y++) {
    for (int x = 0; x < window_height; x++) {
      color_buffer[get_pixel(y, x)] = 0xFF444444;
    }
  }
}