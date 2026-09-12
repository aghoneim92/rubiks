#ifndef INITFUNCTIONS_H
#define INITFUNCTIONS_H

#include "Angel.h"

// A random position for one TV-noise speck, drawn from rand() in a fixed order.
vec3 randomNoisePosition();

void build_model();
// Builds the camera and both matrices.  Call once at start-up.
void setup_view();
// Recomputes just the projection, for when the drawable resizes.
void update_projection();
void init();
void cleanup();

#endif
