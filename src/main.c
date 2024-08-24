#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

int window_width = 800;
int window_height = 600;

bool is_running = false;
SDL_Window* window = false;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;  // Raw pixel data
SDL_Texture* color_buffer_texture = NULL; // Texture to be displayed to the render target

size_t get_pixel(const size_t i, const size_t j) {
  return (window_width * i) + j;
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
  for (int row = y; row < height && row < window_height; row++) {
    for (int col = x; col < width && col < window_width; col++) {
      color_buffer[get_pixel(row, col)] = color;
    }
  }
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

bool setup(void) {
  // Allocate memory for the color buffer
  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);

  if (color_buffer == NULL) {
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

void update(void) {
  // TODO
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

void render(void) {
  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
  SDL_RenderClear(renderer);

  draw_rect(0, 0, 200, 200, 0xFF00FF00);

  render_color_buffer();
  clear_color_buffer(0xFF000000);

  SDL_RenderPresent(renderer);
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


int main(void) {
  
  is_running = initialize_window();

  // Game loop:
  // 1. Process input.
  // 2. Update.
  // 3. Render.
  bool is_setup = setup();
  if (!is_setup) {{
    printf("Could not setup.\n");
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