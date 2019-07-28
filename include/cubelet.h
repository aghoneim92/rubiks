#ifndef CUBELET_H
#define CUBELET_H
#include "cube.h"
#include "rectangle.h"
#include "drawablegroup.h"

class Cubelet:public DrawableGroup{
public:
	Cubelet(vec3 offset);
	Cubelet();
	void glowRims(int nFrames, short direction);
	void resetRims();
protected:
	vec3 offset;
	Drawable* blackRims[12];
	friend class RubiksCube;
};

#endif