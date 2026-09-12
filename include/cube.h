#ifndef __CUBE_H__
#define __CUBE_H__

#include "drawable.h"

// Wireframe box.
class Cube : public Drawable
{
public:
	Cube(vec4 center, float width, float height, float depth);

	void render() override;
	// Wireframe cubes are not shaded, so they need no normals.
	void calculateNormals() override {}
};

// Solid box, one face normal per vertex.
class FilledCube : public Drawable
{
public:
	FilledCube(vec4 center, float width, float height, float depth);

	void render() override;
	void calculateNormals() override;
};

#endif
