#include "../loader.c"

#define DEFAULT_SPRITE_WIDTH 300
#define DEFAULT_SPRITE_HEIGHT 300

#define MIN_WIDTH_STRETCH 200
#define MAX_WIDTH_STRETCH 400

#define MIN_HEIGHT_STRETCH 200
#define MAX_HEIGHT_STRETCH 400

#define PI 3.14159265

static Color pixels[DEFAULT_WIDTH * DEFAULT_HEIGHT] = {0};
static Color texture[DEFAULT_SPRITE_WIDTH*DEFAULT_SPRITE_HEIGHT] = {0};
static float sprite_width = DEFAULT_SPRITE_WIDTH;
static float sprite_height = DEFAULT_SPRITE_HEIGHT;
static float x_offset = 0;
static float y_offset = 0;

u8 grow_width = 1;

u8 sprite_initialized = 0;

void create_checkerboard_texture(Color *buffer, u32 width, u32 height, u32 div){
  for (u32 i = 0; i < height; i++) {
    for (u32 j = 0; j < width; j++) {
      Color color = {
        .a = 0xFF,
        .r = 0x00,
        .g = 0x00,
        .b = 0x00,
      };
      if ((j/div)%2 == (i/div)%2) {
        color.r = 0xFF;
      }
      buffer[i * width + j] = color;
    }
  }
}

BurnCanvas burn_render(float dt){
  BurnCanvas canvas = (BurnCanvas){
    .pixels = pixels,
    .width = DEFAULT_WIDTH,
    .height = DEFAULT_HEIGHT,
  };

  if (!sprite_initialized){
    create_checkerboard_texture(texture, DEFAULT_SPRITE_WIDTH, DEFAULT_SPRITE_HEIGHT, 8);
    sprite_initialized = 1;

  }

  Color color = (Color){
      .r = 0xFF,
      .a = 0xFF
      };

  burn_fill(canvas, (Color){
      .a = 0x00
      });

  static float angle = 0;
  angle += 0.5*PI*(dt*0.01);
  vec2f v1 = (vec2f){ .x = 250, .y = 75 };
  vec2f v2 = (vec2f){ .x = 150, .y = 375 };
  vec2f v3 = (vec2f){ .x = 600, .y = 425 };

  vec2f t1 = (vec2f){ .x = 0, .y = 0 };
  vec2f t2 = (vec2f){ .x = 1, .y = 0 };
  vec2f t3 = (vec2f){ .x = 1, .y = 1 };

  vec2f center = (vec2f){
    .x = (v1.x + v2.x + v3.x) / 3,
    .y = (v1.y + v2.y + v3.y) / 3
  };

  v1 = burn_rotate_2d(v1, center, angle);
  v2 = burn_rotate_2d(v2, center, angle);
  v3 = burn_rotate_2d(v3, center, angle);
  burn_draw_triangle_textured(canvas, v1, v2, v3, t1, t2, t3,
                              (BurnCanvas){
                                  .pixels = texture,
                                  .width = sprite_width,
                                  .height = sprite_height,
                              });

  return canvas;
}

void burn_update(float dt){
}

