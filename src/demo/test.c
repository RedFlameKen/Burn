#include "../loader.c"

static Color pixels[DEFAULT_WIDTH * DEFAULT_HEIGHT] = {0};
static int offset = 0;
BurnCanvas burn_render(float dt){
  BurnCanvas canvas = (BurnCanvas){
    .pixels = pixels,
    .width = DEFAULT_WIDTH,
    .height = DEFAULT_HEIGHT,
  };

  offset+=5;

  Color color = (Color){
      .r = 0xFF,
      .a = 0xFF
      };

  burn_fill(canvas, (Color){
      .g = 0xFF,
      .a = 0xFF
      });

  burn_draw_line(canvas, 50+offset, 25+offset, 100+offset, 25+offset, color);
  burn_draw_rect(canvas, 50+offset, 50, 100, 100, color);
  burn_fill_rect(canvas, 200+offset, 50, 100, 100, color);
  vec2 v1 = (vec2){ .x = 150, .y = 50 };
  vec2 v2 = (vec2){ .x = 150, .y = 100 };
  vec2 v3 = (vec2){ .x = 250, .y = 100 };
  burn_fill_triangle(canvas, v1, v2, v3, color);

  return canvas;
}

void burn_update(float dt){
}
