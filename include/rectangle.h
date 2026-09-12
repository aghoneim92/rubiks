#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "drawable.h"

// Wireframe quad.
class Rectangle : public Drawable
{
public:
	Rectangle() = default;
	Rectangle(vec4 center, float width, float height);

	void render() override;
	void calculateNormals() override {}
};

// Solid quad, one shared normal.
class FilledRectangle : public Rectangle
{
public:
	FilledRectangle(vec4 center, float width, float height);
	explicit FilledRectangle(const vec4 corners[4]);

	void render() override;
	void calculateNormals() override;
};

#endif
