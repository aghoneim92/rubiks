#ifndef PICKING_H
#define PICKING_H

#include "Angel.h"

// Turns a click into a point in world space.
//
// The original desktop code read GL_DEPTH_COMPONENT straight out of the default
// framebuffer and fed it to gluUnProject.  WebGL allows neither, so the scene is
// re-rendered into an off-screen buffer whose fragment shader packs the depth
// into RGBA8; that pixel is read back and unprojected on the CPU.  Both builds
// take this path so they behave identically.
namespace picking
{

// (Re)allocates the off-screen buffer.  Call whenever the drawable resizes.
void resize(int width, int height);

// World-space position under the given framebuffer pixel, with y measured from
// the top of the window (the convention both platform backends report).
vec3 worldPositionAt(int x, int y);

} // namespace picking

#endif
