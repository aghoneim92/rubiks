#ifndef SHADER_H
#define SHADER_H

#include "Angel.h"

// Compiles and links a program from GLSL source held in memory.  Both shaders
// are embedded into the binary at build time (see cmake/embedded_shaders.h.in)
// so the program has no runtime working-directory dependency, which matters for
// the web build where there is no filesystem to read from.
GLuint buildShaderProgram(const char *vertexSource, const char *fragmentSource);

#endif
