#ifndef BURN_C
#define BURN_C

#include <stdint.h>

float sinf(float x);
float cosf(float x);

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef struct {
  u8 r, g, b, a;
} Color;

typedef struct {
  Color *pixels;
  u32 width;
  u32 height;
} BurnCanvas;

typedef struct {
  u32 x;
  u32 y;
  u32 w;
  u32 h;
} Rect;

typedef struct {
  i32 x;
  i32 y;
} vec2;

typedef struct {
  float x;
  float y;
} vec2f;

typedef struct {
  i32 x;
  i32 y;
  i32 z;
} vec3;

typedef struct {
  float x;
  float y;
  float z;
} vec3f;

BurnCanvas burn_create_canvas(Color *pixels, u32 width, u32 height);
void burn_draw_line(BurnCanvas canvas, i32 x1, i32 y1, i32 x2, i32 y2,
                    Color color);
void burn_draw_rect(BurnCanvas canvas, i32 x, i32 y, i32 w, i32 h, Color color);
void burn_draw_rect2(BurnCanvas canvas, Rect rect, Color color);
void burn_fill(BurnCanvas canvas, Color color);
void burn_fill_rect(BurnCanvas canvas, i32 x, i32 y, i32 w, i32 h, Color color);
void burn_fill_rect2(BurnCanvas canvas, Rect rect, Color color);
void burn_sprite_copy(BurnCanvas canvas, i32 ox, i32 oy, i32 w, i32 h,
                      BurnCanvas sprite);
void burn_fill_triangle(BurnCanvas canvas, vec2f v1, vec2f v2, vec2f v3,
                        Color color);
void burn_fill_triangle3c(BurnCanvas canvas, vec2f v1, vec2f v2, vec2f v3,
                        Color c1, Color c2, Color c3);
vec2f burn_rotate_2d(vec2f point, vec2f center, float angle);
vec2f burn_project_to_2d(vec3f point);
vec2f burn_to_screen(BurnCanvas canvas, vec2f point);

#define BURN_IMPLEMENTATION
#ifdef BURN_IMPLEMENTATION

#define BURN_PIXEL(canvas, x, y) (canvas).pixels[(y) * (canvas.width) + (x)]
#define BURN_SIGN(T, x) ((T)(0) < x) - (x < (T)(0))
#define BURN_ABS(T, x) (BURN_SIGN(T, x) * (x))
#define BURN_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define BURN_MIN(x, y) (((x) < (y)) ? (x) : (y))

BurnCanvas burn_create_canvas(Color *pixels, u32 width, u32 height) {
  return (BurnCanvas){.pixels = pixels, .width = width, .height = height};
}

void burn_draw_line(BurnCanvas canvas, i32 x1, i32 y1, i32 x2, i32 y2,
                    Color color) {
  i32 dx = BURN_ABS(i32, x2 - x1);
  i32 sx = x1 < x2 ? 1 : -1;
  i32 dy = -BURN_ABS(i32, y2 - y1);
  i32 sy = y1 < y2 ? 1 : -1;

  i32 error = dx + dy;

  while (1) {
    if (x1 < canvas.width && y1 < canvas.height) {
      BURN_PIXEL(canvas, x1, y1) = color;
    }
    i32 e2 = 2 * error;
    if (e2 >= dy) {
      if (x1 == x2)
        break;
      error += dy;
      x1 += sx;
    }
    if (e2 <= dx) {
      if (y1 == y2)
        break;
      error += dx;
      y1 += sy;
    }
  }
}

void burn_fill(BurnCanvas canvas, Color color) {
  for (i32 i = 0; i < canvas.height; i++) {
    for (i32 j = 0; j < canvas.width; j++) {
      BURN_PIXEL(canvas, j, i) = color;
    }
  }
}

void burn_draw_rect(BurnCanvas canvas, i32 x, i32 y, i32 w, i32 h,
                    Color color) {
  burn_draw_line(canvas, x, y, x + w - 1, y, color);
  burn_draw_line(canvas, x + w, y, x + w, y + h - 1, color);
  burn_draw_line(canvas, x + w, y + h, x + 1, y + h, color);
  burn_draw_line(canvas, x, y + h, x, y + 1, color);
}

