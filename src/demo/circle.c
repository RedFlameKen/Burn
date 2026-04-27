#include "../loader.c"

static Color pixels[DEFAULT_WIDTH * DEFAULT_HEIGHT] = {0};

BurnCanvas burn_render(float dt){
  BurnCanvas canvas = (BurnCanvas){
    .pixels = pixels,
    .width = DEFAULT_WIDTH,
    .height = DEFAULT_HEIGHT,
  };

  burn_fill(canvas, (Color){
      .a = 0x00,
      });

  static i32 x = DEFAULT_WIDTH / 4;
  static i32 y = DEFAULT_HEIGHT / 2;
  i32 r = 150;

  burn_draw_circle(canvas, x, y, r, (Color){
        .a = 0xFF,
        .r = 0xFF,
      });

  burn_draw_circle_filled(canvas, x+DEFAULT_WIDTH / 2, y, r, (Color){
        .a = 0xFF,
        .r = 0xFF,
      });

  return canvas;
}

void burn_update(float dt){
}
