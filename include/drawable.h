#ifndef __DRAWABLE_H__
#define __DRAWABLE_H__

#include "Angel.h"
#include "light_source.h"
#include "material.h"

#include <vector>

// A chunk of geometry with its own transform and material.  Subclasses fill in
// `points`, then call init() to upload them.
class Drawable
{
public:
	static GLuint shaderProgram;

	vec3 scale = vec3(1.0f, 1.0f, 1.0f);
	vec3 translation = vec3(0.0f, 0.0f, 0.0f);
	vec3 rotation = vec3(0.0f, 0.0f, 0.0f);
	vec3 rotationAfter = vec3(0.0f, 0.0f, 0.0f);

	std::vector<vec4> points;
	std::vector<vec4> normals;
	Material material;

	bool overlay = false;

	Drawable() = default;
	Drawable(const Drawable &) = delete;
	Drawable &operator=(const Drawable &) = delete;
	virtual ~Drawable();

	// Copies points and normals back into the GL buffers.
	void update();

	// Calculates a unit normal vector for each vertex.
	virtual void calculateNormals() {}

	virtual void render() {}

protected:
	// Allocates the buffers and uploads whatever is currently in `points`.
	void init();

	GLuint vao = 0;
	GLuint vbuffer = 0;
	GLuint nbuffer = 0;
};

#endif