void burn_draw_rect2(BurnCanvas canvas, Rect rect, Color color) {
  burn_draw_line(canvas, rect.x, rect.y, rect.x + rect.w - 1, rect.y, color);
  burn_draw_line(canvas, rect.x + rect.w, rect.y, rect.x + rect.w,
                 rect.y + rect.h - 1, color);
  burn_draw_line(canvas, rect.x + rect.w, rect.y + rect.h, rect.x + 1,
                 rect.y + rect.h, color);
  burn_draw_line(canvas, rect.x, rect.y + rect.h, rect.x, rect.y + 1, color);
}

void burn_fill_rect(BurnCanvas canvas, i32 x, i32 y, i32 w, i32 h,
                    Color color) {
  for (i32 i = y; i < y + h; i++) {
    for (i32 j = x; j < x + w; j++) {
      if (j < canvas.width && i < canvas.height) {
        BURN_PIXEL(canvas, j, i) = color;
      }
    }
  }
}

void burn_sprite_copy(BurnCanvas canvas, i32 ox, i32 oy, i32 w, i32 h,
                      BurnCanvas sprite) {
  i32 x_min = ox;
  i32 y_min = oy;
  i32 x_max = ox + w;
  i32 y_max = oy + h;

  for (i32 y = y_min; y <= y_max; y++) {
    for (i32 x = x_min; x <= x_max; x++) {
      i32 nx = (x - ox)*(sprite.width)/w;
      i32 ny = (y - oy)*(sprite.height)/h;
      if (nx > 0 && ny > 0 && nx < x_max && ny < y_max) {
        BURN_PIXEL(canvas, x, y) = BURN_PIXEL(sprite, nx, ny);
      }
    }
  }
}

void burn_fill_rect2(BurnCanvas canvas, Rect rect, Color color) {
  for (i32 i = rect.y; i < rect.y + rect.h; i++) {
    for (i32 j = rect.x; j < rect.x + rect.w; j++) {
      if (j < canvas.width && i < canvas.height) {
        BURN_PIXEL(canvas, j, i) = color;
      }
    }
  }
}

float is_top_left(vec2f start, vec2f end){
  vec2f edge = { end.x - start.x, end.y - start.y };

  float is_top_edge = edge.y == 0 && edge.x > 0;
  float is_left_edge = edge.y < 0;

  return is_top_edge || is_left_edge;
}

float burn_edge_cross_product(vec2f a, vec2f b, vec2f p) {
  vec2f ab = { b.y - a.y, b.x - a.x };
  vec2f ap = { p.y - a.y, p.x - a.x };
  return ab.x * ap.y - ap.x * ab.y;
}

u8 burn_interpolate_color(float alpha, float beta, float gamma, u8 c1, u8 c2, u8 c3){
  return (alpha) * c1 + (beta) * c2 + (gamma) * c3;
}

void burn_fill_triangle3c(BurnCanvas canvas, vec2f v1, vec2f v2, vec2f v3,
                        Color c1, Color c2, Color c3) {
  i32 x_min = BURN_MIN(BURN_MIN(v1.x, v2.x), v3.x);
  i32 y_min = BURN_MIN(BURN_MIN(v1.y, v2.y), v3.y);
  i32 x_max = BURN_MAX(BURN_MAX(v1.x, v2.x), v3.x);
  i32 y_max = BURN_MAX(BURN_MAX(v1.y, v2.y), v3.y);

  float delta_w1_col = (v3.y - v2.y);
  float delta_w2_col = (v1.y - v3.y);
  float delta_w3_col = (v2.y - v1.y);

  float delta_w1_row = (v2.x - v3.x);
  float delta_w2_row = (v3.x - v1.x);
  float delta_w3_row = (v1.x - v2.x);

  float bias1 = is_top_left(v2, v3) ? 0 : -1;
  float bias2 = is_top_left(v3, v1) ? 0 : -1;
  float bias3 = is_top_left(v1, v2) ? 0 : -1;

  float area = burn_edge_cross_product(v1, v2, v3);

  vec2f p = {x_min, y_min};
  float w1_row = burn_edge_cross_product(v2, v3, p) + bias1;
  float w2_row = burn_edge_cross_product(v3, v1, p) + bias2;
  float w3_row = burn_edge_cross_product(v1, v2, p) + bias3;

  for (i32 y = y_min; y <= y_max; y++) {
    float w1 = w1_row;
    float w2 = w2_row;
    float w3 = w3_row;
    for (i32 x = x_min; x <= x_max; x++) {

      if (w1 >= 0 && w2 >= 0 && w3 >= 0){
        if (x >= 0 && y >= 0 && x <= canvas.width && y <= canvas.height){
          float alpha = w1 / area;
          float beta  = w2 / area;
          float gamma = w3 / area;

          u8 a = burn_interpolate_color(alpha, beta, gamma, c1.a, c2.a, c3.a);
          u8 r = burn_interpolate_color(alpha, beta, gamma, c1.r, c2.r, c3.r);
          u8 g = burn_interpolate_color(alpha, beta, gamma, c1.g, c2.g, c3.g);
          u8 b = burn_interpolate_color(alpha, beta, gamma, c1.b, c2.b, c3.b);

          Color color = (Color) { .a = a, .r = r, .g = g, .b = b };

          BURN_PIXEL(canvas, x, y) = color;
        }
      }
      w1 += delta_w1_col;
      w2 += delta_w2_col;
      w3 += delta_w3_col;
    }
    w1_row += delta_w1_row;
    w2_row += delta_w2_row;
    w3_row += delta_w3_row;
  }
}

