#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

bool is_running = false;
SDL_Window* window = false;
SDL_Renderer* renderer = NULL;
uint32_t* color_buffer = NULL;

inline size_t get_pixel(const size_t i, const size_t j) {
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
  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
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

void render(void) {
  SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
  SDL_RenderClear(renderer);

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