#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

SDL_Window* window = false;
SDL_Renderer* renderer = NULL;
bool is_running = false;

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
      800, // width
      600, // height
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
  // TODO
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

  return 0;
}