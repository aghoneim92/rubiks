#ifndef ANIMATIONS_H
#define ANIMATIONS_H

// Every animation is a self-rescheduling timer callback; the int argument is the
// frame counter within that animation.

void rotateFace(int animIndex);
void animateWater(int unused);
void shuffle();
void initGlowStep();
void glowCube(int animIndex);
void glowRims(int animIndex);
void floatView(int animIndex);
void animateAssemble(int unused);
void animateLSD(int animIndex);
void animateNoise(int animIndex);
void animateHarlemShake1(int animIndex);
void animateHarlemShake2(int animIndex);
void animateHarlemShake3(int animIndex);

#endif
