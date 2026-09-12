//////////////////////////////////////////////////////////////////////////////
//
//  --- Angel.h ---
//
//   Vector/matrix helpers from Angel 6th Edition, plus the GL headers.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ANGEL_H__
#define __ANGEL_H__

#include <cmath>
#include <cstdio>
#include <iostream>

//  Define M_PI in the case it's not defined in the math header file
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

//----------------------------------------------------------------------------
//
// --- GL headers ---
//
//  The desktop builds run against legacy OpenGL (2.1 on macOS, whatever GLEW
//  finds elsewhere); the web build runs against WebGL 2 through GLES 3.
//

#if defined(__EMSCRIPTEN__)
#include <GLES3/gl3.h>
#elif defined(__APPLE__)
#define GL_SILENCE_DEPRECATION 1
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
// macOS only exposes vertex array objects through ARB_/APPLE_vertex_array_object.
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#else
#include <GL/glew.h>
#endif

// Define a helpful macro for handling offsets into buffer objects
#define BUFFER_OFFSET(offset) ((GLvoid *)(offset))

//----------------------------------------------------------------------------
//
//  --- Include our class libraries and constants ---
//

namespace Angel
{

//  Defined constant for when numbers are too small to be used in the
//    denominator of a division operation.  This is only used if the
//    DEBUG macro is defined.
const GLfloat DivideByZeroTolerance = GLfloat(1.0e-07);

//  Degrees-to-radians constant
const GLfloat DegreesToRadians = M_PI / 180.0f;

} // namespace Angel

#include "vec.h"
#include "mat.h"

//  Globally use our namespace in our example programs.
using namespace Angel;

typedef vec3 color3;
typedef vec4 color4;
typedef vec4 point4;

#endif // __ANGEL_H__
