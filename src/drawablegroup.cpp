#include "drawablegroup.h"

void DrawableGroup::translate(vec3 translation)
{
	for (Drawable *child : children)
		child->translation += translation;
}

void DrawableGroup::rotate(vec3 rotation)
{
	for (Drawable *child : children)
		child->rotation += rotation;
}

void DrawableGroup::rotateAfter(vec3 rotation)
{
	for (Drawable *child : children)
		child->rotationAfter += rotation;
}

void DrawableGroup::scale(vec3 scale)
{
	for (Drawable *child : children)
		child->scale = scale;
}

void DrawableGroup::rotatePermanent(const mat4 &rotation)
{
	for (Drawable *child : children)
		for (vec4 &point : child->points)
			point = rotation * point;
}

void DrawableGroup::rotatePermanentX(float angle)
{
	rotatePermanent(RotateX(angle * 180 / M_PI));
}

void DrawableGroup::rotatePermanentY(float angle)
{
	rotatePermanent(RotateY(angle * 180 / M_PI));
}

void DrawableGroup::rotatePermanentZ(float angle)
{
	rotatePermanent(RotateZ(angle * 180 / M_PI));
}
