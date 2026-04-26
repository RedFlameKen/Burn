#ifndef LOADER_C
#define LOADER_C

#define BURN_IMPLEMENTATION
#include "../burn.c"

#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600
#define FPS 60.0f

#define WAYLAND_BACKEND 0
#define SDL_BACKEND     1
#define WASM_BACKEND    2


#ifndef DISPLAY_BACKEND
#error "Please define a DISPLAY_BACKEND macro"
#endif

#if DISPLAY_BACKEND == SDL_BACKEND
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_oldnames.h>
#include <stdio.h>
#include <SDL3/SDL_pixels.h>

#define TIME_IMPLEMENTATION
#include "util/time.c"

#endif

typedef struct {
#if DISPLAY_BACKEND == SDL_BACKEND
  SDL_Window *window;
  SDL_Surface *surface;
  SDL_Renderer *renderer;
  u8 running;
#endif
} Display;

BurnCanvas burn_render(float dt);
void burn_update(float dt);
float costhis(float x);

#if DISPLAY_BACKEND == SDL_BACKEND

void sdl_render(Display *display, float dt){
  BurnCanvas canvas = burn_render(dt);
  Color *pixels = (Color*)display->surface->pixels;

  SDL_LockSurface(display->surface);
  for (int i = 0; i < canvas.height; i++) {
    for (int j = 0; j < canvas.width; j++) {
      Color color = BURN_PIXEL(canvas, j, i);
      u8 temp = color.r;
      color.r = color.b;
      color.b = temp;
      pixels[i * canvas.width + j] = color;
    }
  }
  SDL_UnlockSurface(display->surface);
  SDL_UpdateWindowSurface(display->window);

}

void sdl_loop(Display *display){
  display->running = 1;
  u64 prev_time = current_time_nanos();
  u64 cur_time;

  float dt = 0;
  float delta_time = NANOS_PER_SECOND / FPS;

  u64 prev_check = current_time_millis();
  u64 cur_check;

  u32 frames = 0;

  while(display->running) {
    cur_time = current_time_nanos();

    dt += (cur_time - prev_time) / delta_time;
    prev_time = cur_time;

    if (dt >= 1){
      SDL_Event e;
      while(SDL_PollEvent(&e)){
        if (e.type == SDL_EVENT_QUIT) {
          display->running = 0;
        }
      }
      // burn_update(dt);
      sdl_render(display, dt);
      dt--;
      frames++;
    }

    cur_check = current_time_millis();
    if ((cur_check - prev_check) >= MILLIS_PER_SECOND) {
      fprintf(stderr, "FPS: %d\n", frames);
      frames = 0;
      prev_check = cur_check;
    }

  }

}

int sdl_open_window(Display *display){
  SDL_SetAppMetadata("Burn Demo", "1.0", "com.redflameken.burn_demo");
  if(!SDL_Init(SDL_INIT_VIDEO)){
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return -1;
  }

  if (!(display->window = SDL_CreateWindow("Burn Demo", DEFAULT_WIDTH, DEFAULT_HEIGHT, 0))) {
    fprintf(stderr, "Unable to create window: %s\n", SDL_GetError());
    return -1;
  }


  if (!(display->surface = SDL_GetWindowSurface(display->window))) {
    fprintf(stderr, "Unable to get window surface: %s\n", SDL_GetError());
    return -1;
  }

  return 0;
}

int main(int argc, char *argv[]) {
  Display display = (Display){0};
  sdl_open_window(&display);

  sdl_loop(&display);

}

#endif

#endif
