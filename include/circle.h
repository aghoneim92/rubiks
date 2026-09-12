#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "drawable.h"

// Solid disc drawn as a triangle fan.
class FilledCircle : public Drawable
{
public:
	FilledCircle(vec4 center, GLfloat radius, int segments);

	void render() override;
	void calculateNormals() override;
};

#endif