void burn_fill_triangle(BurnCanvas canvas, vec2f v1, vec2f v2, vec2f v3,
                        Color color) {
  i32 x_min = BURN_MIN(BURN_MIN(v1.x, v2.x), v3.x);
  i32 y_min = BURN_MIN(BURN_MIN(v1.y, v2.y), v3.y);
  i32 x_max = BURN_MAX(BURN_MAX(v1.x, v2.x), v3.x);
  i32 y_max = BURN_MAX(BURN_MAX(v1.y, v2.y), v3.y);


  float delta_w1_col = (v3.y - v2.y);
  float delta_w2_col = (v1.y - v3.y);
  float delta_w3_col = (v2.y - v1.y);

  float delta_w1_row = (v2.x - v3.x);
  float delta_w2_row = (v3.x - v1.x);
  float delta_w3_row = (v1.x - v2.x);

  float bias1 = is_top_left(v2, v3) ? 0 : -1;
  float bias2 = is_top_left(v3, v1) ? 0 : -1;
  float bias3 = is_top_left(v1, v2) ? 0 : -1;

  vec2f p = {x_min, y_min};
  float w1_row = burn_edge_cross_product(v2, v3, p) + bias1;
  float w2_row = burn_edge_cross_product(v3, v1, p) + bias2;
  float w3_row = burn_edge_cross_product(v1, v2, p) + bias3;

  for (i32 y = y_min; y <= y_max; y++) {
    float w1 = w1_row;
    float w2 = w2_row;
    float w3 = w3_row;
    for (i32 x = x_min; x <= x_max; x++) {
      if (w1 >= 0 && w2 >= 0 && w3 >= 0)
        if (x >= 0 && y >= 0 && x <= canvas.width && y <= canvas.height)
          BURN_PIXEL(canvas, x, y) = color;

      w1 += delta_w1_col;
      w2 += delta_w2_col;
      w3 += delta_w3_col;
    }
    w1_row += delta_w1_row;
    w2_row += delta_w2_row;
    w3_row += delta_w3_row;
  }
}

