const DEFAULT_WIDTH = 800
const DEFAULT_HEIGHT = 600

const libs = {
  env: {
    cosf: (x) => Math.cos(x),
    sinf: (x) => Math.sin(x),
  }
};

const FPS = 60.0
const MILLIS_PER_SECOND = 1000.0
const DELTA_TIME = MILLIS_PER_SECOND/FPS;

async function start(canvasId, wasmPath) {
  const w = await WebAssembly.instantiateStreaming(fetch(wasmPath), libs)
  const app = document.getElementById(`app-${canvasId}`)
  const ctx = app.getContext("2d")

  const div = document.getElementById(`div-${canvasId}`)
  let state = { paused: false };
  div.addEventListener("mouseenter", () => state.paused = false)
  div.addEventListener("mouseleave", () => state.paused = true)

  let prev_time = 0

  function loop(cur_time) {
    const dt = (cur_time - prev_time) / DELTA_TIME;
    prev_time = cur_time
    if (!state.paused) {
      render(w, ctx, dt)
    }
    window.requestAnimationFrame(loop)
  }
  window.requestAnimationFrame(loop)

}

function getBurnCanvas(buffer, canvas_ptr){
  const memory = new Uint32Array(buffer, canvas_ptr, 3)
  return {
    pixels: memory[0],
    width:  memory[1],
    height: memory[2],
  }
}

function render(w, ctx, dt) {
  const memory = w.instance.exports.memory

  const heap_base = w.instance.exports.__heap_base.value
  w.instance.exports.burn_render(heap_base, dt)
  const canvas = getBurnCanvas(memory.buffer, heap_base)

  const pixels_offset = canvas.pixels
  const width = canvas.width
  const height = canvas.height

  const pixels = new Uint8ClampedArray(memory.buffer, pixels_offset, width * height * 4)
  rgba_to_bgra(pixels)

  const image = new ImageData(pixels, width, height)
  ctx.canvas.width = width
  ctx.canvas.height = height

  ctx.putImageData(image, 0, 0)
}

function rgba_to_bgra(pixels){
  for(let i = 0; i < pixels.length; i+=4){
    const b = pixels[i];
    const r = pixels[i+2];
    pixels[i] = r;
    pixels[i+2] = b;
  }
}
