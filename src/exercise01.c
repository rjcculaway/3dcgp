#include <stdint.h>

void draw_grid(uint32_t* color_buffer, const int width, const int height) {
  for (int y = 0; y < height; y += 10) {
    for (int x = 0; x < width; x += 10) {
      color_buffer[y * height + x] = 0xFF888888;
    }
  }
}