const DEFAULT_WIDTH = 800
const DEFAULT_HEIGHT = 600

const libs = {
  env: {
    cosf: (x) => Math.cos(x),
    sinf: (x) => Math.sin(x),
  }
};

async function start(canvasId, wasmPath) {
  const w = await WebAssembly.instantiateStreaming(fetch(wasmPath), libs)
  const app = document.getElementById(`app-${canvasId}`)
  const ctx = app.getContext("2d")

  prev_time = 0;
  function loop(cur_time) {
    const dt = cur_time - prev_time;
    prev_time = cur_time
    render(w, ctx, dt)
    window.requestAnimationFrame(loop)
  }
  window.requestAnimationFrame(loop)

}

function render(w, ctx, dt) {
  const memory = w.instance.exports.memory
  const canvas_offset = w.instance.exports.burn_render(dt*0.001)
  const view = new DataView(memory.buffer, canvas_offset, 12)

  const pixels_offset = view.getInt32(0, true)
  const width = view.getInt32(4, true)
  const height = view.getInt32(8, true)

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
