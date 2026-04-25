#include "../loader.c"
#include "points.c"

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
      .a = 0x00,
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


  for (int i = 0; i < 12; i++) {
    i32 *face = cube_faces[i];
    vec3f a = cube_points[face[0]];
    vec3f b = cube_points[face[1]];
    vec3f c = cube_points[face[2]];

    a = burn_rotate_xz(a, angle);
    b = burn_rotate_xz(b, angle);
    c = burn_rotate_xz(c, angle);

    a = burn_rotate_yz(a, -0.15*PI);
    b = burn_rotate_yz(b, -0.15*PI);
    c = burn_rotate_yz(c, -0.15*PI);

    a = burn_translate_z(a, 1);
    b = burn_translate_z(b, 1);
    c = burn_translate_z(c, 1);

    vec2f v1 = burn_to_screen(canvas, burn_project_to_2d(a));
    vec2f v2 = burn_to_screen(canvas, burn_project_to_2d(b));
    vec2f v3 = burn_to_screen(canvas, burn_project_to_2d(c));

    if (a.z > 0.1 && b.z > 0.1 && c.z > 0.1) {
      burn_fill_triangle3c(canvas, v1, v2, v3, c1, c2, c3);
    }

  }

  return canvas;
}

void burn_update(float dt){
}
