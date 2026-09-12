#ifndef DRAWABLEGROUP_H
#define DRAWABLEGROUP_H

#include "drawable.h"

#include <vector>

// A non-owning collection of drawables that can be transformed as a unit.
// Ownership stays with whoever created the drawables (see Cubelet).
class DrawableGroup
{
public:
	DrawableGroup() = default;

	void translate(vec3 translation);
	void rotate(vec3 rotation);
	void scale(vec3 scale);
	void rotateAfter(vec3 rotation);

	// Bakes a rotation into the vertex data itself, so the transient
	// `rotationAfter` can be cleared once a face has finished turning.
	void rotatePermanentX(float angle);
	void rotatePermanentY(float angle);
	void rotatePermanentZ(float angle);

	const std::vector<Drawable *> &getChildren() const { return children; }
	vec3 getRotationAfter() const { return children[0]->rotationAfter; }
	size_t size() const { return children.size(); }

protected:
	void rotatePermanent(const mat4 &rotation);

	std::vector<Drawable *> children;
};

#endif
