# rubiks

OpenGL Rubik's Cube — runs natively on macOS/Linux/Windows and in the browser via
WebAssembly.

![Preview](https://github.com/aghoneim92/rubiks/raw/master/rubiks.gif)

## Controls

| Input | Action |
| --- | --- |
| <kbd>←</kbd> <kbd>→</kbd> | Cycle which axis the highlighted face turns around |
| <kbd>↑</kbd> <kbd>↓</kbd> | Cycle which slice along that axis is highlighted |
| <kbd>+</kbd> <kbd>−</kbd> | Turn the highlighted slice one way or the other |
| <kbd>W</kbd> <kbd>A</kbd> <kbd>S</kbd> <kbd>D</kbd> | Orbit the camera |
| Drag | Grab a face and swipe it round |
| <kbd>Q</kbd> / <kbd>Esc</kbd> | Quit (desktop only) |

There are also a few easter eggs; type them and see.

## Building

### Web (WebAssembly)

Needs the [Emscripten SDK](https://emscripten.org/docs/getting_started/downloads.html)
(`brew install emscripten` works on macOS).

```
emcmake cmake -S . -B build-web -DCMAKE_BUILD_TYPE=Release
cmake --build build-web
```

That writes `index.html`, `index.js` and `index.wasm` into `build-web/`. They need
to be served over HTTP rather than opened from disk:

```
cd build-web && python3 -m http.server 8000
```

then open <http://localhost:8000>.

### macOS

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Then run `./build/Cube`. Pass `--fullscreen` to take over the whole screen.

### Linux

Install the development packages for OpenGL, GLUT (or FreeGLUT) and GLEW (package
names vary per distribution), then build exactly as above.

### Windows

From a Visual Studio Developer Command Prompt, with the OpenGL, GLUT and GLEW
development libraries visible to CMake:

```
cmake -S . -B build
cmake --build build --config Release
```

## Layout

```
include/, src/   the game
  platform.h     windowing, input and animation timers, behind one interface
  platform_glut.cpp / platform_web.cpp   the two backends
  picking.cpp    turns a click into a point in world space
  renderer.cpp   draws the scene (shared by the visible and picking passes)
shaders/         GLSL, baked into the binary at build time by CMake
web/shell.html   the page the WebAssembly build is served in
```

The shaders are written to the GLSL ES 1.00 subset so the same source compiles
against desktop OpenGL 2.1 and WebGL 2 without changes.
