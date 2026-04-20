#include "../loader.c"

static Color pixels[DEFAULT_WIDTH * DEFAULT_HEIGHT] = {0};

// TODO: Triangle is only white in html
BurnCanvas burn_render(float dt){
  BurnCanvas canvas = (BurnCanvas){
    .pixels = pixels,
    .width = DEFAULT_WIDTH,
    .height = DEFAULT_HEIGHT,
  };

  burn_fill(canvas, (Color){
      .a = 0xFF,
      });

  Color c1 = (Color){
      .r = 0xFF,
      .g = 0x00,
      .b = 0x00,
      .a = 0xFF
      };

  Color c2 = (Color){
      .a = 0xFF,
      .r = 0x00,
      .g = 0xFF,
      .b = 0x00,
      };

  Color c3 = (Color){
      .a = 0xFF,
      .r = 0x00,
      .g = 0x00,
      .b = 0xFF
      };

  vec2 v1 = (vec2){ .x = 150, .y = 50 };
  vec2 v2 = (vec2){ .x = 50, .y = 300 };
  vec2 v3 = (vec2){ .x = 400, .y = 350 };
  burn_fill_triangle3c(canvas, v1, v2, v3, c1, c2, c3);

  return canvas;
}

void burn_update(float dt){
}

