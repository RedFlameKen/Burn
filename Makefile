project_name := burn

libs := sdl3
flags := -g

.PHONY: build all build-wasm clean

all: build build-wasm

build:
	clang -x c -std=c11 -DDISPLAY_BACKEND=SDL_BACKEND -o bin/test src/demo/test.c -lm $(shell pkg-config --cflags --libs $(libs)) $(flags)
	clang -x c -std=c11 -DDISPLAY_BACKEND=SDL_BACKEND -o bin/triangle src/demo/triangle.c -lm $(shell pkg-config --cflags --libs $(libs)) $(flags)
	clang -x c -std=c11 -DDISPLAY_BACKEND=SDL_BACKEND -o bin/sprite_resize src/demo/sprite_resize.c -lm $(shell pkg-config --cflags --libs $(libs)) $(flags)
	clang -x c -std=c11 -DDISPLAY_BACKEND=SDL_BACKEND -o bin/triangle_textured src/demo/triangle_textured.c -lm $(shell pkg-config --cflags --libs $(libs)) $(flags)

build-wasm:
	clang -fno-builtin --target=wasm32 --no-standard-libraries -DDISPLAY_BACKEND=WASM_BACKEND -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -o bin/test.wasm src/demo/test.c
	clang -fno-builtin --target=wasm32 --no-standard-libraries -DDISPLAY_BACKEND=WASM_BACKEND -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -o bin/triangle.wasm src/demo/triangle.c
	clang -fno-builtin --target=wasm32 --no-standard-libraries -DDISPLAY_BACKEND=WASM_BACKEND -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -o bin/sprite_resize.wasm src/demo/sprite_resize.c
	clang -fno-builtin --target=wasm32 --no-standard-libraries -DDISPLAY_BACKEND=WASM_BACKEND -nostdlib -Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined -o bin/triangle_textured.wasm src/demo/triangle_textured.c

clean:
	rm -rf bin/*
