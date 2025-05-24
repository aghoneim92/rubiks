# rubiks

OpenGL Rubiks Cube

![Preview](https://github.com/aghoneim92/rubiks/raw/master/rubiks.gif)

## Building

### macOS

Run the following:

```
mkdir build && cd build
cmake ..
make
```

Then run `./Cube` from the `build` folder

### Linux

Install the development packages for OpenGL, GLUT (or FreeGLUT) and GLEW
(package names vary per distribution). Then run:

```
mkdir build && cd build
cmake ..
make
```

Run `./Cube` from the `build` folder.

### Windows

Use a terminal such as the Visual Studio Developer Command Prompt and ensure
that the OpenGL, GLUT and GLEW development libraries are installed and visible
to CMake. Then run:

```
mkdir build && cd build
cmake ..
cmake --build .
```

The resulting executable `Cube.exe` will be placed in the `build` directory.
