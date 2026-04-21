#include "../loader.c"

static Color pixels[DEFAULT_WIDTH * DEFAULT_HEIGHT] = {0};

#define PI 3.14159265

static float angle = 0;

BurnCanvas burn_render(float dt){
  angle += 0.5*PI*(dt*0.01);
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

  vec2f v1 = (vec2f){ .x = 250, .y = 75 };
  vec2f v2 = (vec2f){ .x = 150, .y = 375 };
  vec2f v3 = (vec2f){ .x = 600, .y = 425 };

  vec2f center = (vec2f){
    .x = (v1.x + v2.x + v3.x) / 3,
    .y = (v1.y + v2.y + v3.y) / 3
  };

  v1 = burn_rotate_2d(v1, center, angle);
  v2 = burn_rotate_2d(v2, center, angle);
  v3 = burn_rotate_2d(v3, center, angle);
  burn_fill_triangle3c(canvas, v1, v2, v3, c1, c2, c3);

  return canvas;
}

void burn_update(float dt){
}
