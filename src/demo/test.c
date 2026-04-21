#include "../loader.c"

static Color pixels[DEFAULT_WIDTH * DEFAULT_HEIGHT] = {0};

BurnCanvas burn_render(float dt){
  BurnCanvas canvas = (BurnCanvas){
    .pixels = pixels,
    .width = DEFAULT_WIDTH,
    .height = DEFAULT_HEIGHT,
  };

  Color color = (Color){
      .r = 0xFF,
      .a = 0xFF
      };

  burn_fill(canvas, (Color){
      .a = 0xFF
      });

  burn_draw_line(canvas, 50, 25, 100, 25, color);
  burn_draw_rect(canvas, 50, 150, 100, 100, color);
  burn_fill_rect(canvas, 200, 150, 100, 100, color);
  vec2f v1 = (vec2f){ .x = 150, .y = 350 };
  vec2f v2 = (vec2f){ .x = 125, .y = 500 };
  vec2f v3 = (vec2f){ .x = 350, .y = 525 };
  burn_fill_triangle(canvas, v1, v2, v3, color);

  return canvas;
}

void burn_update(float dt){
}

