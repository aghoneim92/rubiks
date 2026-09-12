#ifndef INPUTFUNCTIONS_H
#define INPUTFUNCTIONS_H

#include "platform.h"

void keyboard(unsigned char key);
void mouse(bool released, int x, int y);
void special(platform::SpecialKey key);

#endif
