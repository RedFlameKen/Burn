#ifndef BURN_C
#define BURN_C

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef struct {
  u8 b, g, r, a;
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
  i32 x;
  i32 y;
  i32 z;
} vec3;

BurnCanvas burn_create_canvas(Color *pixels, u32 width, u32 height);
void burn_draw_line(BurnCanvas canvas, i32 x1, i32 y1, i32 x2, i32 y2,
                    Color color);
void burn_draw_rect(BurnCanvas canvas, i32 x, i32 y, i32 w, i32 h, Color color);
void burn_draw_rect2(BurnCanvas canvas, Rect rect, Color color);
void burn_fill(BurnCanvas canvas, Color color);
void burn_fill_rect(BurnCanvas canvas, i32 x, i32 y, i32 w, i32 h, Color color);
void burn_fill_rect2(BurnCanvas canvas, Rect rect, Color color);
void burn_fill_triangle(BurnCanvas canvas, vec2 v1, vec2 v2, vec2 v3,
                        Color color);

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

void burn_fill_rect2(BurnCanvas canvas, Rect rect, Color color) {
  for (i32 i = rect.y; i < rect.y + rect.h; i++) {
    for (i32 j = rect.x; j < rect.x + rect.w; j++) {
      if (j < canvas.width && i < canvas.height) {
        BURN_PIXEL(canvas, j, i) = color;
      }
    }
  }
}

i32 burn_edge_cross_product(vec2 a, vec2 b, vec2 p) {
  vec2 ab = { b.y - a.y, b.x - a.x };
  vec2 ap = { p.y - a.y, p.x - a.x };
  return ab.x * ap.y - ap.x * ab.y;
}

void burn_fill_triangle(BurnCanvas canvas, vec2 v1, vec2 v2, vec2 v3,
                        Color color) {
  i32 x_min = BURN_MIN(BURN_MIN(v1.x, v2.x), v3.x);
  i32 y_min = BURN_MIN(BURN_MIN(v1.y, v2.y), v3.y);
  i32 x_max = BURN_MAX(BURN_MAX(v1.x, v2.x), v3.x);
  i32 y_max = BURN_MAX(BURN_MAX(v1.y, v2.y), v3.y);

  for (i32 y = y_min; y <= y_max; y++) {
    for (i32 x = x_min; x <= x_max; x++) {
      vec2 p = {x, y};
      i32 w1 = burn_edge_cross_product(v2, v3, p);
      i32 w2 = burn_edge_cross_product(v3, v1, p);
      i32 w3 = burn_edge_cross_product(v1, v2, p);

      if (w1 >= 0 && w2 >= 0 && w3 >= 0)
        BURN_PIXEL(canvas, x, y) = color;

    }
  }
}

#endif

#endif