void burn_draw_triangle_textured(BurnCanvas canvas, vec2f v1, vec2f v2,
                                 vec2f v3, vec2f tv1, vec2f tv2, vec2f tv3,
                                 BurnCanvas texture) {
  i32 x_min = BURN_MIN(BURN_MIN(v1.x, v2.x), v3.x);
  i32 y_min = BURN_MIN(BURN_MIN(v1.y, v2.y), v3.y);
  i32 x_max = BURN_MAX(BURN_MAX(v1.x, v2.x), v3.x);
  i32 y_max = BURN_MAX(BURN_MAX(v1.y, v2.y), v3.y);

  float delta_w1_col = (v3.y - v2.y);
  float delta_w2_col = (v1.y - v3.y);
  float delta_w3_col = (v2.y - v1.y);

  float delta_w1_row = (v2.x - v3.x);
  float delta_w2_row = (v3.x - v1.x);
  float delta_w3_row = (v1.x - v2.x);

  float bias1 = is_top_left(v2, v3) ? 0 : -1;
  float bias2 = is_top_left(v3, v1) ? 0 : -1;
  float bias3 = is_top_left(v1, v2) ? 0 : -1;

  float area = burn_edge_cross_product(v1, v2, v3);

  vec2f p = {x_min, y_min};
  float w1_row = burn_edge_cross_product(v2, v3, p) + bias1;
  float w2_row = burn_edge_cross_product(v3, v1, p) + bias2;
  float w3_row = burn_edge_cross_product(v1, v2, p) + bias3;

  for (i32 y = y_min; y <= y_max; y++) {
    float w1 = w1_row;
    float w2 = w2_row;
    float w3 = w3_row;
    for (i32 x = x_min; x <= x_max; x++) {
      if (w1 >= 0 && w2 >= 0 && w3 >= 0)
        if (x >= 0 && y >= 0 && x <= canvas.width && y <= canvas.height){
          float z = w1/area + w2/area + w3/area;
          float tx = tv1.x*w1/area + tv2.x*w2/area + tv3.x*w3/area;
          float ty = tv1.y*w1/area + tv2.y*w2/area + tv3.y*w3/area;
          float texture_x = tx/z*texture.width;
          if (texture_x < 0) texture_x = 0;
          if (texture_x >= texture.width) texture_x = texture.width - 1;
          float texture_y = ty/z*texture.height;
          if (texture_y < 0) texture_y = 0;
          if (texture_y >= texture.height) texture_y = texture.height - 1;
          BURN_PIXEL(canvas, x, y) = BURN_PIXEL(texture, (i32)texture_x, (i32)texture_y);
        }

      w1 += delta_w1_col;
      w2 += delta_w2_col;
      w3 += delta_w3_col;
    }
    w1_row += delta_w1_row;
    w2_row += delta_w2_row;
    w3_row += delta_w3_row;
  }
}

vec2f burn_rotate_2d(vec2f point, vec2f center, float angle){
  vec2f rotated;
  point.x -= center.x;
  point.y -= center.y;
  rotated.x = (point.x * cosf(angle)) - (point.y * sinf(angle));
  rotated.y = (point.x * sinf(angle)) + (point.y * cosf(angle));
  rotated.x += center.x;
  rotated.y += center.y;
  return rotated;
}

vec3f burn_rotate_xz(vec3f point, float angle){
  return (vec3f){
    .x = (point.x * cosf(angle)) - (point.z * sinf(angle)),
    .y = point.y,
    .z = (point.x * sinf(angle)) + (point.z * cosf(angle)),
  };
}

vec3f burn_rotate_xy(vec3f point, float angle){
  return (vec3f){
    .x = (point.x * cosf(angle)) - (point.y * sinf(angle)),
    .y = (point.x * sinf(angle)) + (point.y * cosf(angle)),
    .z = point.z,
  };
}

vec3f burn_rotate_yz(vec3f point, float angle){
  return (vec3f){
    .x = point.x,
    .y = (point.y * cosf(angle)) - (point.z * sinf(angle)),
    .z = (point.y * sinf(angle)) + (point.z * cosf(angle)),
  };
}

vec3f burn_translate_x(vec3f point, float dx){
  return (vec3f){.x = point.x+dx, .y = point.y, .z = point.z};
}

vec3f burn_translate_y(vec3f point, float dy){
  return (vec3f){.x = point.x, .y = point.y+dy, .z = point.z};
}

vec3f burn_translate_z(vec3f point, float dz){
  return (vec3f){.x = point.x, .y = point.y, .z = point.z+dz};
}

vec2f burn_project_to_2d(vec3f point){
  float x, y;

  if (point.z > 0){
    x = point.x / point.z;
    y = point.y / point.z;
  } else {
    x = point.x;
    y = point.y;
  }
  return (vec2f){
    .x = x,
    .y = y,
  };
}

vec2f burn_to_screen(BurnCanvas canvas, vec2f point){
  return (vec2f){
    .x = (1 - (point.x + 1)/2) * canvas.width,
    .y = (1 - (point.y + 1)/2) * canvas.height,
  };
}

#endif

#endif

