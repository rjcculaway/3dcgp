#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

bool is_running = false;
SDL_Window* window = false;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;  // Raw pixel data
SDL_Texture* color_buffer_texture = NULL; // Texture to be displayed to the render target

size_t get_pixel(const size_t i, const size_t j) {
  return (WINDOW_WIDTH * i) + j;
}

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL.\n");
    return false;
  }
  // Create an SDL Window
  window = SDL_CreateWindow(
      NULL, // window title 
      SDL_WINDOWPOS_CENTERED, // pos x
      SDL_WINDOWPOS_CENTERED, // pos y
      WINDOW_WIDTH, // width
      WINDOW_HEIGHT, // height
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

  return true;
}

void setup(void) {
  // Allocate memory for the color buffer
  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);

  // Create the SDL Texture to display the color buffer
  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888, // Pixel Format
    SDL_TEXTUREACCESS_STREAMING,  // Changes will automatically update the texture
    WINDOW_WIDTH,
    WINDOW_HEIGHT
  );
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
    (int)(WINDOW_WIDTH * sizeof(uint32_t))
  );
  SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}

/**
 * Fills the buffer with a color.
 * @param color The color (in ARGB8888 format) to fill the color buffer with.
 */
void clear_color_buffer(uint32_t color) {
  for (int y = 0; y < WINDOW_HEIGHT; y++) {
    for (int x = 0; x < WINDOW_WIDTH; x++) {
      color_buffer[get_pixel(y, x)] = color;
    }
  }
}

void render(void) {
  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
  SDL_RenderClear(renderer);

  render_color_buffer();
  clear_color_buffer(0xFFFFFF00);

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
  setup();
  while (is_running) {
    process_input();
    update();
    render();
  }
  destroy_window();

  return 0;
}