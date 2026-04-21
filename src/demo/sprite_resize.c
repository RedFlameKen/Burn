#include "../loader.c"

#define DEFAULT_SPRITE_WIDTH 300
#define DEFAULT_SPRITE_HEIGHT 300

#define MIN_WIDTH_STRETCH 200
#define MAX_WIDTH_STRETCH 400

#define MIN_HEIGHT_STRETCH 200
#define MAX_HEIGHT_STRETCH 400

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

  if (grow_width) {
    if (sprite_width < MAX_WIDTH_STRETCH && sprite_height > MIN_HEIGHT_STRETCH) {
      sprite_width += 5*dt;
      sprite_height -= 5*dt;
      x_offset -= 2.5*dt;
      y_offset += 2.5*dt;
    } else {
      grow_width = !grow_width;
    }
  } else {
    if (sprite_width > MIN_WIDTH_STRETCH && sprite_height < MAX_HEIGHT_STRETCH) {
      sprite_width -= 5*dt;
      sprite_height += 5*dt;
      x_offset += 2.5*dt;
      y_offset -= 2.5*dt;
    } else {
      grow_width = !grow_width;
    }
  }

  burn_sprite_copy(canvas, 250+x_offset, 150+y_offset, sprite_width, sprite_height,
                   (BurnCanvas){
                       .pixels  = texture,
                       .width   = DEFAULT_SPRITE_WIDTH,
                       .height  = DEFAULT_SPRITE_HEIGHT,
                   });

  return canvas;
}

void burn_update(float dt){
}

