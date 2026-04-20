project_name := burn

libs := sdl3
flags := -g

.PHONY: build all build-wasm clean

all: build build-wasm

build:
	clang -x c -std=c11 -DDISPLAY_BACKEND=SDL_BACKEND -o bin/test src/demo/test.c $(shell pkg-config --cflags --libs $(libs)) $(flags)

build-wasm:
	clang --target=wasm32 -DDISPLAY_BACKEND=WASM_BACKEND -nostdlib -Wl,--no-entry -Wl,--export-all -o bin/test.wasm src/demo/test.c

clean:
	rm -rf bin/*
