#ifndef DRAWABLEGROUP_H
#define DRAWABLEGROUP_H
#include "drawable.h"

class DrawableGroup{
public:
	DrawableGroup();
	DrawableGroup(Drawable** children, int size);
	DrawableGroup(DrawableGroup* childGroups, int n);

	void translate(vec3 translation);
	void rotate(vec3 rotation);
	void scale(vec3 scale);
	void rotateAfter(vec3 rotation);
	void rotatePermanentX(float angle);
	void rotatePermanentY(float angle);
	void rotatePermanentZ(float angle);
	Drawable** getChildren();
	vec3 getRotationAfter(){
		return children[0]->rotationAfter;
	}
	int size();
	Drawable** start();
	Drawable** end();
protected:

	Drawable** children;
	int _size;
};

#endif
